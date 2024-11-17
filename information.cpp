#include "information.h"
#include "tools.h"
#include "ui_information.h"

#include <QSqlDatabase>

Information::Information(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Information)
{
    ui->setupUi(this);
    connectSql();
    init();
}
//连接至库存数据库，进行表格初始化
void Information::connectSql(){
    int val1=0,val2=INT_MAX;
    QSqlDatabase cdb=QSqlDatabase::addDatabase("QSQLITE");
    cdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/val.db");
    if(cdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery cql(cdb);
    cql.exec("select * from WarningThreshold");
    while(cql.next()){
        val1=cql.value("val1").toInt();
        val2=cql.value("val2").toInt();
    }
    cdb.close();
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    sql.exec("select * from inventoryproduct");
    while(sql.next()){
        if(sql.value("number").toInt()<val1){
            ID1.insert(sql.value("itemnumber").toString());
        }
        if(sql.value("number").toInt()>val2){
            ID2.insert(sql.value("itemnumber").toString());
        }
    }
    sdb.close();
}
//初始化
void Information::init(){
    bool isempty=true;
    foreach(QString s,ID1){
        QListWidgetItem *item = new QListWidgetItem("库存ID为"+s+"的库存商品数量过少，请及时补货");

        // 设置行高
        item->setSizeHint(QSize(item->sizeHint().width(), 40)); // 设置行高为40像素

        // 禁用编辑
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        ui->listWidget->addItem(item);
        isempty=false;
    }
    foreach(QString s,ID2){
        QListWidgetItem *item = new QListWidgetItem("库存ID为"+s+"的库存商品数量过多，建议减少进货量或降价出售");

        // 设置行高
        item->setSizeHint(QSize(item->sizeHint().width(), 40)); // 设置行高为40像素

        // 禁用编辑
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        ui->listWidget->addItem(item);
        isempty=false;
    }
    if(isempty){
        QListWidgetItem *item = new QListWidgetItem("暂无通知");

        // 设置行高
        item->setSizeHint(QSize(item->sizeHint().width(), 200)); // 设置行高为200像素

        // 禁用编辑
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        ui->listWidget->addItem(item);
    }
}
Information::~Information()
{
    delete ui;
}
