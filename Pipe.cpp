#include <QDebug>

#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>

#include "Boxes/BoxCollector.h"
#include "Hydraulics/Fluid.h"
#include "Pipe.h"

Pipe::Pipe(Pipe::PipeType type, Pipe::SpecificLoss loss, Fluid* fluid)
{
    initialize(type, loss, fluid);
    connectSlots();
}

void Pipe::initialize(PipeType type, SpecificLoss loss, Fluid* fluid)
{
    switch (type)
    {
    case STEEL_CLEAN:
        rough = 0.2;
        break;
    case STEEL_RUSTY:
        rough = 2.0;
        break;
    case PLASTIC:
        rough = 0.005;
        break;
    default:
        rough = 0.2;
        break;
    }

    switch (loss)
    {
    case PA_PER_M_80:
        pressureLossCriteria = 85;
        break;
    case PA_PER_M_100:
        pressureLossCriteria = 105;
        break;
    case PA_PER_M_130:
        pressureLossCriteria = 135;
        break;
    case PA_PER_M_150:
        pressureLossCriteria = 155;
        break;
    default:
        pressureLossCriteria = 85;
        break;
    }

    m_dn = 15;
    fillPipeData();
    m_fluid = fluid;
    m_specPressure = 0;
    m_speed = 0;
}

void Pipe::connectSlots()
{
    if (m_fluid != 0)
    {
        connect(m_fluid, SIGNAL(fluidChanged()), SLOT(fitPipe()));
    }
}


void Pipe::fitPipe()
{
    QSqlRecord record;
    QSqlQuery query;
    QString queryStr;

    queryStr = "SELECT Dn, Din FROM Pipes;";

    query.exec(queryStr);
    record = query.record();

    m_speed = 10.0;
    m_specPressure = 500.0;

    bool isSpeedOk = false;
    bool isPressOk = false;

    while ( query.next() && (!isSpeedOk || !isPressOk) )
    {
        m_dn  = query.value(record.indexOf("Dn")).toInt();
        m_din = query.value(record.indexOf("Din")).toDouble();
        m_CSA = PI * m_din * m_din / (4 * mmToM * mmToM);
        m_speed = m_fluid->flow()->flow() / (hToSec * m_CSA);

        computePipe();

        isSpeedOk = m_speed < speedCriteria;
        isPressOk = m_specPressure < pressureLossCriteria;
    }
    fillPipeData();
}

void Pipe::fitPipe(Flow flow)
{
    m_fluid = new Fluid(this);
    m_fluid->temperature()->setTemperature(75, 65);
    m_fluid->setFlow(flow);
    fitPipe();
}

void Pipe::fitPipe(BoxCollector* collector)
{
    QSqlRecord record;
    QSqlQuery query;
    QString queryStr;

    queryStr = "SELECT pipeName, Din, Dn FROM Pipes;";

    query.exec(queryStr);
    record = query.record();

    m_CSA = 0;

    while ( query.next() && (m_CSA < collector->getBranchesArea()) )
    {
        m_din = query.value(record.indexOf("Din")).toDouble();
        m_dn  = query.value(record.indexOf("Dn")).toInt();
        m_CSA = PI * m_din * m_din / (4 * mmToM * mmToM);
    }
    fillPipeData();
}


void Pipe::computePipe()
{
    computeFriction();
    conditionTest();
    computeLambda();

    m_specPressure = mmToM * (m_lambda / m_din) *
                     m_fluid->thermoPhysics()->density() * pow(m_speed, 2) / 2;

    computeDynamic();
}

void Pipe::computeFriction()
{
    m_reinolds = (m_din / mmToM ) * m_speed *
                  m_fluid->thermoPhysics()->density() /
                  m_fluid->thermoPhysics()->viscosity();

    friction   = m_reinolds * rough / m_din;
}

void Pipe::conditionTest()
{
    isLaminar    = m_reinolds < 2300;
    isTurbulence = m_reinolds < 125000;
    isSmoothPipe = friction < 23;
    isRoughPipe  = friction > 560;
}

void Pipe::computeLambda()
{
    if (isLaminar)
    {
        m_lambda = 64 / m_reinolds;

        lambdaStr = "lambda = 64 / Re";
    } else {
        if (isSmoothPipe)
        {
            if (isTurbulence)
            {
                m_lambda = 0.3164 / pow(m_reinolds, 0.25);

                lambdaStr = "lambda = 0.3164 / Re^0.25)";
            } else {
                m_lambda = 0.0032 + 0.221 / pow(m_reinolds, 0.237);

                lambdaStr = "lambda = 0.0032 + 0.221 / Re^0.237";
            }
        } else {
            if (isRoughPipe)
            {
                m_lambda = 1 / pow( (1.14 + 2 * log10(m_din / rough)), 2 );

                lambdaStr = "lambda = 1 / ( 1.14 + 2 * log(Din/K) )^2";
            } else {
                m_lambda = 0.11 * pow( (rough/m_din + 68/m_reinolds), 0.25 );

                lambdaStr = "0.11 * ( K/D + 68/Re )^0.25";
            }
        }
    }
}

void Pipe::computeDynamic()
{
    m_dynamPressure = m_localPressLoss *
                     m_fluid->thermoPhysics()->density() * pow(m_speed, 2) / 2;

    m_totalPressureLoss = 2 * m_specPressure * m_routeLenght * m_dynamPressure;
}

void Pipe::fillPipeData()
{
    QSqlRecord record;
    QSqlQuery query;
    QString queryStr;

    queryStr = "SELECT Dn, Dext, S, GOST, pipeName FROM Pipes "
                "WHERE Dn = %1;";

    query.exec(queryStr.arg(m_dn));
    record = query.record();

    while (query.next())
    {
        m_dext  = query.value(record.indexOf("Dext")).toDouble();
        m_s     = query.value(record.indexOf("S")).toDouble();
        gostStr = query.value(record.indexOf("GOST")).toString();
    }
}


//=============================================================================
// Getters

QString Pipe::pipeName()
{
    QSqlQuery query;
    QSqlRecord record;
    QString queryStr, clearName;

    queryStr = "SELECT Dn, pipeName FROM Pipes "
                "WHERE Dn = %1;";

    query.exec(queryStr.arg(m_dn));
    record = query.record();

    bool isFlanged = (m_dn > 50);

    while (query.next())
    {
        if (isFlanged)
        {
            clearName = query.value(record.indexOf("pipeName")).toString();
            pipeNameStr = "Ø" + clearName;
            return pipeNameStr;
        } else {
            clearName = query.value(record.indexOf("Dn")).toString();
            pipeNameStr = "Ду" + clearName;
            return pipeNameStr;
        }
    }
    return tr("Неизвестная труба");
}

qreal Pipe::dn() const
{
    return m_dn;
}

qreal Pipe::dext() const
{
    return m_dext;
}

qreal Pipe::din() const
{
    return m_din;
}

qreal Pipe::s() const
{
    return m_s;
}

qreal Pipe::CSA() const
{
    return m_CSA;
}

qreal Pipe::velocity() const
{
    return m_speed;
}

qreal Pipe::specPressure() const
{
    return m_specPressure;
}
