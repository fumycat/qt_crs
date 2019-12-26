#ifndef CONFIGSCORE_H
#define CONFIGSCORE_H

#include <QWidget>

namespace Ui {
class ConfigScore;
}

class ConfigScore : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigScore(QStringList, QWidget *parent = nullptr);
    ~ConfigScore();

private:
    Ui::ConfigScore *ui;
};

#endif // CONFIGSCORE_H
