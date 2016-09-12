#include <QSqlRecord>
#include <QSqlQuery>
#include <QDebug>

#include "Vessel.h"

#include "Hydraulics/Fluid.h"
#include "Pipe.h"

Vessel::Vessel(): expansionVolume(0)
{

}


QString Vessel::getVesselName()
{
    findVesselData();

    if (numVessel == 1)
    {
        return vesselName;
    } else {
        return QString(QString::number(numVessel) + " x " + vesselName);
    }
}

int Vessel::getNominalVolume()
{
    findVesselData();
    return nominalVolume;
}

int Vessel::getNominalPress() const
{
    return nominalPress;
}

int Vessel::getDiametr() const
{
    return diametr;
}


void Vessel::setVesselData(const qreal volume, const QString type)
{
    splitVesselType(type);
    expansionVolume = volume * 1000;
}

void Vessel::findVesselData()
{
    QSqlRecord record;
     QSqlQuery query;
       QString queryTxt;

    query.setForwardOnly(true);

    numVessel = 0;

    do
    {
        queryTxt = "SELECT vessel_name, nominal_volume, connection, Pn "
                     "FROM Vessels "
                    "WHERE (Company = '%1')"
                      "AND (Type = '%2') "
                      "AND (Pn = %3) "
                      "AND (nominal_volume > %4);";
        ++numVessel;

        query.exec(queryTxt.arg(vesselCompany)
                           .arg(vesselType)
                           .arg(nominalPress)
                           .arg(expansionVolume / numVessel));

        record = query.record();
        query.first();
    }
    while (!query.isValid());

    nominalVolume = query.value(record.indexOf("nominal_volume")).toInt();
       vesselName = query.value(record.indexOf("vessel_name")).toString();
          diametr = query.value(record.indexOf("connection")).toInt();
     nominalPress = query.value(record.indexOf("Pn")).toInt();
}

void Vessel::splitVesselType(const QString type)
{
    QStringList list = type.split(QRegExp("\\s+"), QString::SkipEmptyParts);
    if (!list.isEmpty())
    {
        QString press = list.at(2);

        vesselCompany = list.at(0);
           vesselType = list.at(1);
         nominalPress = press.toInt();
    }
}
