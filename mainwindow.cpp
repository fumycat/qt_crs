#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() : tabWidget(new QTabWidget) // , ui(new Ui::MainWindow)
{
    setCentralWidget(tabWidget);

    table = new QTableWidget;
    table -> setColumnCount(3);
    table -> setRowCount(5);
    table -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "Дата рождения", "Телефон"});
    table -> setItem(1, 1, new QTableWidgetItem("Test"));

    tabWidget->insertTab(0, table, "KEKW ILI CHTO");

    createDockWindow();
    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createDockWindow()
{
    QDockWidget *dock = new QDockWidget("Группы", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    groupsList = new QListWidget(dock);
    groupsList->addItems(QStringList()
            << "ИП-711"
            << "ИП-712"
            << "ИП-713"
            << "ИП-714");
    dock->setWidget(groupsList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
//    viewMenu->addAction(dock->toggleViewAction());

    connect(groupsList, &QListWidget::itemSelectionChanged, this, &MainWindow::groupSelected);
}

void MainWindow::createActions()
{
    // connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));

    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open file"));
}

void MainWindow::groupSelected()
{
    QListWidgetItem *item = this->groupsList->selectedItems()[0];
    QString g = item -> text();
    if (g == "ИП-711") {
        table -> setRowCount(4);
        table -> setColumnCount(4);
        return;
    }
    table -> setRowCount(5);
    table -> setColumnCount(3);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
//    fileMenu->addAction(saveAct);
//    fileMenu->addAction(printAct);
//    fileMenu->addSeparator();
//    fileMenu->addAction(exitAct);
}
