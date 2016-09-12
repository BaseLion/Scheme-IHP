#pragma once;

#include <QSqlDatabase>

#include "Ui_Boxes/Ui_Scheme.h"

class Pipe;

class Scheme : public Ui_Scheme
{
    Q_OBJECT

public:
    Scheme(QWidget* parent = 0);

private:
    static QSqlDatabase dataBase;
    static bool createDbConnection();
    void connectSlots();
    void removeAllItems();
    void setCollectorFluid();
    void setBranchLoad();
    void setDiffPressData();
//    void setActuValveData();

private slots:
    void drawNewScheme();

    void computeTotalLoad();
    void setLeadinData();
    void setPresExchData();
    void setExchData();
    void setTwoStageData();
    void setSingleStageData();
    void setVesselData();
    void setBranchNums();
};
