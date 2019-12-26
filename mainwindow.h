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

private slots:
    void groupSelected();
    void mcChanged(int, int);

private:
    struct r {
        QString name;
        QString phone;
        QString email;
        QString pname;
        QString pphone;
        QString sscore;
        QString perf;
    };

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;

    QAction *helpAct;
    QAction *aboutAct;
    QAction *aboutQtAct;

    void createDockWindow();
    void createActions();
    void createMenus();


    QSqlDatabase db;

    QTabWidget *tabWidget;
    QListWidget *groupsList;

    QList<QTableWidget*> *tlist;
    QList<QString> *nlist;

    QList<QString> *groups;

    QList< QList<r*> > rd;
};


#endif // MAINWINDOW_H
