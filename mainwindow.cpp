#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() : tabWidget(new QTabWidget) // , ui(new Ui::MainWindow)
{
    setCentralWidget(tabWidget);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\Fumycat\\Desktop\\sqlite3.db");
    db.open();

    QSqlQuery query;
    query.exec("SELECT _id, fio, phone, email FROM basic");

    while (query.next())
    {
        QString _id = query.value(0).toString();
        QString fio = query.value(1).toString();
        QString phone = query.value(2).toString();
        QString email = query.value(3).toString();
        qDebug() << _id << fio << phone << email;
    }


    tlist = new QList<QTableWidget*>;
    nlist = new QList<QString>;

    QTableWidget *table;
    table = new QTableWidget;
    table -> setColumnCount(3);
    table -> setRowCount(5);
    table -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "Дата рождения", "Телефон"});
    table -> setItem(1, 1, new QTableWidgetItem("Test"));
    tlist->append(table);
    nlist->append("tab1");

    QTableWidget *table2;
    table2 = new QTableWidget;
    table2 -> setColumnCount(6);
    table2 -> setRowCount(6);
    table2 -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "Дата рождения", "Телефон", "KEKW", "KEKW2", "huh"});
    table2 -> setItem(3, 0, new QTableWidgetItem("Test123"));
    tlist->append(table2);
    nlist->append("tab2");


    for (int i = 0; i < nlist->size(); i++) {
        tabWidget->insertTab(i, tlist->at(i), nlist->at(i));
        connect(tlist->at(i), &QTableWidget::cellChanged, this, &MainWindow::mcChanged);
    }

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
}

void MainWindow::mcChanged(int x, int y){

    qDebug() << x;
    qDebug() << y;
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


