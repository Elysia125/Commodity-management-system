#ifndef PRIDUCTCHANGE_H
#define PRIDUCTCHANGE_H

#include <QDialog>
#include"product.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QKeyEvent>
namespace Ui {
class PriductChange;
}
//该类为修改商品信息界面
class PriductChange : public QDialog
{
    Q_OBJECT

public:
    explicit PriductChange(QWidget *parent = nullptr);
    PriductChange(const Product &pro);

    ~PriductChange();

    void init();
    void connections();
    void datatoSQL();
    //关闭事件重写
    void closeEvent(QCloseEvent*)override;
    void keyPressEvent(QKeyEvent*)override;
    void setPro(const Product &newPro);

public slots:
    void reButtonAndClose_clicked();
    void restoreButton_clicked();
private:
    Ui::PriductChange *ui;
    // QString itemnumber;//商品编号
    // QString itemname;//商品名称
    // QString itemcategory;//商品类别
    // QString itembrand;//品牌
    // QString itemsupplier;//供应商
    // int itemprice;//单价
    // QString Inventoryquantity;//库存数量
    // QString productdescription;//商品描述
    Product pro;
    //数据库对象
    QSqlDatabase db;
    //暂时性存储商品编号，方便数据库修改
    QString id;
    //数据是否保存
    bool isrestore=false;
    //是否保存成功
    bool issuccess=true;
};

#endif // PRIDUCTCHANGE_H
