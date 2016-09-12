#pragma once;

#include "Ui_Boxes/Ui_BoxHeatExch.h"
#include "Ui_Boxes/Ui_Scheme.h"
#include "Scheme.h"

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>

class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
   ~MainWindow();

private:
    Scheme*   scheme;

    QAction*  exitAction;
    QAction*  aboutQtAction;
    QAction*  newScheme;
    QMenu*    fileMenu;
    QToolBar* fileToolBar;

    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void newFile();
};
