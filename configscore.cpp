#include "configscore.h"
#include "ui_configscore.h"

ConfigScore::ConfigScore(QStringList p, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigScore)
{
    ui->setupUi(this);
    changed = false;
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->listWidget->addItems(p);
    for ( const auto& i : p  ) subsList << i;
}

ConfigScore::~ConfigScore()
{
    delete ui;
}

void ConfigScore::on_addButton_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Добавление предмета"), tr("Введите новый предмет:"), QLineEdit::Normal, "", &ok);
    if (ok && !text.isEmpty())
    {
        for ( const auto& i : subsList  )
        {
            if (i == text) {
                QMessageBox::warning(this, tr("Ошибка"), tr("Такой предмет уже есть."));
                return;
            }
        }
        subsList << text;
        ui->listWidget->addItem(text);
        changed = true;
    }
}

void ConfigScore::on_removeButton_clicked()
{
    if (ui->listWidget->selectedItems().length() < 0)
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Нечего удалять."));
        return;
    }
    QListWidgetItem *item = ui->listWidget->selectedItems()[0];
    subsList.removeOne(item->text());
    delete ui->listWidget->takeItem(ui->listWidget->row(item));
    changed = true;
}

void ConfigScore::on_cancelButton_clicked()
{
    if (!changed)
    {
        this->close();
        return;
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Выход", "Вы уверены, что хотите выйти?\n"
                                            "Все изменения будут потеряны.", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            this->close();
        } else {
            return;
        }
    }
}

void ConfigScore::on_saveButton_clicked()
{
    if (!changed)
    {
        this->close();
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Сохранить", "Вы уверены, что хотите сохранить новый список предметов?\n"
                                        "Это действие очистит все оценки.", QMessageBox::Yes|QMessageBox::No);
    switch (reply) {
        case QMessageBox::Yes:
        {
            emit sigSubListUpdate(subsList);
            this->close();
            return;
        }
        default:
        {
            return;
        }
    }
}
