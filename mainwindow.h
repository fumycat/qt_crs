#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QListWidget>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QStandardPaths>
#include <QStyle>
#include <QInputDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include <QtSql>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void updateData(int);

public slots:
    void updateSubs(QStringList);

private slots:
    void groupSelected();
    void mcChanged(int, int);
    void addStudentSlot();
    void addGroupSlot();
    void exitApplicationSlot();
    void openFileSlot();
    void newFileSlot();
    void configureteScoreSlot();
    void saveDbSlot();
    void aboutSlot();
    void aboutQtSlot();
    void helpSlot();

private:
    struct r {
        QString name;
        QString phone;
        QString email;
        QString pname;
        QString pphone;
        QString sscore;
        QString perf;
        QString bdate;
    };

    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;

    QAction *addGroup;
    QAction *addStudent;
    QAction *configScore;

    QAction *helpAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    void createTabs();
    void createActions();
    void createMenus();
    void closeEvent(QCloseEvent*);

    QStringList getSubs();
    void setSubs(QStringList);
    QStringList getPerfs(QString);

    int currentGroup;
    bool cani;
    bool changed;

    QSqlDatabase db;

    QTabWidget *tabWidget;
    QListWidget *groupsList;
    QDockWidget *dock;

    QList<QTableWidget*> *tlist;

    QStringList groups;

    QList< QList<r*> > rd;
};


#endif // MAINWINDOW_H
