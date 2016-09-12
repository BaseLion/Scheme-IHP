#-------------------------------------------------
#
# Project created by QtCreator 2016-07-26T20:15:27
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Scheme-IHP
TEMPLATE = app

SOURCES +=\
    Boxes/BoxCollector.cpp \
    Boxes/BoxCreator.cpp \
    Boxes/BoxDhw.cpp \
    Boxes/BoxHeatExch.cpp \
    Boxes/BoxHwsJoint.cpp \
    Boxes/BoxHwsLine.cpp \
    Boxes/BoxHwsPiping.cpp \
    Boxes/BoxLeadin.cpp \
    Boxes/BoxPresValve.cpp \
    Boxes/BoxPump.cpp \
    Boxes/BoxRechargeHor.cpp \
    Boxes/BoxRechargeVer.cpp \
    Boxes/BoxRechMain.cpp \
    Boxes/BoxTee.cpp \
    Boxes/BoxTeeLine.cpp \
    Boxes/BoxTwoStage.cpp \
    Boxes/BoxVessel.cpp \
    Dialoges/AbstractDialog.cpp \
    Dialoges/DialogCustomerBox.cpp \
    Dialoges/DialogExchanger.cpp \
    Dialoges/DialogLeadin.cpp \
    Dialoges/DialogPump.cpp \
    Dialoges/DialogSchemeCfg.cpp \
    Dialoges/DialogTwoStage.cpp \
    Dialoges/DialogVessel.cpp \
    Hydraulics/Agent.cpp \
    Hydraulics/Fluid.cpp \
    Hydraulics/Temperature.cpp \
    Hydraulics/Thermophysics.cpp \
    Info/InfoPump.cpp \
    Items/ItemAbstractValve.cpp \
    Items/ItemActuValve.cpp \
    Items/ItemArrow.cpp \
    Items/ItemCheckValve.cpp \
    Items/ItemCustomerBox.cpp \
    Items/ItemDot.cpp \
    Items/ItemDrainValve.cpp \
    Items/ItemHeatExch.cpp \
    Items/ItemLeadinBox.cpp \
    Items/ItemPresoStat.cpp \
    Items/ItemPresValve.cpp \
    Items/ItemPump.cpp \
    Items/ItemSafeValve.cpp \
    Items/ItemSolValve.cpp \
    Items/ItemStopValve.cpp \
    Items/ItemVessel.cpp \
    Items/ItemZoneValve.cpp \
    Ui_Boxes/Ui_AbstractBox.cpp \
    Ui_Boxes/Ui_BoxCollector.cpp \
    Ui_Boxes/Ui_BoxDhw.cpp \
    Ui_Boxes/Ui_BoxHeatExch.cpp \
    Ui_Boxes/Ui_BoxHwsJoint.cpp \
    Ui_Boxes/Ui_BoxLeadin.cpp \
    Ui_Boxes/Ui_BoxPresValve.cpp \
    Ui_Boxes/Ui_BoxPump.cpp \
    Ui_Boxes/Ui_BoxRechMain.cpp \
    Ui_Boxes/Ui_BoxTee.cpp \
    Ui_Boxes/Ui_BoxTwoStage.cpp \
    Ui_Boxes/Ui_BoxVessel.cpp \
    Ui_Boxes/Ui_Scheme.cpp \
    lagrange.cpp \
    Main.cpp \
    MainView.cpp \
    MainWindow.cpp \
    Pipe.cpp \
    Scheme.cpp \
    Vessel.cpp \
    Boxes/BoxBranchItem.cpp \
    Ui_Boxes/Ui_BoxBranchItem.cpp \
    Units/Load.cpp \
    Units/Flow.cpp \
    Units/Pressure.cpp \
    Dialoges/DialogDiffPressure.cpp \
    Info/InfoPressValve.cpp \
    Units/Kvs.cpp \
    Info/InfoActuatorValve.cpp \
    Dialoges/DialogActuatorValve.cpp


HEADERS  += \
    Boxes/BoxCollector.h \
    Boxes/BoxCreator.h \
    Boxes/BoxDhw.h \
    Boxes/BoxHeatExch.h \
    Boxes/BoxHwsJoint.h \
    Boxes/BoxHwsLine.h \
    Boxes/BoxHwsPiping.h \
    Boxes/BoxLeadin.h \
    Boxes/BoxPresValve.h \
    Boxes/BoxPump.h \
    Boxes/BoxRechargeHor.h \
    Boxes/BoxRechargeVer.h \
    Boxes/BoxRechMain.h \
    Boxes/BoxTee.h \
    Boxes/BoxTeeLine.h \
    Boxes/BoxTwoStage.h \
    Boxes/BoxVessel.h \
    Dialoges/AbstractDialog.h \
    Dialoges/DialogCustomerBox.h \
    Dialoges/DialogExchanger.h \
    Dialoges/DialogLeadin.h \
    Dialoges/DialogPump.h \
    Dialoges/DialogSchemeCfg.h \
    Dialoges/DialogTwoStage.h \
    Dialoges/DialogVessel.h \
    Hydraulics/Agent.h \
    Hydraulics/Fluid.h \
    Hydraulics/Temperature.h \
    Hydraulics/Thermophysics.h \
    Info/InfoPump.h \
    Items/ItemAbstractValve.h \
    Items/ItemActuValve.h \
    Items/ItemArrow.h \
    Items/ItemCheckValve.h \
    Items/ItemCustomerBox.h \
    Items/ItemDot.h \
    Items/ItemDrainValve.h \
    Items/ItemHeatExch.h \
    Items/ItemLeadinBox.h \
    Items/ItemPresoStat.h \
    Items/ItemPresValve.h \
    Items/ItemPump.h \
    Items/ItemSafeValve.h \
    Items/ItemSolValve.h \
    Items/ItemStopValve.h \
    Items/ItemVessel.h \
    Items/ItemZoneValve.h \
    Ui_Boxes/Ui_AbstractBox.h \
    Ui_Boxes/Ui_BoxCollector.h \
    Ui_Boxes/Ui_BoxDhw.h \
    Ui_Boxes/Ui_BoxHeatExch.h \
    Ui_Boxes/Ui_BoxHwsJoint.h \
    Ui_Boxes/Ui_BoxLeadin.h \
    Ui_Boxes/Ui_BoxPresValve.h \
    Ui_Boxes/Ui_BoxPump.h \
    Ui_Boxes/Ui_BoxRechMain.h \
    Ui_Boxes/Ui_BoxTee.h \
    Ui_Boxes/Ui_BoxTwoStage.h \
    Ui_Boxes/Ui_BoxVessel.h \
    Ui_Boxes/Ui_Scheme.h \
    lagrange.h \
    MainView.h \
    MainWindow.h \
    Pipe.h \
    Scheme.h \
    Vessel.h \
    Boxes/BoxBranchItem.h \
    Ui_Boxes/Ui_BoxBranchItem.h \
    Units/Load.h \
    Units/Flow.h \
    Units/Pressure.h \
    Dialoges/DialogDiffPressure.h \
    Units/Kvs.h \
    Info/InfoPressValve.h \
    Info/InfoActuatorValve.h \
    Dialoges/DialogActuatorValve.h


RESOURCES += \
    schemeIhp.qrc

CONFIG += c++11
