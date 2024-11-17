#include "inventoryproductchange.h"
#include "tools.h"
#include "ui_inventoryproductchange.h"

#include <QMessageBox>
#include<QKeyEvent>
#include <QDateTime>
InventoryproductChange::InventoryproductChange(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::InventoryproductChange)
{
    ui->setupUi(this);
    init();
    connections();

}
InventoryproductChange::InventoryproductChange(QString ID)
    :ui(new Ui::InventoryproductChange),ID(std::move(ID)){
    ui->setupUi(this);
    //初始化
    init();
    //连接
    connections();
}
InventoryproductChange::~InventoryproductChange()
{
    delete ui;
}

void InventoryproductChange::init(){
    if(ID.isNull()){
        //初始化各输入框中的文本
        ui->itempriceEdit->setText("0");
        ui->InventoryquantityEdit->setText("0");
    }else{
        QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
        if(db.open()){
            qDebug()<<"数据库打开成功";
        }else{
            qDebug()<<"数据库打开失败";
        }
        QSqlQuery sql(db);
        sql.exec("select * from inventoryproduct");
        while(sql.next()){
            if(sql.value("itemnumber")==ID){
                //初始化各输入框中的文本
                ui->itemnumberEdit->setText(ID);
                ui->itemnameEdit->setText(sql.value("itemname").toString());
                ui->itemcategoryEdit->setText(sql.value("itemcategory").toString());
                ui->itembrandEdit->setText(sql.value("itembrand").toString());
                ui->itempriceEdit->setText(sql.value("itemprice").toString());
                ui->InventoryquantityEdit->setText(sql.value("number").toString());
                ui->itemsupplierEdit->setText(sql.value("itemsupplier").toString());
            }
        }
        db.close();
    }
    //qDebug()<<"初始化完成";
}
//保存修改按钮功能
void InventoryproductChange::restoreButton_clicked(){

    //将数据库中的数据修改
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    if(db.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(db);

    bool tableExists = sql.exec("SELECT name FROM productinformation WHERE type='table' AND name= 'inventoryproduct'");
    if(tableExists && sql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
        sql.exec("create table inventoryproduct(itemnumber UNIQUE NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                 "itemprice NOT NULL,number NOT NULL,productdescription)");
    }
    // 获取编辑框中的文本
    QString itemnumber = ui->itemnumberEdit->text();
    QString itemname = ui->itemnameEdit->text();
    QString itemcategory = ui->itemcategoryEdit->text();
    QString itembrand = ui->itembrandEdit->text();
    QString itemsupplier = ui->itemsupplierEdit->text();
    QString itempriceStr = ui->itempriceEdit->text();
    QString numberStr = ui->InventoryquantityEdit->text();

    // 检查所有字段是否都不为空
    if (itemnumber.isEmpty() || itemname.isEmpty() || itemcategory.isEmpty() ||
        itembrand.isEmpty() || itemsupplier.isEmpty() || itempriceStr.isEmpty() || numberStr.isEmpty()) {
        QMessageBox::information(this,tr("提示"),tr("存在商品信息为空，无法添加"));
        db.close();
        issuccess=false;
        return;
    }

    if(!ID.isNull()&&ui->itemnumberEdit->text()==ID){
        // // 获取编辑框中的文本
        // QString itemnumber = ui->itemnumberEdit->text();
        // QString itemname = ui->itemnameEdit->text();
        // QString itemcategory = ui->itemcategoryEdit->text();
        // QString itembrand = ui->itembrandEdit->text();
        // QString itemsupplier = ui->itemsupplierEdit->text();
        // QString itempriceStr = ui->itempriceEdit->text();
        // QString numberStr = ui->InventoryquantityEdit->text();

        // // 检查所有字段是否都不为空
        // if (itemnumber.isEmpty() || itemname.isEmpty() || itemcategory.isEmpty() ||
        //     itembrand.isEmpty() || itemsupplier.isEmpty() || itempriceStr.isEmpty() || numberStr.isEmpty()) {
        //     QMessageBox::information(this,tr("提示"),tr("存在商品信息为空，无法添加"));
        //     db.close();
        //     issuccess=false;
        //     return;
        // }
        sql.exec("delete from inventoryproduct where itemnumber='"+ID+"'");
        if(!sql.exec("insert into inventoryproduct (itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                      "itemprice,number)"
                      " values('"+ui->itemnumberEdit->text()+"','"+ui->itemnameEdit->text()+"','"+ui->itemcategoryEdit->text()+"','"
                      +ui->itembrandEdit->text()+"','"+ui->itemsupplierEdit->text()+"',"+ui->itempriceEdit->text()+","
                      +ui->InventoryquantityEdit->text()+")"))
        {
            QMessageBox::information(this,tr("提示"),tr("存在商品信息为空，无法添加"));
            issuccess=false;
        }else{
            issuccess=true;
        }
    }else{

        bool isexist=false;//判断ID是否已存在，存在的话就只添加数量
        int val=0;
        sql.exec("select * from inventoryproduct");
        while(sql.next()){
            if(ui->itemnumberEdit->text()==sql.value("itemnumber").toString()){
                isexist=true;
                val=sql.value("number").toInt();
                // ui->itemnameEdit->setText(sql.value("itemname").toString());
                // ui->itemcategoryEdit->setText(sql.value("itemcategory").toString());
                // ui->itembrandEdit->setText(sql.value("itembrand").toString());
                // ui->itemsupplierEdit->setText(sql.value("itemsupplier").toString());
                // ui->itempriceEdit->setText(sql.value("itemprice").toString());
            }
        }
        if(isexist){
            if(sql.exec( "UPDATE inventoryproduct SET number = " + QString::number(val+ui->InventoryquantityEdit->text().toInt()) + " WHERE itemnumber = '" + ui->itemnumberEdit->text() + "'"))
            {
                issuccess=true;
            }else{
                issuccess=false;
            }

        }else{
            // itemNumber = ui->itemnumberEdit->text();
            // itemName = ui->itemnameEdit->text();
            // itemCategory = ui->itemcategoryEdit->text();
            // itemBrand = ui->itembrandEdit->text();
            // itemSupplier = ui->itemsupplierEdit->text();
            // itemPrice = ui->itempriceEdit->text().toInt();
            // inventoryQuantity = ui->InventoryquantityEdit->text().toInt();
            // 使用预编译语句
            // sql.prepare("INSERT INTO inventoryproduct (itemnumber, itemname, itemcategory, itembrand, itemsupplier, "
            //             "itemprice, number,productdescription) "
            //              "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
            // sql.addBindValue(itemNumber);
            // sql.addBindValue(itemName);
            // sql.addBindValue(itemCategory);
            // sql.addBindValue(itemBrand);
            // sql.addBindValue(itemSupplier);
            // sql.addBindValue(itemPrice);
            // sql.addBindValue(inventoryQuantity);
            // sql.addBindValue("");

            if(!sql.exec("insert into inventoryproduct (itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                         "itemprice,number)"
                         " values('"+ui->itemnumberEdit->text()+"','"+ui->itemnameEdit->text()+"','"+ui->itemcategoryEdit->text()+"','"
                         +ui->itembrandEdit->text()+"','"+ui->itemsupplierEdit->text()+"',"+ui->itempriceEdit->text()+","
                          +ui->InventoryquantityEdit->text()+")"))
            {
                QMessageBox::information(this,tr("提示"),tr("存在商品信息为空，无法添加"));
                issuccess=false;
            }else{
                issuccess=true;
            }
        }
    }
    db.close();
    if(issuccess){
        QMessageBox::information(this,tr("提示"),tr("保存成功"),
                                 QMessageBox::Ok);
        isrestore=true;
    }
    if(!ID.isNull()||!issuccess){
        return;
    }
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");

    if(sdb.open()){
        qDebug()<<"数据库打开成功xxxxx";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery qsql(sdb);
    tableExists = qsql.exec("SELECT name FROM record WHERE type='table' AND name='inputrecord'");
    if(tableExists && qsql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
        qsql.exec("create table inputrecord(itemnumber NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                  "itemprice NOT NULL,number NOT NULL,time)");
    }

    // 获取当前的日期和时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // 将当前的日期和时间转换为字符串，这里使用格式 "yyyy/MM/dd hh:mm:ss"
    QString formattedDateTime = currentDateTime.toString("yyyy/MM/dd hh:mm:ss");
    if(!qsql.exec("insert into inputrecord(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                   "itemprice,number,time)"
                   " values('"+ui->itemnumberEdit->text()+"','"+ui->itemnameEdit->text()+"','"+ui->itemcategoryEdit->text()+"','"
                   +ui->itembrandEdit->text()+"','"+ui->itemsupplierEdit->text()+"',"+ui->itempriceEdit->text()+","
                   +ui->InventoryquantityEdit->text()+",'"+formattedDateTime+"')"))
    {
        qDebug()<<"无法录入入库记录,有数据为空";
    }
    //录入进货记录
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
}

//关闭按钮功能重写
void InventoryproductChange::closeEvent(QCloseEvent*){
    reButtonAndClose_clicked();
}
//键盘事件重写
void InventoryproductChange::keyPressEvent(QKeyEvent*event){
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
                restoreButton_clicked();
            }
        }
    }
    event->accept();
}

//返回按钮和关闭功能
void InventoryproductChange::reButtonAndClose_clicked(){
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

//信号与槽
void InventoryproductChange::connections(){
    //返回与关闭按钮
    connect(ui->reButton,&QPushButton::clicked,
            this,&InventoryproductChange::reButtonAndClose_clicked);
    connect(ui->restoreButton,&QPushButton::clicked,
            this,&InventoryproductChange::restoreButton_clicked);
    //信号与槽连接成功
    qDebug()<<"信号与槽连接成功";
}
