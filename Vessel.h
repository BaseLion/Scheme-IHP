#pragma once;

#include <QObject>

#include "Boxes/BoxHeatExch.h"


class Pipe;

class Vessel : public QObject
{
    Q_OBJECT

public:
    Vessel();

    QString getVesselName();
    int     getNominalVolume();
    int     getNominalPress() const;
    int     getDiametr() const;

    void setVesselData(const qreal, QString);

private:
    void findVesselData();
    void splitVesselType(const QString);

    QString vesselCompany;
    QString vesselName;
    QString vesselType;
    qreal   expansionVolume;
    int     nominalPress;
    int     diametr;
    int     nominalVolume;
    int     numVessel;
};
