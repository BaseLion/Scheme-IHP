#include "MainView.h"

#include <QtWidgets>
#include <QToolButton>
#include <qmath.h>

#ifndef QT_NO_WHEELEVENT
void MainView::wheelEvent(QWheelEvent* event)
{
    if(event->modifiers() & Qt::ControlModifier) {
        if(event->delta() > 0)
            view->zoomIn(3);
        else
            view->zoomOut(3);
        event->accept();
    } else {
        MainView::wheelEvent(event);
    }
}
#endif

View::View(const QString& name, QWidget* parent) : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);

    mainView = new MainView(this);
    mainView->setRenderHint(QPainter::Antialiasing, false);
    mainView->setDragMode(QGraphicsView::NoDrag);
    mainView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
    mainView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    mainView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size,size);

    QToolButton* zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(21);
    zoomInIcon->setAutoRepeatDelay(0);
    zoomInIcon->setIcon(QPixmap(":/images/zoom_in.png"));
    zoomInIcon->setIconSize(iconSize);

    QToolButton* zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(21);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/images/zoom_out.png"));
    zoomOutIcon->setIconSize(iconSize);

    zoomSlider = new QSlider;
    zoomSlider->setMaximum(0);
    zoomSlider->setMaximum(50);
    zoomSlider->setValue(25);
    zoomSlider->setFixedWidth(500);
    zoomSlider->setTickPosition(QSlider::TicksAbove);

    resetButton = new QToolButton;
    resetButton->setText(tr("0"));
    resetButton->setEnabled(false);

    QHBoxLayout* labelLayout = new QHBoxLayout;
    label1 = new QLabel(name);
    label2 = new QLabel(tr("Pointer Mode"));

    labelLayout->addWidget(label1);
    labelLayout->addStretch();
    labelLayout->addWidget(label2);
    labelLayout->addStretch();
    labelLayout->addWidget(zoomInIcon);
    labelLayout->addWidget(zoomSlider);
    labelLayout->addWidget(zoomOutIcon);
    labelLayout->addWidget(resetButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(labelLayout);
    mainLayout->addWidget(mainView);
    setLayout(mainLayout);

    connect(resetButton, SIGNAL(clicked()),      SLOT(resetView()) );
    connect( zoomSlider, SIGNAL(valueChanged()), SLOT(setupMatrix()) );

    connect(mainView->verticalScrollBar(), SIGNAL(valueChanged(int)),
                                             SLOT(setResetButtonEnabled()) );

    connect(mainView->horizontalScrollBar(), SIGNAL(valueChanged(int)),
                                             SLOT(setResetButtonEnabled()) );

    connect(zoomInIcon, SIGNAL(clicked()), SLOT(zoomIn()) );
    connect(zoomOutIcon, SIGNAL(clicked()), SLOT(zoomOut()) );

    setupMatrix();
}

MainView* View::view() const
{
    return static_cast<MainView*>(mainView);
}

void View::resetView()
{
    zoomSlider->setValue(25);
    setupMatrix();
    mainView->ensureVisible(QRectF(0, 0, 0, 0));
    resetButton->setEnabled(false);
}

void View::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

void View::setupMatrix()
{
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 25) / qreal(5));

    QMatrix matrix;
    matrix.scale(scale, scale);

    mainView->setMatrix(matrix);
    setResetButtonEnabled();
}

void View::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

void View::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}
