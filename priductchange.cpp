#include "priductchange.h"
#include "tools.h"
#include "ui_priductchange.h"
#include <QDateTime>
#include<QMessageBox>
PriductChange::PriductChange(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PriductChange)
{

    ui->setupUi(this);
    //初始化
    init();
    //连接
    connections();

}

PriductChange::~PriductChange()
{
    delete ui;
}
void PriductChange::init(){
    //初始化各输入框中的文本
    ui->itemnumberEdit->setText(pro.getItemnumber());
    ui->itemnameEdit->setText(pro.getItemname());
    ui->itemcategoryEdit->setText(pro.getItemcategory());
    ui->itembrandEdit->setText(pro.getItembrand());
    ui->itempriceEdit->setText(QString::number(pro.getItemprice()));
    ui->InventoryquantityEdit->setText(QString::number(pro.getInventoryquantity()));
    ui->itemsupplierEdit->setText(pro.getItemsupplier());
    ui->productdescriptionEdit->setText(pro.getProductdescription());
    //存储商品编号
    id=pro.getItemnumber();
    qDebug()<<"初始化完成";
}
//信号与槽
void PriductChange::connections(){
//返回与关闭按钮
    connect(ui->reButton,&QPushButton::clicked,
        this,&PriductChange::reButtonAndClose_clicked);
    connect(ui->restoreButton,&QPushButton::clicked,
        this,&PriductChange::restoreButton_clicked);
    //信号与槽连接成功
    qDebug()<<"信号与槽连接成功";
}


PriductChange::PriductChange(const Product &pro): ui(new Ui::PriductChange),pro(pro)
{
    qDebug()<<"到了这里";
    ui->setupUi(this);
    //初始化
    init();
    //连接
    connections();

}

//返回按钮和关闭功能
void PriductChange::reButtonAndClose_clicked(){
    if(!isrestore){
        int result=QMessageBox::information(this,tr("提示"),tr("你所做的修改尚未保存，是否进行保存"),
                                              QMessageBox::Yes,QMessageBox::No);
        if(result==QMessageBox::Yes){
            restoreButton_clicked();
            //datatoSQL();
            // QMessageBox::information(this,tr("提示"),tr("保存成功"),
            //                          QMessageBox::Ok);
            isrestore=true;
        }else{
        }
    }
    if(isrestore){
        accept();
    }else{
        this->hide();
    }
}
//保存修改按钮功能
void PriductChange::restoreButton_clicked(){
    //修改数据
    pro.setItemnumber(ui->itemnumberEdit->text());
    pro.setItemname(ui->itemnameEdit->text());
    pro.setItemcategory(ui->itemcategoryEdit->text());
    pro.setItembrand(ui->itembrandEdit->text());
    pro.setItemsupplier(ui->itemsupplierEdit->text());
    pro.setItemprice(ui->itempriceEdit->text().toInt());
    pro.setInventoryquantity(ui->InventoryquantityEdit->text().toInt());
    pro.setProductdescription(ui->productdescriptionEdit->toPlainText());
    //将数据库中的数据修改
    datatoSQL();
    if(issuccess){
        QMessageBox::information(this,tr("提示"),tr("保存成功"),
                                 QMessageBox::Ok);
        isrestore=true;
    }

}

void PriductChange::setPro(const Product &newPro)
{
    qDebug()<<"cccccc";
    pro = newPro;
    qDebug()<<pro.getInventoryquantity();
}
//数据存入数据库
void PriductChange::datatoSQL(){
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    if(db.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(db);
    bool tableExists = sql.exec("SELECT name FROM productinfomation WHERE type='table' AND name='product'");
    if(tableExists && sql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
    sql.exec("create table product(itemnumber UNIQUE NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
             "itemprice NOT NULL,Inventoryquantity NOT NULL,productdescription)");
    }
    if(pro.getItemnumber().isEmpty()||pro.getItemname().isEmpty()||pro.getItemcategory().isEmpty()||pro.getItembrand().isEmpty()||
        pro.getItemsupplier().isEmpty()||QString::number(pro.getItemprice()).isEmpty()||QString::number(pro.getInventoryquantity()).isEmpty()){
        QMessageBox::information(this,tr("提示"),tr("该商品编号已存在或存在除描述外的商品信息为空，无法添加"));
        issuccess=false;
        db.close();
        return;
    }
    sql.exec("delete from product where itemnumber='"+id+"'");

    if(!sql.exec("insert into product(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
             "itemprice,Inventoryquantity,productdescription)"
             " values('"+pro.getItemnumber()+"','"+pro.getItemname()+"','"+pro.getItemcategory()+"','"
             +pro.getItembrand()+"','"+pro.getItemsupplier()+"',"+QString::number(pro.getItemprice())+","
                    +QString::number(pro.getInventoryquantity())+",'"+pro.getProductdescription()+"')"))
    {
        QMessageBox::information(this,tr("提示"),tr("该商品编号已存在或存在除描述外的商品信息为空，无法添加"));
        issuccess=false;
    }else{
        issuccess=true;
    }
    db.close();
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery qsql(sdb);
    //录入进货记录
    if(!id.isEmpty()){
        return;
    }
    // 获取当前的日期和时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 将当前的日期和时间转换为字符串，这里使用格式 "yyyy/MM/dd hh:mm:ss"
    QString formattedDateTime = currentDateTime.toString("yyyy/MM/dd hh:mm:ss");
    tableExists = qsql.exec("SELECT name FROM record WHERE type='table' AND name='restockrecord'");
    if(tableExists && qsql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
        qsql.exec("create table restockrecord(itemnumber NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                  "itemprice NOT NULL,number NOT NULL,productdescription,time)");
    }

    if(!qsql.exec("insert into restockrecord(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                   "itemprice,number,productdescription,time)"
                   " values('"+ui->itemnumberEdit->text()+"','"+ui->itemnameEdit->text()+"','"+ui->itemcategoryEdit->text()+"','"
                   +ui->itembrandEdit->text()+"','"+ui->itemsupplierEdit->text()+"',"+ui->itempriceEdit->text()+","
                   +ui->InventoryquantityEdit->text()+",'"+""+"','"+formattedDateTime+"')"))
    {
        qDebug()<<"无法录入进货记录,有数据为空";
    }
    sdb.close();
    // db=QSqlDatabase::addDatabase("QSQLITE");
    // db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    // if(db.open()){
    //     qDebug()<<"数据库打开成功";
    // } else {
    //     qDebug()<<"数据库打开失败";
    //     return;
    // }

    // QSqlQuery query(db);
    // bool tableExists = query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='product'");
    // if(tableExists && query.next()) {
    //     // 表已存在，不执行创建操作
    // } else {
    //     // 表不存在，执行创建操作
    //     query.exec("create table product(itemnumber TEXT, itemname TEXT, itemcategory TEXT, itembrand TEXT, itemsupplier TEXT, itemprice INTEGER, Inventoryquantity INTEGER, productdescription TEXT)");
    // }

    // query.exec("delete from product where itemnumber='"+id+"'");

    // query.exec("insert into product(itemnumber,itemname,itemcategory,itembrand,itemsupplier,itemprice,Inventoryquantity,productdescription) "
    //            "values('"+pro.getItemnumber()+"','"+pro.getItemname()+"','"+pro.getItemcategory()+"','"+pro.getItembrand()+"','"+pro.getItemsupplier()+"',"
    //            +QString::number(pro.getItemprice())+","+QString::number(pro.getInventoryquantity())+",'"+pro.getProductdescription()+"')");

    // db.close();
}
//关闭按钮功能重写
void PriductChange::closeEvent(QCloseEvent*){
    reButtonAndClose_clicked();
}
//键盘事件重写
void PriductChange::keyPressEvent(QKeyEvent*event){
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        // 用户按下了 Enter 键
        // 在这里处理 Enter 键被按下的情况
        // 获取当前焦点所在的QLineEdit对象
        QWidget *focusedWidget = QApplication::focusWidget();
        if (focusedWidget != nullptr && focusedWidget->inherits("QLineEdit")) {
            if(focusedWidget == ui->itemnumberEdit){
                ui->itemnameEdit->setFocus();
            }else if(focusedWidget == ui->itemnameEdit){
                ui->itemcategoryEdit->setFocus();
            }else if(focusedWidget == ui->itemcategoryEdit){
                ui->itembrandEdit->setFocus();
            }else if(focusedWidget == ui->itembrandEdit){
                ui->itempriceEdit->setFocus();
            }else if(focusedWidget == ui->itempriceEdit){
                ui->itemsupplierEdit->setFocus();
            }else if(focusedWidget == ui->itemsupplierEdit){
                ui->InventoryquantityEdit->setFocus();
            }else if(focusedWidget == ui->InventoryquantityEdit){
                ui->productdescriptionEdit->setFocus();
            }else if(focusedWidget == ui->productdescriptionEdit){
                restoreButton_clicked();
            }
        }
    }
    event->accept();
}
