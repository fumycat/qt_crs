#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow() : tabWidget(new QTabWidget) // , ui(new Ui::MainWindow)
{
    setCentralWidget(tabWidget);

    currentGroup = -2;

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\Fumycat\\Desktop\\sqlite3.db");
    db.open();


    QSqlQuery query;
    query.exec("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
    groups = new QList<QString>;
    while (query.next())
    {
        QString tmp = query.value(0).toString();
        if (tmp != "basic") groups->append(tmp);
    }

    for (int i = 0; i < groups->size(); ++i)
    {
        // qDebug() << QString("SELECT * FROM \"%1\";").arg(groups->at(i));
        rd.append(QList<r*>());
        QSqlQuery q;
        q.exec(QString("SELECT * FROM \"%1\";").arg(groups->at(i)));

        while (q.next())
        {
            struct r *t = new r;
            t->name = q.value(1).toString();
            t->phone = q.value(2).toString();
            t->email = q.value(3).toString();
            t->pname = q.value(4).toString();
            t->pphone = q.value(5).toString();
            t->sscore = q.value(6).toString();
            t->perf = q.value(7).toString();
            rd[i].append(t);
        }

    }
    // qDebug() << rd[0].at(0)->name;
    /*
    query.exec("SELECT _id, fio, phone, email FROM basic");

    while (query.next())
    {
        QString _id = query.value(0).toString();
        QString fio = query.value(1).toString();
        QString phone = query.value(2).toString();
        QString email = query.value(3).toString();
        qDebug() << _id << fio << phone << email;
    }
    */

    tlist = new QList<QTableWidget*>;
    nlist = new QList<QString>;

    QTableWidget *table;
    table = new QTableWidget;
    table -> setColumnCount(4);
    table -> setRowCount(0);
    table -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "Дата рождения", "Телефон", "Баллы ЕГЭ"});
    tlist->append(table);
    nlist->append("Базовая информация");

    QTableWidget *table2;
    table2 = new QTableWidget;
    table2 -> setColumnCount(3);
    table2 -> setRowCount(0);
    table2 -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "ФИО Родителя", "Телефон родителя"});
    tlist->append(table2);
    nlist->append("О Родителях");

    QTableWidget *table3;
    table3 = new QTableWidget;
    table3 -> setColumnCount(3);
    table3 -> setRowCount(0);
    table3 -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "ФИО Родителя", "Телефон родителя"});
    tlist->append(table3);
    nlist->append("Успеваемость");

    for (int i = 0; i < nlist->size(); i++) {
        tabWidget->insertTab(i, tlist->at(i), nlist->at(i));
        connect(tlist->at(i), &QTableWidget::cellChanged, this, &MainWindow::mcChanged);
    }

    if (groups->size() != 0) currentGroup = -1;

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
    for (int i = 0; i < groups->size(); ++i) groupsList->addItem(groups->at(i));
    dock->setWidget(groupsList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
//    viewMenu->addAction(dock->toggleViewAction());

    connect(groupsList, &QListWidget::itemSelectionChanged, this, &MainWindow::groupSelected);
}

// connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
void MainWindow::createActions()
{
    // Actions
    newAct = new QAction(this->style()->standardIcon(QStyle::SP_FileIcon), tr("Новая база данных..."), this);
    newAct->setShortcuts(QKeySequence::New);

    openAct = new QAction(this->style()->standardIcon(QStyle::SP_DirOpenIcon) ,tr("Открыть базу данных..."), this);
    openAct->setShortcuts(QKeySequence::Open);

    saveAct = new QAction(this->style()->standardIcon(QStyle::SP_DialogSaveButton) ,tr("Сохранить изменения"), this);
    saveAct->setShortcut(QKeySequence::Save);

    addStudent = new QAction(this->style()->standardIcon(QStyle::SP_DialogYesButton) ,tr("Добавить студента"), this);
    addStudent->setShortcut(QKeySequence::AddTab);
    connect(addStudent, &QAction::triggered, this, &MainWindow::addStudentSlot);

    addGroup = new QAction(this->style()->standardIcon(QStyle::SP_FileDialogNewFolder) ,tr("Добавить группу"), this);
    addGroup->setShortcut(QKeySequence::Italic);


    // Toolbar
    QList<QAction*> toolbarActionList = {newAct, openAct, saveAct, addGroup, addStudent};
    QToolBar *fileToolBar = addToolBar(tr("File"));
    fileToolBar->addActions(toolbarActionList);
}

void MainWindow::groupSelected()
{

    QListWidgetItem *item = this->groupsList->selectedItems()[0];

    for (int i = 0; i < groups->size(); ++i){
        if (groups->at(i) == item->text()) {
            updateData(i);
            currentGroup = i;
        }
    }
}

void MainWindow::addStudentSlot()
{
    switch (currentGroup){
        case -2:
            {
                QMessageBox::warning(this, tr("Ошибка"),
                               tr("Невозможно добавить студента, так как нет ни одной группы.\n"
                                  "Сначала создайте группу."));
                return;
            }
        case -1:
            {
                QMessageBox::warning(this, tr("Ошибка"),
                                       tr("Невозможно добавить студента, так как не выбрана ни одна группа.\n"
                                          "Сначала выберите группу в боковом меню."));
                return;
            }
        default:
            break;
    }
    if (currentGroup == -1){

    }
    bool ok;
    QString text = QInputDialog::getText(this, tr("Добавление студента"), tr("Введите ФИО студента:"),
                                         QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        struct r *t = new r;
        t->name = text;
        t->phone = "";
        t->email = "";
        t->pname = "";
        t->pphone = "";
        t->sscore = "";
        t->perf = "";
        rd[currentGroup].append(t);
        updateData(currentGroup);
    }
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
    fileMenu->addSeparator();
    fileMenu->addAction(addStudent);
//    fileMenu->addAction(saveAct);
//    fileMenu->addAction(printAct);
//    fileMenu->addSeparator();
//    fileMenu->addAction(exitAct);
}

void MainWindow::updateData(int group)
{
    tlist->at(0)->setRowCount(rd[group].size());
    for (int i = 0; i < rd[group].size(); ++i)
    {
        tlist->at(0)->setItem(i, 0, new QTableWidgetItem(rd[group].at(i)->name));
        tlist->at(0)->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tlist->at(0)->setItem(i, 1, new QTableWidgetItem(rd[group].at(i)->phone));
        tlist->at(0)->setItem(i, 2, new QTableWidgetItem(rd[group].at(i)->email));
        tlist->at(0)->setItem(i, 3, new QTableWidgetItem(rd[group].at(i)->sscore));
    }
    tlist->at(0)->resizeColumnsToContents();

    tlist->at(1)->setRowCount(rd[group].size());
    for (int i = 0; i < rd[group].size(); ++i)
    {
        tlist->at(1)->setItem(i, 0, new QTableWidgetItem(rd[group].at(i)->name));
        tlist->at(1)->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tlist->at(1)->setItem(i, 1, new QTableWidgetItem(rd[group].at(i)->pname));
        tlist->at(1)->setItem(i, 2, new QTableWidgetItem(rd[group].at(i)->pphone));
    }
    tlist->at(1)->resizeColumnsToContents();

}

