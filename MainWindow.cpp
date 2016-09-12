#include "MainWindow.h"

#include <QApplication>
#include <QtWidgets>
#include <QAction>
#include <QMenu>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon(":/images/heating_room.png"));

    scheme = new Scheme(this);

    createActions();
    createMenus();
    createStatusBar();
    createToolBars();

    QSize winSize(QApplication::desktop()
                  ->availableGeometry(QApplication::desktop()
                                      ->primaryScreen()).width(),
                  QApplication::desktop()
                  ->availableGeometry(QApplication::desktop()
                                      ->primaryScreen()).height());

    resize(winSize);
    newFile();
}

MainWindow::~MainWindow()
{
    delete scheme;
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("В&ыход"), this);
    exitAction->setIcon(QIcon(":/images/exit.png"));
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Выйти из программы"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutQtAction = new QAction(tr("&Про Qt"), this);
    aboutQtAction->setIcon(QIcon(":/images/about.png"));
    aboutQtAction->setStatusTip(tr("О библиотеке Qt"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    newScheme = new QAction(tr("&Новая схема"), this);
    newScheme->setIcon(QIcon(":/images/new.png"));
    newScheme->setStatusTip(tr("Создать новую конфигурацию схемы"));
    connect(newScheme, SIGNAL(triggered()), scheme, SLOT(drawNewScheme()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newScheme);
    fileMenu->addAction(exitAction);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newScheme);
    fileToolBar->addAction(exitAction);
    fileToolBar->addAction(aboutQtAction);
}

void MainWindow::createStatusBar()
{
    statusBar()->autoFillBackground();
}

void MainWindow::newFile()
{
    setCentralWidget(scheme);
}
