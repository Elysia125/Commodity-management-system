#ifndef DELRECORD_H
#define DELRECORD_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QVector>
#include"priductchange.h"
#include<QTableWidgetItem>
namespace Ui {
class DelRecord;
}

class DelRecord : public QDialog
{
    Q_OBJECT

public:
    explicit DelRecord(QWidget *parent = nullptr);
    ~DelRecord();

    void connectSQL();
    void inserttoTable(QVector<QString> message, int row);
    void delSQL(QVector<Product>&p);
    bool addDataToSQL(QVector<Product>&p);
    void connections();
    void init();
public slots:
    void reButton_clicked();
    void AllRecoverButton_clicked();
    void AllDeleteButton_clicked();
    void TableClear();
    void item_clicked(QTableWidgetItem *item);
private:
    Ui::DelRecord *ui;
    QSqlDatabase db;
    QSqlDatabase sdb;
    //存储商品信息，方便后续删除和恢复
    QVector<Product>pro;
    //恢复成功的商品信息
    QVector<Product>successpro;
};

#endif // DELRECORD_H
