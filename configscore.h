#ifndef CONFIGSCORE_H
#define CONFIGSCORE_H

#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "mainwindow.h"

namespace Ui {
class ConfigScore;
}

class ConfigScore : public QDialog
{
    Q_OBJECT

public:
    ConfigScore(QStringList, QWidget *parent = nullptr);
    ~ConfigScore();

signals:
    void sigSubListUpdate(QStringList);

private slots:
    void on_addButton_clicked();

    void on_removeButton_clicked();

    void on_cancelButton_clicked();

    void on_saveButton_clicked();

private:
    bool changed;
    QStringList subsList;
    Ui::ConfigScore *ui;
};

#endif // CONFIGSCORE_H
