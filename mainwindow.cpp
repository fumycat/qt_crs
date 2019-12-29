#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "configscore.h"

MainWindow::MainWindow() : tabWidget(new QTabWidget) // , ui(new Ui::MainWindow)
{
    setCentralWidget(tabWidget);
    setWindowTitle("Журнал");
    db = QSqlDatabase::addDatabase("QSQLITE");
    cani = true; // sync
    changed = false;
    currentGroup = -2;

    // QDockWidget
    dock = new QDockWidget("Группы", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    groupsList = new QListWidget(dock);
    dock->setWidget(groupsList);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    connect(groupsList, &QListWidget::itemSelectionChanged, this, &MainWindow::groupSelected);

    createTabs();
    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{
    db.close();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if (changed)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти?\n"
                                            "Все измененные данные будут утерены.", QMessageBox::Yes|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
        {
            e->accept();
        } else {
            e->ignore();
        }
    }

}

void MainWindow::createTabs()
{
    tlist = new QList<QTableWidget*>;
    QStringList nlist = {"Базовая информация", "О Родителях", "Успеваемость"};

    QTableWidget *table;
    table = new QTableWidget;
    table -> setColumnCount(5);
    table -> setRowCount(0);
    table -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "Дата рождения", "Телефон", "E-Mail", "Баллы ЕГЭ"});
    tlist->append(table);

    QTableWidget *table2;
    table2 = new QTableWidget;
    table2 -> setColumnCount(3);
    table2 -> setRowCount(0);
    table2 -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "ФИО Родителя", "Телефон родителя"});
    tlist->append(table2);

    QTableWidget *table3;
    table3 = new QTableWidget;
    table3 -> setColumnCount(0);
    table3 -> setRowCount(0);
    table3 -> setHorizontalHeaderLabels(QList<QString> {"ФИО", "ФИО Родителя", "Телефон родителя"});
    tlist->append(table3);

    for (int i = 0; i < nlist.size(); i++) {
        tabWidget->insertTab(i, tlist->at(i), nlist.at(i));
        connect(tlist->at(i), &QTableWidget::cellChanged, this, &MainWindow::mcChanged);
    }
}

void MainWindow::createDockWindow()
{
    // viewMenu->addAction(dock->toggleViewAction());
}

void MainWindow::newFileSlot()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Создание новой базы данных"),
                               "new.db",
                               tr("SQLite3 database files (*.db)"));
    if (fileName == "") return;
    setWindowTitle("Журнал - " + fileName);
    if ( db.isOpen() ) db.close();
    db.setDatabaseName(fileName);
    db.open();
    QSqlQuery query;
    query.exec("CREATE TABLE 'subs' ('plain' TEXT)");
    saveAct->setEnabled(true);
    addGroup->setEnabled(true);
    addStudent->setEnabled(true);
    configScore->setEnabled(true);
}

void MainWindow::openFileSlot()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open database"), "", tr("SQLite3 database files (*.db);;SQLite3 database files(*)"));
    if (fileName == "") return;
    setWindowTitle("Журнал - " + fileName);
    if ( db.isOpen() ) db.close();
    db.setDatabaseName(fileName);
    db.open();

    QSqlQuery query;
    query.exec("SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
    groups.clear();

    while (query.next())
    {
        QString tmp = query.value(0).toString();
        if (tmp != "subs" && tmp != "sqlite_sequence") groups.append(tmp);
    }
    if (groups.size() != 0) currentGroup = -1;

    groupsList->disconnect();
    delete groupsList;
    groupsList = new QListWidget(dock);
    groupsList->addItems(groups);
    dock->setWidget(groupsList);
    connect(groupsList, &QListWidget::itemSelectionChanged, this, &MainWindow::groupSelected);

    rd.clear();
    for (int i = 0; i < groups.size(); i++)
    {
        rd.append(QList<r*>());
        QSqlQuery q;
        q.exec(QString("SELECT * FROM \"%1\";").arg(groups.at(i)));
        while (q.next())
        {
            struct r *t = new r;
            t->name = q.value(0).toString();
            t->phone = q.value(1).toString();
            t->email = q.value(2).toString();
            t->pname = q.value(3).toString();
            t->pphone = q.value(4).toString();
            t->sscore = q.value(5).toString();
            t->perf = q.value(6).toString();
            t->bdate = q.value(7).toString();
            rd[i].append(t);
        }
    }
    saveAct->setEnabled(true);
    addGroup->setEnabled(true);
    addStudent->setEnabled(true);
    configScore->setEnabled(true);
    if (groupsList->count() > 0)
    {
        groupsList->setCurrentRow(0);
        currentGroup = 0;
    } else {
        for (int i = 0; i < 3; i++) tlist->at(i)->setRowCount(0);
        currentGroup = -1;
    }
}

void MainWindow::createActions()
{
    // Actions
    exitAct = new QAction(this->style()->standardIcon(QStyle::SP_DialogNoButton), tr("Выход"), this);
    exitAct->setShortcut(QKeySequence("Ctrl+Q"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exitApplicationSlot);

    newAct = new QAction(this->style()->standardIcon(QStyle::SP_FileIcon), tr("Новая база данных..."), this);
    newAct->setShortcuts(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &MainWindow::newFileSlot);

    openAct = new QAction(this->style()->standardIcon(QStyle::SP_DirOpenIcon) ,tr("Открыть базу данных..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &MainWindow::openFileSlot);

    saveAct = new QAction(this->style()->standardIcon(QStyle::SP_DialogSaveButton) ,tr("Сохранить изменения"), this);
    saveAct->setShortcut(QKeySequence::Save);
    saveAct->setEnabled(false);
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveDbSlot);

    addStudent = new QAction(this->style()->standardIcon(QStyle::SP_DialogYesButton) ,tr("Добавить студента"), this);
    addStudent->setShortcut(QKeySequence("Ctrl+T"));
    addStudent->setEnabled(false);
    connect(addStudent, &QAction::triggered, this, &MainWindow::addStudentSlot);

    addGroup = new QAction(this->style()->standardIcon(QStyle::SP_FileDialogNewFolder) ,tr("Добавить группу"), this);
    addGroup->setShortcut(QKeySequence("Ctrl+G"));
    addGroup->setEnabled(false);
    connect(addGroup, &QAction::triggered, this, &MainWindow::addGroupSlot);

    configScore = new QAction(this->style()->standardIcon(QStyle::SP_CommandLink) ,tr("Настроить предметы"), this);
    configScore->setShortcut(QKeySequence("Ctrl+C"));
    configScore->setEnabled(false);
    connect(configScore, &QAction::triggered, this, &MainWindow::configureteScoreSlot);

    helpAct = new QAction(this->style()->standardIcon(QStyle::SP_TitleBarContextHelpButton) ,tr("Помощь..."), this);
    helpAct->setShortcut(QKeySequence("Ctrl+H"));
    // TODO

    aboutAct = new QAction(this->style()->standardIcon(QStyle::SP_MessageBoxInformation) ,tr("О программе..."), this);
    // TODO

    aboutQtAct = new QAction(this->style()->standardIcon(QStyle::SP_DesktopIcon) ,tr("О QT..."), this);
    //TODO

    // Toolbars
    QList<QAction*> toolbarActionList1 = {newAct, openAct, saveAct};
    QToolBar *fileToolBar1 = addToolBar(tr("Панель работы с файлами"));
    fileToolBar1->addActions(toolbarActionList1);

    QList<QAction*> toolbarActionList2 = {addGroup, addStudent, configScore};
    QToolBar *fileToolBar2 = addToolBar(tr("Панель работы с данными"));
    fileToolBar2->addActions(toolbarActionList2);
}

void MainWindow::groupSelected()
{
    QListWidgetItem *item = this->groupsList->selectedItems()[0];
    for (int i = 0; i < groups.size(); ++i){
        if (groups.at(i) == item->text()) {
            updateData(i);
            currentGroup = i;
        }
    }
}

void MainWindow::updateSubs(QStringList rsubs)
{
    // for ( const auto& i : rsubs  ) qDebug() << i;
    setSubs(rsubs);
    for (int i = 0; i < groups.size(); i++)
    {
        for (int j = 0; j < rd[i].size(); j++)
        {
            rd[i][j]->perf = "";
        }
    }
    updateData(currentGroup);
}

QStringList MainWindow::getSubs()
{
    QStringList subs;
    QSqlQuery q;
    q.exec("SELECT plain FROM subs;");
    while (q.next()) subs << q.value(0).toString();
    return subs;
}

void MainWindow::setSubs(QStringList subs)
{
    QSqlQuery q;
    q.exec("DELETE FROM subs;");

    for ( const auto& x : subs  ) {
        QSqlQuery q2;
        q2.prepare("INSERT INTO subs (plain) VALUES (:val)");
        q2.bindValue(":val", x);
        q2.exec();
    }
}

void MainWindow::configureteScoreSlot()
{
    QStringList subs;
    subs = getSubs();
    ConfigScore *csw = new ConfigScore(subs, this);
    connect(csw, SIGNAL(sigSubListUpdate(QStringList)), this, SLOT(updateSubs(QStringList)));
    csw->setModal(true);
    csw->exec();
}

void MainWindow::exitApplicationSlot()
{
    close();
}

void MainWindow::addGroupSlot()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Добавление группы"), tr("Введите название группы:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty()) {
        QString qs = "CREATE TABLE ':g' ("
                     "'name'    TEXT NOT NULL UNIQUE,"
                     "'phone'   TEXT,"
                     "'email'   TEXT,"
                     "'pname'   TEXT,"
                     "'pphone'  TEXT,"
                     "'sscore'  TEXT,"
                     "'perf'    TEXT,"
                     "'bdate'   TEXT,"
                     "PRIMARY KEY('name'))";
        QSqlQuery q;
        bool exec_ok = q.exec(qs.replace(":g", text));
        if (exec_ok){
            groups.append(text);

            groupsList->disconnect();
            delete groupsList;
            groupsList = new QListWidget(dock);
            groupsList->addItems(groups);
            dock->setWidget(groupsList);
            connect(groupsList, &QListWidget::itemSelectionChanged, this, &MainWindow::groupSelected);

            rd.append(QList<r*>());

            currentGroup = groups.size() - 1;
            groupsList->setCurrentRow(currentGroup);
            updateData(currentGroup);
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
    bool ok;
    QString text = QInputDialog::getText(this, tr("Добавление студента"), tr("Введите ФИО студента:"), QLineEdit::Normal, "", &ok);
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

void MainWindow::saveDbSlot()
{
    int total = 0;
    for (int g = 0; g < rd.size(); g++) total += rd[g].size();
    qDebug() << total;

    for (int g = 0; g < rd.size(); g++)
    {
        for (int p = 0; p < rd[g].size(); p++)
        {
            QString x = "INSERT OR IGNORE INTO ':table' "
                        "VALUES (':name', ':phone', ':email', ':pname', ':pphone', ':sscore', ':perf', ':bdate');";
            x.replace(":table", groups.at(g));
            x.replace(":name", rd[g][p]->name);
            x.replace(":phone", rd[g][p]->phone);
            x.replace(":email", rd[g][p]->email);
            x.replace(":pname", rd[g][p]->pname);
            x.replace(":pphone", rd[g][p]->pphone);
            x.replace(":sscore", rd[g][p]->sscore);
            x.replace(":perf", rd[g][p]->perf);
            x.replace(":bdate", rd[g][p]->bdate);
            QSqlQuery qx;
            qx.exec(x);
            // qDebug() << qx.lastQuery();
            QString sq = "UPDATE ':table' "
                         "SET "
                         "phone = ':phone', "
                         "email = ':email', "
                         "pname = ':pname', "
                         "pphone = ':pphone', "
                         "sscore = ':sscore', "
                         "perf = ':perf', "
                         "bdate = ':bdate' "
                         "WHERE name = ':who';";
            sq.replace(":table", groups.at(g));
            sq.replace(":who", rd[g][p]->name);
            sq.replace(":phone", rd[g][p]->phone);
            sq.replace(":email", rd[g][p]->email);
            sq.replace(":pname", rd[g][p]->pname);
            sq.replace(":pphone", rd[g][p]->pphone);
            sq.replace(":sscore", rd[g][p]->sscore);
            sq.replace(":perf", rd[g][p]->perf);
            sq.replace(":bdate", rd[g][p]->bdate);
            QSqlQuery q;
            bool ok = q.exec(sq);
            if (!ok) {
                // qDebug() << "error " << rd[g][p]->name;
            }
        }
    }
    changed = false;
}

QStringList MainWindow::getPerfs(QString perfStr)
{
    if (perfStr == "")
    {
        QStringList strLst;
        for (int i = 0; i < getSubs().size(); i++) {strLst << " ";}
        // qDebug() << "suc " << strLst;
        return strLst;
    } else {
        // qDebug() << "suc " << perfStr.split(",");
        return perfStr.split(",");
    }
}

void MainWindow::mcChanged(int x, int y)
{
    if (!cani) return;
    if (tabWidget->currentIndex() == 2)
    {
        QStringList perfStr;
        for (int i = 1; i < tlist->at(2)->columnCount(); i++) {
            qDebug() << tlist->at(2)->item(x, i)->text();
            QString t = tlist->at(2)->item(x, i)->text();
            if (t == "") t = " ";
            perfStr.append(t);
        }
        QString tmp = perfStr.join(",");
        rd[currentGroup][x]->perf = tmp;
        changed = true;
        return;
    }
    QString t = tlist->at(tabWidget->currentIndex())->item(x, y)->text();
    QString c = tlist->at(tabWidget->currentIndex())->horizontalHeaderItem(y)->text();
    QStringList crutch;
    crutch << "Дата рождения" << "Телефон" << "E-Mail" << "Баллы ЕГЭ" << "ФИО Родителя" << "Телефон родителя";
    switch (crutch.indexOf(c)) {
    case 0:
    {
        rd[currentGroup][x]->bdate = t;
        break;
    }
    case 1:
    {
        rd[currentGroup][x]->phone = t;
        break;
    }
    case 2:
    {
        rd[currentGroup][x]->email = t;
        break;
    }
    case 3:
    {
        rd[currentGroup][x]->sscore = t;
        break;
    }
    case 4:
    {
        rd[currentGroup][x]->pname = t;
        break;
    }
    case 5:
    {
        rd[currentGroup][x]->pphone = t;
        break;
    }
    default:
        return;
    }
    changed = true;
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("Файл"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    toolsMenu = menuBar()->addMenu(tr("Инструменты"));
    toolsMenu->addAction(addGroup);
    toolsMenu->addAction(addStudent);
    toolsMenu->addAction(configScore);
    toolsMenu->addSeparator();
    toolsMenu->addAction(dock->toggleViewAction());

    helpMenu = menuBar()->addMenu(tr("Помощь"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void MainWindow::updateData(int group)
{
    cani = false;
    if (rd.size() == 0) return;
    tlist->at(0)->setRowCount(rd[group].size());
    for (int i = 0; i < rd[group].size(); ++i)
    {
        tlist->at(0)->setItem(i, 0, new QTableWidgetItem(rd[group].at(i)->name));
        tlist->at(0)->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        tlist->at(0)->setItem(i, 1, new QTableWidgetItem(rd[group].at(i)->bdate));
        tlist->at(0)->setItem(i, 2, new QTableWidgetItem(rd[group].at(i)->phone));
        tlist->at(0)->setItem(i, 3, new QTableWidgetItem(rd[group].at(i)->email));
        tlist->at(0)->setItem(i, 4, new QTableWidgetItem(rd[group].at(i)->sscore));
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

    QStringList headers;
    QStringList asubs = getSubs();
    headers << "ФИО";

    for (int i = 0; i < asubs.size(); i++) headers << asubs.at(i);
    tlist->at(2)->setColumnCount(headers.size());
    tlist->at(2)->setRowCount(rd[group].size());
    tlist->at(2)->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < rd[group].size(); ++i)
    {
        tlist->at(2)->setItem(i, 0, new QTableWidgetItem(rd[group].at(i)->name));
        tlist->at(2)->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        QStringList perfs = getPerfs(rd[group][i]->perf);
        for (int j = 1; j < headers.size(); j++)
        {
            tlist->at(2)->setItem(i, j, new QTableWidgetItem(perfs.at(j-1)));
        }
    }
    tlist->at(2)->resizeColumnsToContents();
    cani = true;
}

