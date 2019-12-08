#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QListWidget>

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

    QTabWidget *tabWidget;
    QListWidget *groupsList;

    QTableWidget *table;

};


#endif // MAINWINDOW_H
