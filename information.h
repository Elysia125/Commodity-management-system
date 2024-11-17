#ifndef INFORMATION_H
#define INFORMATION_H

#include <QDialog>

namespace Ui {
class Information;
}

class Information : public QDialog
{
    Q_OBJECT

public:
    explicit Information(QWidget *parent = nullptr);
    ~Information();

    void init();
    void connectSql();
private:
    Ui::Information *ui;
    QSet<QString>ID1,ID2;
};

#endif // INFORMATION_H
