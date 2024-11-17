#ifndef INVENTORYPRODUCTCHANGE_H
#define INVENTORYPRODUCTCHANGE_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
//修改或添加库存信息类
namespace Ui {
class InventoryproductChange;
}

class InventoryproductChange : public QDialog
{
    Q_OBJECT

public:
    explicit InventoryproductChange(QWidget *parent = nullptr);
    InventoryproductChange(QString ID);
    ~InventoryproductChange();

    void init();
    void connections();
public slots:
    void restoreButton_clicked();
    void reButtonAndClose_clicked();
protected:
    void closeEvent(QCloseEvent *)override;
    void keyPressEvent(QKeyEvent *event)override;
private:
    Ui::InventoryproductChange *ui;
    //库存编号
    QString ID=NULL;
    //数据是否保存
    bool isrestore=false;
    //是否保存成功
    bool issuccess=true;
};

#endif // INVENTORYPRODUCTCHANGE_H
