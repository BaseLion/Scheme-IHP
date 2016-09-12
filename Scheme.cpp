#include <QGraphicsScene>
#include <QMessageBox>

#include <QFileDialog>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSql>
#include <QDir>

#include "Scheme.h"

#include "Ui_Boxes/Ui_BoxHeatExch.h"
#include "Info/InfoActuatorValve.h"
#include "Boxes/BoxCollector.h"
#include "Boxes/BoxPresValve.h"
#include "Boxes/BoxHwsJoint.h"
#include "Boxes/BoxTwoStage.h"
#include "Boxes/BoxHeatExch.h"
#include "Boxes/BoxRechMain.h"
#include "Hydraulics/Fluid.h"
#include "Boxes/BoxVessel.h"
#include "Boxes/BoxLeadin.h"
#include "Units/Pressure.h"
#include "Info/InfoPump.h"
#include "Boxes/BoxPump.h"
#include "Boxes/BoxDhw.h"
#include "lagrange.h"
#include "Pipe.h"

QSqlDatabase Scheme::dataBase = QSqlDatabase::addDatabase("QSQLITE", "SQLITE");

Scheme::Scheme(QWidget* parent) : Ui_Scheme(parent)
{
    createDbConnection();
//    setDiffPressData();
    setPresExchData();
}

bool Scheme::createDbConnection()
{
    QString path(QDir::current().absolutePath());
    qDebug() << path;
    path = QFileDialog::getOpenFileName();
    dataBase.setDatabaseName(path);
    qDebug() << path;


//    dataBase.setDatabaseName("/SchemeIHP.db");

    if (!dataBase.open())
    {
        QMessageBox::warning(0, tr("Ошибка базы данных:"), dataBase.lastError().text()
                             + "\n" + path);
    }
    qDebug() << QSqlDatabase::drivers();
    return true;
}

void Scheme::connectSlots()
{
    connect(leadin, SIGNAL(leadinChanged()), SLOT(setLeadinData()));
    connect(leadin, SIGNAL(leadinChanged()), SLOT(setVesselData()));
    connect(leadin, SIGNAL(leadinChanged()),
            presValve, SIGNAL(diffValveChanged()));
    connect(leadin,    SIGNAL(leadinChanged()),    SLOT(setPresExchData()));
    connect(presValve, SIGNAL(diffValveChanged()), SLOT(setPresExchData()));

    if (isTwoStaged)
    {
        connect(twoStage, SIGNAL(twoStageChanged(int)),
                     dhw,   SLOT(setCunsumLoad(int)));

        connect( twoStage, SIGNAL(twoStageChanged(int)),
                             SLOT(setTwoStageData()));
        connect( twoStage, SIGNAL(twoStageChanged(int)),
                presValve, SIGNAL(diffValveChanged()));
        connect( twoStage, SIGNAL(twoStageChanged(int)),
                             SLOT(setPresExchData()));
    }

    for (int i = 0; i < exchanger.size(); ++i)
    {
        connect(exchanger.at(i), SIGNAL(loadBoxChanged()),
                                   SLOT(computeTotalLoad()));
        connect(exchanger.at(i), SIGNAL(loadBoxChanged()),
                presValve,       SIGNAL(diffValveChanged()));
        connect(exchanger.at(i), SIGNAL(loadBoxChanged()),
                                   SLOT(setPresExchData()));
    }

    for (int i = 0; i < collector.size(); ++i)
    {
        connect(exchanger.at(i), SIGNAL(loadBoxChanged()),
                                   SLOT(setExchData()));

        connect(exchanger.at(i), SIGNAL(loadBoxChanged()),
                   vessel.at(i),   SLOT(warnDataUnset()));

        connect(collector.at(i), SIGNAL(branchAdded()),
                                   SLOT(setBranchNums()));

        connect(collector.at(i), SIGNAL(branchDeleted()),
                                   SLOT(setBranchNums()));

        connect(collector.at(i), SIGNAL(lenghtChanged(int)),
                                   SLOT(setSchemeWidthSlot(int)));

        connect(collector.at(i), SIGNAL(branchDeleted()),
                                   SLOT(reduceSchemeWidthSlot()));

        connect(   vessel.at(i), SIGNAL(vesselChanged()),
                                   SLOT(setVesselData()));

    }

    for (int i = nVentExch + nHeatExch; i < exchanger.size(); ++i)
    {
        connect(exchanger.at(i), SIGNAL(loadBoxChanged()),
                                   SLOT(setSingleStageData()));

        connect(exchanger.at(i), SIGNAL(loadBoxChanged(int)),
                dhwSingle.at(i - nVentExch - nHeatExch),
                                   SLOT(setCunsumLoad(int)));
    }
}

void Scheme::removeAllItems()
{
       scene->clear();
    collector.clear();
    exchanger.clear();
    dhwSingle.clear();
       vessel.clear();
         pump.clear();

    insPoint.setX(0);
    insPoint.setY(0);
}


void Scheme::setCollectorFluid()
{
    for (int i = 0; i < collector.size(); ++i)
    {
        collector.at(i)->heatTransfer()->
                         setFluid(exchanger.at(i)->heatTransferConsum());

        connect(exchanger.at(i), SIGNAL(loadBoxChanged()),
                collector.at(i), SIGNAL(heatLoadChanged()));
    }
}

void Scheme::setBranchLoad()
{
    for (int i = 0; i < collector.size(); ++i)
    {
        collector.at(i)->setTotalLoad(*(exchanger.at(i)->heatLoad()));
    }

    for (int i = 0; i < dhwSingle.size(); ++i)
    {
        dhwSingle.at(i)->setCunsumLoad(
                    exchanger.at(nVentExch + nHeatExch + i)->heatLoad()->load()
                                        );
    }
}

void Scheme::setDiffPressData()
{
    Pressure delta(0.2, Pressure::BAR);
    Pressure dP(Pressure::BAR);

    presValve->supplyPressure()->setPressure(*leadin->pressureP01() - delta);
    presValve->returnPressure()->setPressure(*leadin->pressureP02() + delta);
    dP.setPressure(*presValve->supplyPressure() -
                   *presValve->returnPressure());
    presValve->getPressInfo()->setFlow(leadin->heatTransfer()->flow());
    presValve->getPressInfo()->setDP(dP);
    presValve->getPressInfo()->setPipeDn(leadin->pipeDn());
    presValve->getPressInfo()->setSupplyPressure(presValve->supplyPressure());
}


//=============================================================================*
// Slots

void Scheme::drawNewScheme()
{
    removeAllItems();
    configSchDlg();
    initialize();
    drawLeadin();
    drawPresV();
    drawHwsLine();
    drawTee();
    drawHeLine();
    drawCircuits();
    drawRecharge();
    connectSlots();
    setBranchNums();
}


void Scheme::computeTotalLoad()
{
    Load totalLoad(0, Load::KWT);

    for (int i = 0; i < exchanger.size(); ++i)
    {
        totalLoad += exchanger.at(i)->heatLoad();
    }
    if (isTwoStaged)
    {
        totalLoad += twoStage->heatLoadStage1();
        totalLoad += twoStage->heatLoadStage2();
    }

    leadin->heatLoad()->setLoad(totalLoad);
    leadin->setVolumeFlow();
    leadin->drawPipe();
    leadin->setTextSummary();

    setDiffPressData();
}

void Scheme::setLeadinData()
{
    if (isTwoStaged)
    {
        hwsJoint->bypass()->setFluid(leadin->heatTransfer());
        hwsJoint->control()->setFluid(leadin->heatTransfer());
    }

    for (int i = 0; i < exchanger.size(); ++i)
    {
        exchanger.at(i)->setSourceTempTxt(
                        leadin->heatTransfer()->temperature());

        exchanger.at(i)->heatTransferSource()->setFluid(
                        leadin->heatTransfer());

        exchanger.at(i)->computeFlow();
    }
    setDiffPressData();
}

void Scheme::setPresExchData()
{
    for (int i = 0; i < exchanger.size(); ++i)
    {
        exchanger.at(i)->getInfoActuValve()->supplyPressure()->
                         setPressure(presValve->consumerPressure());

        if (isTwoStaged)
        {
            hwsJoint->getReturnPressure()->setPressure(
                        presValve->returnPressure());
            hwsJoint->getJointPressure()->setPressure(
                        *twoStage->getStage1Pressure());

            exchanger.at(i)->getInfoActuValve()->returnPressure()->setPressure(
                        hwsJoint->getJointPressure());

            hwsJoint->updatePressureData();
        } else {
            exchanger.at(i)->getInfoActuValve()->returnPressure()->setPressure(
                        presValve->returnPressure());
        }
    }

    if (isTwoStaged)
    {
        twoStage->getSupplyPressure()->setPressure(
                  presValve->consumerPressure());
        twoStage->getReturnPressure()->setPressure(
                  presValve->returnPressure());
    }
}

void Scheme::setExchData()
{
    for (int i = 0; i < collector.size(); ++i)
    {
        vessel.at(i)->setHeatLoad(exchanger.at(i)->heatLoad());
        vessel.at(i)->setExchType(exchanger.at(i)->exchType());
        vessel.at(i)->heatTransfer()->
                      setFluid(exchanger.at(i)->heatTransferConsum());

        pump.at(i)->infoPump()->flow()->setFlow(
                    exchanger.at(i)->heatTransferConsum()->flow());
        pump.at(i)->updatePumpText();
    }

    setCollectorFluid();
    setBranchLoad();
    setDiffPressData();
}

void Scheme::setTwoStageData()
{
    computeTotalLoad();
    const qreal circFlowPercentage = 0.4;

    Load twoStageLoad(0, Load::KWT);
    twoStageLoad += twoStage->heatLoadStage1();
    twoStageLoad += twoStage->heatLoadStage2();

    dhw->hotColdWater()->agent()->setAgent(Agent::WATER, Agent::G0);
    dhw->hotColdWater()->temperature()->setTemperature(
                twoStage->heatTransferConsumSt2()->temperature()->supplyT(),
                twoStage->heatTransferConsumSt1()->temperature()->returnT());
    dhw->hotColdWater()->computeVolumeFlow(twoStageLoad);

    dhw->circWater()->setFluid(dhw->hotColdWater());
    dhw->circWater()->setFlow(dhw->hotColdWater()->
                              flow()->flow() * circFlowPercentage);

    dhw->infoPump()->flow()->setFlow(dhw->circWater()->flow());
    dhw->updatePumpText();
}

void Scheme::setSingleStageData()
{
    computeTotalLoad();
    int dhwIndex = nVentExch + nHeatExch;
    const qreal circFlowPercentage = 0.4;

    for (int i = 0; i < dhwSingle.size(); ++i)
    {
        dhwSingle.at(i)->hotColdWater()->agent()->
                                         setAgent(Agent::WATER, Agent::G0);

        dhwSingle.at(i)->hotColdWater()->temperature()->setTemperature(
                         exchanger.at(i + dhwIndex)->
                         heatTransferConsum()->temperature());

        dhwSingle.at(i)->hotColdWater()->computeVolumeFlow(
                         exchanger.at(i + dhwIndex)->heatLoad());

        dhwSingle.at(i)->circWater()->setFluid(
                         dhwSingle.at(i)->hotColdWater());

        dhwSingle.at(i)->circWater()->setFlow(dhwSingle.at(i)->
                         hotColdWater()->flow()->flow() * circFlowPercentage);

        dhwSingle.at(i)->infoPump()->flow()->setFlow(dhwSingle.at(i)->
                         circWater()->flow());
        dhwSingle.at(i)->updatePumpText();
    }
}

void Scheme::setVesselData()
{
    const int hoursOfFillingSystem = 4;
    bool  isFeedPipeNeeded = false;
    Pressure feedPressure;
    Pressure vesselP0;
    Pressure leadinP0;
    qreal feedFlow = 0;

    for (int i = 0; i < vessel.size(); ++i)
    {
        vesselP0.setPressure(vessel.at(i)->getP0Press());
        leadinP0.setPressure(leadin->pressureP02()->pressure());

        collector.at(i)->drawStaticPr(QString::number(
                                      vesselP0.pressure(), 'f', 2));

        collector.at(i)->drawRechFlow(QString::number(
                vessel.at(i)->getSysVolume() / hoursOfFillingSystem, 'f', 2));
        collector.at(i)->setFeedFlow(
                vessel.at(i)->getSysVolume() / hoursOfFillingSystem );

        feedFlow += vessel.at(i)->getSysVolume() / hoursOfFillingSystem;

        isFeedPipeNeeded = isFeedPipeNeeded || (vesselP0 > leadinP0);

        if ( (vesselP0 > leadinP0) && (feedPressure < (vesselP0 - leadinP0)) )
        {
            feedPressure = vesselP0 - leadinP0;
        }
    }

    rechMain->needPump(isFeedPipeNeeded);
    rechMain->infoPump()->setPressure(feedPressure.convertTo(Pressure::MWC));
    rechMain->infoPump()->flow()->setFlow(feedFlow);
    rechMain->updatePumpText();
    rechMain->pipe()->fitPipe(feedFlow);
    rechMain->setPipe(rechMain->pipe()->pipeName());
}

void Scheme::setBranchNums()
{
    int num = 0;

    for (int i = 0; i < collector.size(); ++i)
    {
        pump.at(i)->setTempText(QString::number(num + 1));

        for (int j = 0; j < collector.at(i)->getBranchSize(); ++j)
        {
            ++num;
            collector.at(i)->drawBranchMark(j, QString::number(num));
        }
    }
}
