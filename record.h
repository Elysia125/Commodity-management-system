#ifndef RECORD_H
#define RECORD_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include <QTableWidgetItem>
namespace Ui {
class Record;
}

class Record : public QDialog
{
    Q_OBJECT

public:
    explicit Record(QWidget *parent = nullptr);
    Record(QString tablename);
    ~Record();

    void setTableTitle(QString name);
    void rebuildTable();
    void connectSql();
    void connections();
public slots:
    void reButton_clicked();
    void deleteButton_clicked();
    void item_clicked(QTableWidgetItem *item);
private:
    Ui::Record *ui;
    //数据库中table名
    QString tablename;

};

#endif // RECORD_H
