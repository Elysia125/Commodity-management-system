#include "delrecord.h"
#include "ui_delrecord.h"
#include"tools.h"
#include<QMessageBox>
DelRecord::DelRecord(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DelRecord)
{
    ui->setupUi(this);
    //连接数据库
    connectSQL();
    //连接事件
    connections();
    //初始化
    init();
}
//连接数据库，将数据库中的数据加载至表格中
void DelRecord::connectSQL(){
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    if(db.open()){
        qDebug()<<"数据库打开成功ffff";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(db);

    sql.exec("select * from delproduct");
    while(sql.next()){
        int row= ui->tableWidget->rowCount(); // 获取当前的行数，将在最后插入一行
        //qDebug()<<row;
        //表格新建一行
        ui->tableWidget->insertRow(row);
        //存储每一行信息
        QVector<QString> message={sql.value("itemnumber").toString(),sql.value("itemname").toString(),
                                     sql.value("itemcategory").toString(),sql.value("itembrand").toString(),
                                    sql.value("deltime").toString(),"删除","恢复"};
        inserttoTable(message,row);
        Product p(sql.value("itemnumber").toString(),sql.value("itemname").toString(),
                    sql.value("itemcategory").toString(),sql.value("itembrand").toString(),
                    sql.value("itemsupplier").toString(),sql.value("itemprice").toInt(),
                    sql.value("Inventoryquantity").toInt(),sql.value("productdescription").toString());
        pro.push_back(p);
    }
    db.close();
}
void DelRecord::connections(){
    //表格点击事件
    connect(ui->tableWidget, &QTableWidget::itemClicked,
            this, &DelRecord::item_clicked);
    //返回事件
    connect(ui->reButton,&QPushButton::clicked,
            this,&DelRecord::reButton_clicked);
    //全部删除事件
    connect(ui->delButton,&QPushButton::clicked,
            this,&DelRecord::AllDeleteButton_clicked);
    //全部恢复事件
    connect(ui->recoverButton,&QPushButton::clicked,
            this,&DelRecord::AllRecoverButton_clicked);
}
//初始化
void DelRecord::init(){
    // 使列宽自适应内容
    // ui->tableWidget->resizeColumnsToContents();
    // ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 设置所有单元格不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    //ui->tableWidget->horizontalHeader()->setMinimumSectionSize(100); // 设置最小宽度为100像素
}
//将商品信息添加进表格中
void DelRecord::inserttoTable(QVector<QString>message,int row){
    int col=0;
    //qDebug()<<"1111";
    foreach(QString s,message){
        QTableWidgetItem*item=new QTableWidgetItem(s);
        ui->tableWidget->setItem(row,col,item);
        if(col==5){
            item->setForeground(QBrush(QColor(Qt::red)));
        }else if(col==6){
            item->setForeground(QBrush(QColor(Qt::blue)));
        }
        col++;
        // 调整所有行和列的大小
    }
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}
//返回按钮点击事件
void DelRecord::reButton_clicked(){
    this->close();
}
//全部恢复事件
void DelRecord::AllRecoverButton_clicked(){
    //QVector<Product>p=pro;
    // //清空删除记录数据库
    // delSQL(pro);

    //将数据添加到商品信息数据库
    addDataToSQL(pro);
    delSQL(successpro);

    //清空表格
    TableClear();

}

//全部删除事件
void DelRecord::AllDeleteButton_clicked(){
    delSQL(pro);
    TableClear();
}
//清空删除记录数据库
void DelRecord::delSQL(QVector<Product>&p){
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    //打开数据库
    if(db.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败cccc";
        return;
    }
    QSqlQuery sql(db);
    //清出指定内容

    foreach(Product pro,p){
        sql.exec("delete from delproduct where itemnumber='"
                 +pro.getItemnumber()+"'");
        qDebug()<<"ccc";
        int index=0;
        foreach(Product p,Tools::delrecord){
            if(p.getItemnumber()==pro.getItemnumber()){
                Tools::delrecord.removeAt(index);
            }
            index++;
        }
    }
    int index=0;
    foreach(Product pro,p){
        p.removeAt(index);
    }
    db.close();
}
//向商品数据库中添加商品
bool DelRecord::addDataToSQL(QVector<Product>&p){
    bool result=true;
    //加入内容
    sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功ffff";
    }else{
        qDebug()<<"数据库打开失败";
        return false;
    }
    QSqlQuery qsql(sdb);
    int count=0;

    QVector<int>indexs;
    int start=0;
    //QVector<Product>temp;
    foreach(Product pro,p){
        if(!qsql.exec("insert into product(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                 "itemprice,Inventoryquantity,productdescription)"
                 " values('"+pro.getItemnumber()+"','"+pro.getItemname()+"','"+pro.getItemcategory()+"','"
                 +pro.getItembrand()+"','"+pro.getItemsupplier()+"',"+QString::number(pro.getItemprice())+","
                       +QString::number(pro.getInventoryquantity())+",'"+pro.getProductdescription()+"')"))
        {
            count++;
            indexs.push_back(start);
            result=false;
        }else{
            successpro.push_back(pro);
        }
        start++;
    }
    if(count>0){
        QMessageBox::information(this,tr("提示"),tr("部分商品信息恢复失败\n原因：商品信息除描述外为空或商品编号已存在"));
    }
    int index=0;
    foreach(Product pro,p){
        if(indexs.contains(index)){
            index++;
        }else{
            p.removeAt(index);
        }

    }
    sdb.close();
    //delSQL(temp);
    return result;
}
//清空表格
void DelRecord::TableClear(){
    this->pro.clear();
    this->successpro.clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}
//表格点击事件
void DelRecord::item_clicked(QTableWidgetItem *item){
    // 获取被点击的单元格的行和列
    int row = item->row();
    int column = item->column();
    qDebug()<<row<<" "<<column;
    if(column==5){
        QVector<Product>p;
        p.push_back(pro.at(row));
        pro.remove(row);
        delSQL(p);
        //重建表格
        TableClear();
        connectSQL();
    }else if(column==6){
        QVector<Product>p;
        QVector<Product>p1;
        p.push_back(pro.at(row));
        p1.push_back(pro.at(row));
        //成功删除
        if(addDataToSQL(p)){
            pro.remove(row);
            delSQL(p1);
        }
        //重建表格
        TableClear();
        connectSQL();
    }else{
        QMessageBox::information(this,tr("内容"),item->text());
    }
}
DelRecord::~DelRecord()
{
    delete ui;
}
