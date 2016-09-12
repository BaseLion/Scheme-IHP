#pragma once;

#include <QObject>

#include "Hydraulics/Fluid.h"

class BoxCollector;

class Pipe : public QObject
{
    Q_OBJECT

public:
    enum PipeType {
        STEEL_CLEAN,
        STEEL_RUSTY,
        PLASTIC
    };
    enum SpecificLoss {
        PA_PER_M_80,
        PA_PER_M_100,
        PA_PER_M_130,
        PA_PER_M_150,
    };

    Pipe(PipeType, SpecificLoss, Fluid* fluid = 0);

    QString pipeName();
    qreal dn() const;    // Nominal  Diameter
    qreal dext() const;  // External Diameter
    qreal din() const;   // Internal Diameter
    qreal s() const;     // Wall thickness
    qreal CSA() const;   // Cross Section Area
    qreal velocity() const;
    qreal specPressure() const;

    void fitPipe(Flow);
    void fitPipe(BoxCollector*);

private slots:
    void fitPipe();

private:
    Fluid* m_fluid;

    const int   mmToM   = 1000;       // millimeters to meters
    const int   hToSec  = 3600;       // Hours to Seconds
    const qreal PI      = 3.14159;    // PI
    const qreal speedCriteria = 1.55; // m/sec
    qreal pressureLossCriteria;       // Pa/m
    qreal rough;          // Kэ = Δ / d, mm (roughness coefficient)

    int   m_dn;           // DN, mm
    qreal m_dext;         // D exterior, mm
    qreal m_din;          // D interior, mm
    qreal m_s;            // S, mm - wall thickness
    qreal m_CSA;          // Cross Section Area

    qreal m_speed;        // V,  m/sec (Velocity)
    qreal m_lambda;       // λ,  hydraulic
    qreal m_reinolds;     // Re, Reinolds criteria
    qreal m_specPressure; // Pa/m, specific pressure
    qreal friction;       // Δ = Re * K / Din

    qreal m_dynamPressure;
    qreal m_localPressLoss;
    qreal m_routeLenght;
    qreal m_totalPressureLoss;


    QString lambdaStr;
    QString pipeNameStr;
    QString gostStr;

    bool isLaminar;
    bool isTurbulence;
    bool isSmoothPipe;
    bool isRoughPipe;

    void initialize(PipeType, SpecificLoss, Fluid*);
    void connectSlots();
    void computePipe();
    void computeFriction();
    void conditionTest();
    void computeLambda();
    void computeDynamic();
    void fillPipeData();

    Pipe(const Pipe&);
    Pipe& operator =(const Pipe&);    
};
