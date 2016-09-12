#pragma once;

#include <QGraphicsView>
#include <QFrame>

class QToolButton;
class QSlider;
class QLabel;
class View;

class MainView : public QGraphicsView
{
    Q_OBJECT

public:
    MainView(View* v) : QGraphicsView(), view(v) { }

protected:
#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent*) Q_DECL_OVERRIDE;
#endif

private:
    View* view;

};

class View : public QFrame
{
    Q_OBJECT

public:
    explicit View(const QString &name, QWidget* parent = 0);

    MainView* view() const;

public slots:
    void zoomIn (int level = 1);
    void zoomOut(int level = 1);

private slots:
    void resetView();
    void setupMatrix();
    void setResetButtonEnabled();

private:
    QToolButton* resetButton;
    MainView*    mainView;
    QSlider*     zoomSlider;
    QLabel*      label1;
    QLabel*      label2;
};
