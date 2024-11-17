#include "record.h"
#include "tools.h"
#include "ui_record.h"

#include <QMessageBox>

Record::Record(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Record)
{
    ui->setupUi(this);
    //连接数据库
    connectSql();
    //连接信号与槽
    connections();
}
Record::Record(QString tablename)
    :ui(new Ui::Record),tablename(std::move(tablename)) {
    ui->setupUi(this);
    //连接数据库
    connectSql();
    //连接信号与槽
    connections();

    //单元格自适应大小
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 设置所有单元格不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->horizontalHeader()->setMinimumSectionSize(70); // 设置最小宽度为70像素
}
//设置表格标题
void Record::setTableTitle(QString name){
    ui->title->setText(name);
    QFont font("Arial", 20); // 字体名和点大小
    ui->title->setFont(font);
    // 设置文本居中
    ui->title->setAlignment(Qt::AlignCenter);
}
//返回按钮
void Record::reButton_clicked(){
    this->close();
}
//全部删除按钮
void Record::deleteButton_clicked(){
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(db.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery sql(db);
    sql.exec("delete from "+tablename);
    db.close();
    rebuildTable();
}
//重建表格
void Record::rebuildTable(){
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    connectSql();
}
//连接数据库
void Record::connectSql(){
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    bool tableExists = sql.exec("SELECT name FROM record WHERE type='table' AND name= '"+tablename+"'");
    if(tableExists && sql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
        sql.exec("create table "+tablename+" (itemnumber NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                 "itemprice NOT NULL,number NOT NULL,time)");
    }
    sql.exec("select * from "+tablename);
    while(sql.next()){
        int row= ui->tableWidget->rowCount(); // 获取当前的行数，将在最后插入一行
        //表格新建一行
        ui->tableWidget->insertRow(row);
        //每行中各个方格的数据
        QVector<QTableWidgetItem*>item;
        QTableWidgetItem *item1 = new QTableWidgetItem(sql.value("itemnumber").toString());
        QTableWidgetItem *item2 = new QTableWidgetItem(sql.value("itemname").toString());
        QTableWidgetItem *item3 = new QTableWidgetItem(sql.value("itemcategory").toString());
        QTableWidgetItem *item4 = new QTableWidgetItem(sql.value("itembrand").toString());
        QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemsupplier").toString());
        QTableWidgetItem *item6 = new QTableWidgetItem(sql.value("itemprice").toString());
        QTableWidgetItem *item7 = new QTableWidgetItem(sql.value("number").toString());
        QTableWidgetItem *item8 = new QTableWidgetItem(QString::number((sql.value("itemprice").toInt())*(sql.value("number").toInt())));
        QTableWidgetItem *item9 = new QTableWidgetItem(sql.value("time").toString());
        QTableWidgetItem *item10 = new QTableWidgetItem("删除");
        //qDebug()<<QString::number((sql.value("itemprice").toInt())*(sql.value("number").toInt()));
        item.push_back(item1);
        item.push_back(item2);
        item.push_back(item3);
        item.push_back(item4);
        item.push_back(item5);
        item.push_back(item6);
        item.push_back(item7);
        item.push_back(item8);
        item.push_back(item9);
        item.push_back(item10);
        int col=0;
        foreach(QTableWidgetItem *i,item){
            ui->tableWidget->setItem(row,col,i);
            col++;
        }
    }

    sdb.close();
    //右下角
    ui->number->setText("共"+QString::number(ui->tableWidget->rowCount())+"条");
}
//方格点击事件
void Record::item_clicked(QTableWidgetItem*item){
    // 获取被点击的单元格的行和列
    int row = item->row();
    int column = item->column();
    if(column==9){
        //删除操作
        int result=QMessageBox::information(this,tr("警告"),tr("确定删除吗")
                                              ,QMessageBox::Yes,QMessageBox::No);
        //qDebug()<<result<<":"<<QMessageBox::Yes;
        if(result==QMessageBox::Yes){
            QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
            if(sdb.open()){
                qDebug()<<"数据库打开成功cccc";
            }else{
                qDebug()<<"数据库打开失败aaaa";
            }
            QSqlQuery sql(sdb);
            sql.exec("delete from "+tablename+" where "
                     "time='"+ui->tableWidget->item(row,8)->text()+"'");
            sdb.close();
            this->rebuildTable();
        }
    }else{
        //显示内容
        QMessageBox::information(this,tr("内容"),item->text());
    }
}
void Record::connections(){
    //返回
    connect(ui->reButton,&QPushButton::clicked,
            this,&Record::reButton_clicked);
    //全部删除
    connect(ui->deleteButton,&QPushButton::clicked,
            this,&Record::deleteButton_clicked);
    //表格点击事件
    connect(ui->tableWidget, &QTableWidget::itemClicked,
            this, &Record::item_clicked);
}

Record::~Record()
{
    delete ui;
}
