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

private slots:
    void groupSelected();
    void mcChanged(int, int);

private:
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *printAct;
    QAction *exitAct;

    QAction *undoAct;
    QAction *redoAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;

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
};


#endif // MAINWINDOW_H
