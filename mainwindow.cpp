#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QToolButton>
#include"tools.h"
#include<QMessageBox>
#include"login.h"
#include"changepwd.h"
#include"userinfo.h"
#include"priductchange.h"
#include<QFileDialog>
#include <QInputDialog>
#include <QTimer>
#include <QValueAxis>
#include"xlsxdocument.h"
#include"delrecord.h"
#include"inventoryproductchange.h"
#include "record.h"
#include"information.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"这里1111";
    this->setFixedSize(1147,967);
    //初始界面
    ui->tabWidget->setCurrentWidget(ui->Home);
    qDebug()<<Tools::folderName;
    //初始化
    init();
    init_for_saleRecord();
    init_for_inventory();
    init_for_restockRecord();
    init_for_home();
    //连接数据库
    connectSql();
    connectSql_for_saleRecord();
    connectSql_for_Inventory();
    connectSql_for_restockRecord();
    //qDebug()<<"这里1111";
    //右上角用户按钮初始化
    menus_to_usertoolbutton();

    //连接
    connections();
}

//表格设置
void MainWindow::TableSet(QTableWidget*tableWidget){
    //单元格自适应大小
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // 设置所有单元格不可编辑
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableWidget->horizontalHeader()->setMinimumSectionSize(70); // 设置最小宽度为70像素
}
//连接数据库
void MainWindow::connectSql(){
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    if(db.open()){
        qDebug()<<"数据库打开成功ffff";
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
    sql.exec("select * from product");
    while(sql.next()){
        int row= ui->tableWidget->rowCount(); // 获取当前的行数，将在最后插入一行
        //qDebug()<<row;
        //表格新建一行
        ui->tableWidget->insertRow(row);
        Product pro(sql.value("itemnumber").toString(),sql.value("itemname").toString(),
                    sql.value("itemcategory").toString(),sql.value("itembrand").toString(),
                    sql.value("itemsupplier").toString(),sql.value("itemprice").toInt(),
                    sql.value("Inventoryquantity").toInt(),sql.value("productdescription").toString());
        //qDebug()<<sql.value("itemprice").toInt();
        //初始化
        productItem pi(pro);

        proitem.push_back(pi);
        //存储商品信息
        Tools::proitem.push_back(pi);
        //存储商品种类
        Tools::kinds.push_back(pro.getItemcategory());
        //qDebug()<<Tools::kinds.length();
        //插入数据在商品信息表格的指定行
        inserttoTable(&pi,row);
    }

    db.close();


    //在商品种类选择框中添加项目
    foreach(QString s,Tools::kinds){
        ui->comboBox->addItem(s);
    }
    qDebug()<<"连接结束";
}
//插入数据在商品信息表格的指定行
void MainWindow::inserttoTable(productItem*pi,int row){
    //每行中各个方格的数据
    QVector<QTableWidgetItem*>item;
    QTableWidgetItem *item1 = new QTableWidgetItem(pi->pro.getItemnumber());
    QTableWidgetItem *item2 = new QTableWidgetItem(pi->pro.getItemname());
    QTableWidgetItem *item3 = new QTableWidgetItem(pi->pro.getItemcategory());
    QTableWidgetItem *item4 = new QTableWidgetItem(pi->pro.getItembrand());
    QTableWidgetItem *item5 = new QTableWidgetItem(pi->pro.getItemsupplier());
    QTableWidgetItem *item6 = new QTableWidgetItem(QString::number(pi->pro.getItemprice()));
    QTableWidgetItem *item7 = new QTableWidgetItem(QString::number(pi->pro.getInventoryquantity()));
    QTableWidgetItem *item8 = new QTableWidgetItem(pi->pro.getProductdescription());
    QTableWidgetItem *item9 = new QTableWidgetItem("修改");
    QTableWidgetItem *item10 = new QTableWidgetItem("下架");
    QTableWidgetItem *item11 = new QTableWidgetItem("出售");
    // item9->setForeground(QBrush(QColor(Qt::blue)));
    // item10->setForeground(QBrush(QColor(Qt::red)));
    // item10->setForeground(QBrush(QColor(Qt::green)));
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
    item.push_back(item11);
    int col=0;
    //qDebug()<<"1111";
    foreach(QTableWidgetItem *i,item){
        ui->tableWidget->setItem(row,col,i);
        col++;
    }
    // ui->tableWidget->resizeRowsToContents();
    // ui->tableWidget->resizeColumnsToContents();
    //有两个按钮的方格
    // ButtonCell*buttoncell=new ButtonCell(pi,ui->tableWidget);
    // ui->tableWidget->setCellWidget(row,col,buttoncell);
    //添加按钮事件
    // connect(pi->change,&QPushButton::clicked,
    //         this,&MainWindow::windowsFlush_for_commodityDiag);
    // connect(pi->del,&QPushButton::clicked,
    //         this,&MainWindow::windowsFlush_for_commodityDiag);
    //设置行高
    // for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
    //     ui->tableWidget->setRowHeight(row, 30); // 设置行的高度为20像素
    // }
}
//界面刷新代码
void MainWindow::windowsFlush(){
    MainWindow*mw=new MainWindow;
    mw->show();
    this->hide();
    //this->update();
}
//刷新界面后主界面为商品信息界面
void MainWindow::windowsFlush_for_commodityDiag(){
    //清除已有有关表格的数据
    Tools::proitem.clear();
    Tools::kinds.clear();
    Tools::kinds.push_back("全部");
    ui->comboBox->clear();
    proitem.clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    connectSql();
}
//商品信息单元格点击事件
void MainWindow::item_clicked(QTableWidgetItem *item){
    // 获取被点击的单元格的行和列
    int row = item->row();
    int column = item->column();
    if(column==8){
        //修改操作实现
        productItem prod=proitem.at(row);
        PriductChange diag(prod.pro);
        diag.exec();
        // (new MainWindow)->show();
        // this->hide();
        this->windowsFlush_for_commodityDiag();
    }else if(column==9){
        //删除操作实现
        qDebug()<<"删除操作执行";
        productItem prod=proitem.at(row);
        int result=QMessageBox::information(this,tr("警告"),tr("确定下架吗")
                                          ,QMessageBox::Yes,QMessageBox::No);
        qDebug()<<result<<":"<<QMessageBox::Yes;
        if(result==QMessageBox::Yes){
            QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
            if(sdb.open()){
                qDebug()<<"数据库打开成功cccc";
            }else{
                qDebug()<<"数据库打开失败aaaa";
            }
            QSqlQuery sql(sdb);
            //qDebug()<<prod.pro.getItemnumber();
            sql.exec("delete from product where "
                     "itemnumber='"+prod.pro.getItemnumber()+"'");
            //创建新表来存储删除记录
             bool tableExists = sql.exec("SELECT name FROM productinfomation WHERE type='table' AND name='delproduct'");
             if(tableExists && sql.next()) {
                 // 表已存在，不执行创建操作
             } else {
                 // 表不存在，执行创建操作
            sql.exec("create table delproduct(itemnumber UNIQUE NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
            "itemprice NOT NULL,Inventoryquantity NOT NULL,productdescription,deltime)");
            }

            // 获取当前的日期和时间
            QDateTime currentDateTime = QDateTime::currentDateTime();

            // 将当前的日期和时间转换为字符串，这里使用预定义的格式 "yyyy/MM/dd hh:mm:ss"
            QString formattedDateTime = currentDateTime.toString("yyyy/MM/dd hh:mm:ss");

            if(!sql.exec("insert into delproduct(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                     "itemprice,Inventoryquantity,productdescription,deltime)"
                     " values('"+prod.pro.getItemnumber()+"','"+prod.pro.getItemname()+"','"+prod.pro.getItemcategory()+"','"
                     +prod.pro.getItembrand()+"','"+prod.pro.getItemsupplier()+"',"+QString::number(prod.pro.getItemprice())+","
                         +QString::number(prod.pro.getInventoryquantity())+",'"+prod.pro.getProductdescription()+"','"+formattedDateTime+"')"))
            {
                qDebug()<<"无法录入删除记录";
            }
            sdb.close();

            Tools::delrecord.push_back(prod.pro);


            // Tools::proitem.clear();
            // Tools::kinds.clear();
            // (new MainWindow)->show();
            // this->hide();
            this->windowsFlush_for_commodityDiag();
        }
    }
    else if(column==10){
        bool ok;
        int minValue = 0;       // 最小值
        int maxValue = ui->tableWidget->item(row,6)->text().toInt();// 最大值
        int defaultValue = 1;  // 默认值
        int step = 1;           // 步长
        QString id=ui->tableWidget->item(row,0)->text();//商品编号
        int value = QInputDialog::getInt(this, tr("出售数量"),
                                         tr("请输入出售的数量:"),
                                         defaultValue, minValue, maxValue, step, &ok);

        if (ok) {
            //提示消息
            if(value!=0){
                QMessageBox::information(this,tr("提示"),tr("出售成功"));
            }
             QSqlDatabase qdb=QSqlDatabase::addDatabase("QSQLITE");
            //修改表格数据
            qdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
            if(qdb.open()){
                qDebug()<<"数据库打开成功xxxxx";
            }else{
                qDebug()<<"数据库打开失败";
                return;
            }
            QSqlQuery sql(qdb);
            //修改出售数量
            sql.exec( "UPDATE product SET Inventoryquantity = " + QString::number(maxValue - value) + " WHERE itemnumber = '" + id + "'");
            qdb.close();

            //记录出售记录
            this->windowsFlush_for_commodityDiag();
            if(value==0){
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
            bool tableExists = qsql.exec("SELECT name FROM record WHERE type='table' AND name='salerecord'");
            if(tableExists && qsql.next()) {
                // 表已存在，不执行创建操作
            } else {
                // 表不存在，执行创建操作
                qsql.exec("create table salerecord(itemnumber NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                         "itemprice NOT NULL,salenumber NOT NULL,productdescription,saletime)");
            }

            // 获取当前的日期和时间
            QDateTime currentDateTime = QDateTime::currentDateTime();

            // 将当前的日期和时间转换为字符串，这里使用格式 "yyyy/MM/dd hh:mm:ss"
            QString formattedDateTime = currentDateTime.toString("yyyy/MM/dd hh:mm:ss");
            //qDebug()<<ui->tableWidget->item(row,0)->text();
            if(!qsql.exec("insert into salerecord(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                          "itemprice,salenumber,productdescription,saletime)"
                          " values('"+ui->tableWidget->item(row,0)->text()+"','"+ui->tableWidget->item(row,1)->text()+"','"+ui->tableWidget->item(row,2)->text()+"','"
                          +ui->tableWidget->item(row,3)->text()+"','"+ui->tableWidget->item(row,4)->text()+"',"+ui->tableWidget->item(row,5)->text()+","
                          +QString::number(value)+",'"+ui->tableWidget->item(row,7)->text()+"','"+formattedDateTime+"')"))
            {
                qDebug()<<"无法录入出售记录,有数据为空";
            }
            sdb.close();


        } else {

        }
    }
    else{
        QMessageBox::information(this,tr("内容"),item->text());
    }
}
//初始化
void MainWindow::init(){

    //初始化计时器
    time=new QTimer(this);
    // 设置定时器的间隔时间（一分钟）
    time->start(60 * 1000);
    //首页
    homepage=ui->tabWidget->widget(1);
    //商品信息页
    commoditypage=ui->tabWidget->widget(0);
    //库存管理页
    inventoryquantitypage=ui->tabWidget->widget(2);
    //进货记录页
    purchaserecordpage=ui->tabWidget->widget(3);
    //销售记录页
    salerecordpage=ui->tabWidget->widget(4);

    //商品信息页表格初始化
    // 使列宽自适应内容
    // ui->tableWidget->resizeColumnsToContents();
    // ui->tableWidget->resizeRowsToContents();
    TableSet(ui->tableWidget);
    //工具菜单按钮初始化
    addProductAction=new QAction("添加商品",this);
    delProductAction=new QAction("下架记录",this);
    restoreDatatoCsv=new QAction("导出表格",this);

    toolMenu=new QMenu(this);
    toolMenu->addAction(addProductAction);
    toolMenu->addAction(delProductAction);
    toolMenu->addAction(restoreDatatoCsv);

    ui->toolButton->setMenu(toolMenu);
    //设置打开模式
    ui->toolButton->setPopupMode(QToolButton::InstantPopup);
}
//信号与槽功能实现
void MainWindow::connections(){
    //退出功能
    connect(this->exitlogin,&QAction::triggered,
            this,&MainWindow::exitAction_clicked);
    //修改密码功能
    connect(this->changepwd,&QAction::triggered,
            this,&MainWindow::changepwdAction_clicked);
    //个人资料功能
    connect(this->userinfo,&QAction::triggered,
            this,&MainWindow::userinfoAction_clicked);
    //切换页面功能
    connect(ui->homeButton,&QPushButton::clicked,
            this,&MainWindow::homepageButton_clicked);
    connect(ui->CommodityinformationButton,&QPushButton::clicked,
            this,&MainWindow::commoditypageButton_clicked);
    connect(ui->InventoryquantityButton,&QPushButton::clicked,
            this,&MainWindow::inventoryquantitypageButton_clicked);
    connect(ui->PurchaserecordButton,&QPushButton::clicked,
            this,&MainWindow::purchasepageButton_clicked);
    connect(ui->SalerecordButton,&QPushButton::clicked,
            this,&MainWindow::salerecordpageButton_clicked);
    //添加商品功能
    connect(this->addProductAction,&QAction::triggered,
            this,&MainWindow::addproductButton_clicked);
    //导出表格功能
    connect(this->restoreDatatoCsv,&QAction::triggered,
            this,&MainWindow::DatatoXlsx);
    //表格点击事件
    connect(ui->tableWidget, &QTableWidget::itemClicked,
            this, &MainWindow::item_clicked);
    //搜索按钮功能
    connect(ui->searchButton,&QPushButton::clicked,
            this,&MainWindow::SearchForCondition);
    //删除按钮功能
    connect(this->delProductAction,&QAction::triggered,
            this,&MainWindow::delRecord);
    //清空记录功能
    connect(this->dataClear,&QAction::triggered,
            this,&MainWindow::DataClear);
    //出售记录导出表格
    connect(this->restoreDatatoxlsx,&QAction::triggered,
            this,&MainWindow::DatatoXlsx_for_salerecord);
    //搜索按钮
    connect(ui->searchButton_5,&QPushButton::clicked,
            this,&MainWindow::SearchForCondition_forsaleRecord);
    //出售记录表格点击事件
    connect(ui->tableWidget_3,&QTableWidget::itemClicked,
            this, &MainWindow::item_clicked_for_saletable);

    //出售记录显示图表
    connect(dataforgraph,&QAction::triggered,
            this,&MainWindow::datatograph);
    //出售下载图表
    connect(downgraph,&QAction::triggered,
            this,&MainWindow::downloadGraph);

    //库存界面搜索按钮
    connect(ui->searchButton_2,&QPushButton::clicked,
            this,&MainWindow::SearchForCondition_for_inventory);
    //表格按钮
    connect(ui->tableWidget_2,&QTableWidget::itemClicked,
            this, &MainWindow::item_clicked_for_Inventory);
    //入库
    connect(inputstorage,&QAction::triggered,
            this,&MainWindow::inputAction);
    //出库记录
    connect(outputrecord,&QAction::triggered,
            this,&MainWindow::outputRecord);
    //入库记录
    connect(inputrecord,&QAction::triggered,
            this,&MainWindow::inputRecord);
    //预警阈值
    connect(WarningThreshold1,&QAction::triggered,
            this,&MainWindow::setWarningThreshold1);
    connect(WarningThreshold2,&QAction::triggered,
            this,&MainWindow::setWarningThreshold2);
    //库存界面导出表格
    connect(datatoxlsx,&QAction::triggered,
            this,&MainWindow::DatatoXlsx_for_inventory);
    //库存界面清空数据
    connect(clearAction,&QAction::triggered,
            this,&MainWindow::DataClear_for_inventory);
    //进货记录界面搜索按钮
    connect(ui->searchButton_7,&QPushButton::clicked,
            this,&MainWindow::SearchForCondition_for_restockRecord);
    //进货界面导出表格
    connect(Datatoxlsx,&QAction::triggered,
            this,&MainWindow::DatatoXlsx_for_restockRecord);
    //进货界面清空数据
    connect(Cleardata,&QAction::triggered,
            this,&MainWindow::DataClear_for_restockDiag);
    //进货界面显示图表
    connect(datagraph,&QAction::triggered,
            this,&MainWindow::datatograph_for_restockDiag);
    //进货界面下载图表
    connect(graphdownload,&QAction::triggered,
            this,&MainWindow::downloadGraph_for_restockDiag);

    //进货记录界面表格事件
    connect(ui->tableWidget_7,&QTableWidget::itemClicked,
            this, &MainWindow::item_clicked_for_restocktable);
    //总销售额按钮
    connect(ui->salevolumeButton,&QPushButton::clicked,
            this,&MainWindow::salevolumeButton_clicked);
    //总成本按钮
    connect(ui->costButton,&QPushButton::clicked,
            this,&MainWindow::costButton_clicked);
    //总利润按钮
    connect(ui->profitButton,&QPushButton::clicked,
            this,&MainWindow::profitButton_clicked);
    //预警消息按钮
    connect(ui->informationButton,&QPushButton::clicked,
            this,&MainWindow::informationButton_clicked);
    //两个下载按钮
    connect(ui->downloadButton1,&QPushButton::clicked,
            this,&MainWindow::downloadButton1_clicked);
    connect(ui->downloadButton2,&QPushButton::clicked,
            this,&MainWindow::downloadButton2_clicked);
    //计时器事件
    connect(time, &QTimer::timeout, this, &MainWindow::init_for_home);
}
//退出登录功能实现
void MainWindow::exitAction_clicked(){
    //消息框，询问用户是否真的退出
    int result=QMessageBox::information(this,tr("提示"),tr("确定退出登录吗？？？"),QMessageBox::Yes,QMessageBox::No);
    if(result==QMessageBox::Yes){
        Tools::username="username";
        Tools::password="000000";
        Tools::userimage=":/images/images/user.png";
        Tools::birthday=NULL;
        Tools::phonenumber=NULL;
        Tools::emailnumber=NULL;
        Tools::sex=NULL;
        Tools::selfedit=NULL;

        //重新登陆界面
        (new Login())->show();
        this->hide();
    }else{
        //直接默认功能
    }
}
//修改密码功能实现
void MainWindow::changepwdAction_clicked(){
    ChangePwd cp;
    cp.setModal(true);
    if(cp.exec()==QDialog::Accepted){
        this->hide();
    }
}
//个人资料功能实现
void MainWindow::userinfoAction_clicked(){
    //类名和变量名不小心重名了
    class userinfo usif(this);
    //usif.exec();
    if(usif.exec()==QDialog::Rejected){
        this->close();
    }
    this->user->setStyleSheet("QToolButton {"
                              "border-image: url("+Tools::userimage+") 0 0 0 0 stretch stretch;"
                                                   "border-radius: 43px;"
                                                   "}");

}
//向windows中添加QToolButton-user,并将其与usermenu相关联，添加了三个QAction
void MainWindow::menus_to_usertoolbutton(){
    user=new QToolButton(this);
    user->setPopupMode(QToolButton::InstantPopup);
    user->setGeometry(1072,0,70,50);
    //user->setIcon(QIcon(Tools::userimage));
    user->setStyleSheet("QToolButton {"
                        "border-image: url("+Tools::userimage+") 0 0 0 0 stretch stretch;"
                        "border-radius: 43px;"
                        "}");

    //user->setText(Tools::username);
    //user->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);// 文字和图标并排
    //设置图片大小
    user->setIconSize(QSize(50,50));
    //设置字号
    QFont font;
    font.setPointSize(12);
    user->setFont(font);
    //初始化数据
    userinfo=new QAction("个人资料",this);
    changepwd=new QAction("修改密码",this);
    exitlogin=new QAction("退出登录",this);
    //将三个QAction对象加到usermenu中
    usermenu=new QMenu(this);
    usermenu->addAction(userinfo);
    usermenu->addAction(changepwd);
    usermenu->addAction(exitlogin);
    //将usermenu与user按钮绑定
    user->setMenu(usermenu);
}
//切换首页按钮实现
void MainWindow::homepageButton_clicked(){
    ui->tabWidget->setCurrentIndex(1);
    this->windowFlush_for_home();
}
//切换商品信息页按钮实现
void MainWindow::commoditypageButton_clicked(){
    ui->tabWidget->setCurrentIndex(0);
    this->windowsFlush_for_commodityDiag();
}
//切换库存管理页按钮实现
void MainWindow::inventoryquantitypageButton_clicked(){
    static bool right=true;
    ui->tabWidget->setCurrentIndex(2);
    if(right){
        foreach(QString s,ID1){
            QMessageBox::warning(this,tr("预警"),tr("库存ID为")+s+tr("的库存商品存量过低，请及时补充"));
        }
        foreach(QString s,ID2){
            QMessageBox::warning(this,tr("预警"),tr("库存ID为")+s+tr("的库存商品存量过高，建议适当降价出售"));
        }
        right=false;
    }

    this->windowsFlush_for_inventoryDiag();
}
//切换进货记录页按钮实现
void MainWindow::purchasepageButton_clicked(){
    ui->tabWidget->setCurrentIndex(3);
    this->windowsFlush_for_restockDiag();
}
//切换出售记录页按钮实现
void MainWindow::salerecordpageButton_clicked(){
    ui->tabWidget->setCurrentIndex(4);
    this->windowsFlush_for_salerecordDiag();
}

//添加商品按钮实现
void MainWindow::addproductButton_clicked(){
    //商品信息录入界面
    PriductChange prodiag(this);
    if(prodiag.exec()==QDialog::Accepted){
        // MainWindow*mw=new MainWindow;
        // mw->ui->tabWidget->setCurrentIndex(0);
        // Tools::kinds.clear();
        // Tools::proitem.clear();
        // mw->show();
        // this->hide();
        this->windowsFlush_for_commodityDiag();
    }
}
//根据搜索选项挑选符合条件的商品
void MainWindow::SearchForCondition(){
    // //如果条件都为空则不进行任何操作
    // if(ui->comboBox->currentText().isEmpty()&&ui->namelineEdit->text().isEmpty()&&ui->numberlineEdit->text().isEmpty()){
    //     return;
    // }
    //清空商品信息
    proitem.clear();
    //清空表格数据
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    //与数据库建立连接
    if(db.open()){
        qDebug()<<"数据库打开成功11111";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    //符合条件的数目
    int count=0;
    QSqlQuery sql(db);
    sql.exec("select * from product");
    while(sql.next()){
        if((ui->comboBox->currentText()=="全部"?true:sql.value("itemcategory").toString()==ui->comboBox->currentText())&&
            (ui->namelineEdit->text().isEmpty()?true:sql.value("itemname").toString()==ui->namelineEdit->text())&&
            (ui->numberlineEdit->text().isEmpty()?true:sql.value("itemnumber").toString()==ui->numberlineEdit->text())){
            int row= ui->tableWidget->rowCount(); // 获取当前的行数，将在最后插入一行
            //表格新建一行
            ui->tableWidget->insertRow(row);
            Product pro(sql.value("itemnumber").toString(),sql.value("itemname").toString(),
                        sql.value("itemcategory").toString(),sql.value("itembrand").toString(),
                        sql.value("itemsupplier").toString(),sql.value("itemprice").toInt(),
                        sql.value("Inventoryquantity").toInt(),sql.value("productdescription").toString());
            productItem pi(pro);
            proitem.push_back(pi);
            inserttoTable(&pi,row);
            count++;
        }
    }
    db.close();
    if(count==0){
        QMessageBox::information(this,tr("提示"),tr("没有符合条件的商品"));
    }
}
//删除记录功能
void MainWindow::delRecord(){
    DelRecord delrecord;
    //显示窗口
    delrecord.exec();
    //刷新表格
    this->windowsFlush_for_commodityDiag();
}
//存储信息到xlsx表格文件
 void MainWindow::DatatoXlsx(){
    //操作表格对象，第三方库
    //存入数据
    QVector<QString>head={"商品编号","商品名称","商品类别","品牌",
                             "供应商","单价","库存数量","商品描述"};
    QXlsx::Document xlsx;
    int col=1;
    foreach(QString s,head){
        xlsx.write(1,col,s);
        col++;
    }
    int row=2;
    foreach(productItem p,Tools::proitem){
        Product pro=p.pro;
        xlsx.write(row,1,pro.getItemnumber());
        xlsx.write(row,2,pro.getItemname());
        xlsx.write(row,3,pro.getItemcategory());
        xlsx.write(row,4,pro.getItembrand());
        xlsx.write(row,5,pro.getItemsupplier());
        xlsx.write(row,6,pro.getItemprice());
        xlsx.write(row,7,pro.getInventoryquantity());
        xlsx.write(row,8,pro.getProductdescription());
        row++;
    }
    qDebug()<<"到这了";
    //保存文件夹位置
    QString dirpath=QFileDialog::getExistingDirectory(this,"选择文件保存路径","D:/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QDir filepath(dirpath);
    if(!filepath.exists()){
        //文件路径不存在
        QMessageBox::warning(this,tr("错误"),tr("导出失败，文件夹不存在"));
        return;
    }
    //文件路径存在
    //输入文件名
    QString filename=QInputDialog::getText(this,"输入文件名","请输入文件名（不用加后缀，指定.xlsx）");
    if(filename.isEmpty()){
        QMessageBox::warning(this,tr("错误"),tr("文件名不能为空"));
        return;
    }
    //文件绝对路径
    QString fileabsoluteoath=filepath.absoluteFilePath(filename+".xlsx");
    QFile file(fileabsoluteoath);
    if (file.open(QIODevice::WriteOnly))
    {
        if (!xlsx.saveAs(&file))
        {
            qDebug() << "文件导出失败";
        }
        else
        {
            qDebug() << "文件导出成功";
        }
        file.close();
    }else
    {
        qDebug() << "打开失败";
    }
}
 //下面为出售记录页面初始化
//连接数据库导入表格
void MainWindow::connectSql_for_saleRecord(){
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    bool tableExists = sql.exec("SELECT name FROM record WHERE type='table' AND name='salerecord'");
    if(tableExists && sql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
        sql.exec("create table salerecord(itemnumber NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                  "itemprice NOT NULL,salenumber NOT NULL,productdescription,saletime)");
    }
    sql.exec("select * from salerecord");
    while(sql.next()){
        int row= ui->tableWidget_3->rowCount(); // 获取当前的行数，将在最后插入一行
        //qDebug()<<row;
        //表格新建一行
        ui->tableWidget_3->insertRow(row);
        //每行中各个方格的数据
        QVector<QTableWidgetItem*>item;
        QTableWidgetItem *item1 = new QTableWidgetItem(sql.value("itemnumber").toString());
        QTableWidgetItem *item2 = new QTableWidgetItem(sql.value("itemname").toString());
        QTableWidgetItem *item3 = new QTableWidgetItem(sql.value("itemcategory").toString());
        QTableWidgetItem *item4 = new QTableWidgetItem(sql.value("itembrand").toString());
        //QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemsupplier").toString());
        QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemprice").toString());
        QTableWidgetItem *item6 = new QTableWidgetItem(sql.value("salenumber").toString());
        QTableWidgetItem *item7 = new QTableWidgetItem(QString::number((sql.value("itemprice").toInt())*(sql.value("salenumber").toInt())));
        QTableWidgetItem *item8 = new QTableWidgetItem(sql.value("saletime").toString());
        QTableWidgetItem *item9 = new QTableWidgetItem("删除");
        qDebug()<<QString::number((sql.value("itemprice").toInt())*(sql.value("salenumber").toInt()));
        item.push_back(item1);
        item.push_back(item2);
        item.push_back(item3);
        item.push_back(item4);
        item.push_back(item5);
        item.push_back(item6);
        item.push_back(item7);
        item.push_back(item8);
        item.push_back(item9);
        int col=0;
        //qDebug()<<"1111";
        foreach(QTableWidgetItem *i,item){
            ui->tableWidget_3->setItem(row,col,i);
            col++;
        }
    }

    sdb.close();
    //右下角
    ui->number->setText("共"+QString::number(ui->tableWidget_3->rowCount())+"条");
}

//出售记录页的初始化
void MainWindow::init_for_saleRecord(){
    //工具菜单按钮初始化
    restoreDatatoxlsx=new QAction("导出表格",this);
    dataClear=new QAction("清空数据",this);
    dataforgraph=new QAction("显示图表",this);
    downgraph=new QAction("下载图表",this);
    toolMenu_5=new QMenu(this);
    toolMenu_5->addAction(restoreDatatoxlsx);
    toolMenu_5->addAction(dataforgraph);
    toolMenu_5->addAction(downgraph);
    toolMenu_5->addAction(dataClear);

    ui->toolButton_5->setMenu(toolMenu_5);
    //设置打开模式
    ui->toolButton_5->setPopupMode(QToolButton::InstantPopup);

    TableSet(ui->tableWidget_3);
}
//方格点击事件
void MainWindow::item_clicked_for_saletable(QTableWidgetItem*item){
    // 获取被点击的单元格的行和列
    int row = item->row();
    int column = item->column();
    if(column==8){
        //删除操作
        int result=QMessageBox::information(this,tr("警告"),tr("确定删除吗")
                                              ,QMessageBox::Yes,QMessageBox::No);
        qDebug()<<result<<":"<<QMessageBox::Yes;
        if(result==QMessageBox::Yes){
            QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
            if(sdb.open()){
                qDebug()<<"数据库打开成功cccc";
            }else{
                qDebug()<<"数据库打开失败aaaa";
            }
            QSqlQuery sql(sdb);
            sql.exec("delete from salerecord where "
                     "saletime='"+ui->tableWidget_3->item(row,7)->text()+"'");
            sdb.close();
            this->windowsFlush_for_salerecordDiag();
        }
    }else{
        //显示内容
        QMessageBox::information(this,tr("内容"),item->text());
    }
}

//出售记录界面表格重建
void MainWindow::windowsFlush_for_salerecordDiag(){
    ui->tableWidget_3->clearContents();
    ui->tableWidget_3->setRowCount(0);
    connectSql_for_saleRecord();
}
//出售界面搜索按钮事件
void MainWindow::SearchForCondition_forsaleRecord(){
    //清空表格数据
    ui->tableWidget_3->clearContents();
    ui->tableWidget_3->setRowCount(0);
    //与数据库建立连接
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    //符合条件的数目
    int count=0;
    sql.exec("select * from salerecord");
    while(sql.next()){
        if((ui->timelineEdit->text().isEmpty()?true:isvaild(ui->timelineEdit->text().trimmed(),sql.value("saletime").toString()))&&
            (ui->namelineEdit_5->text().isEmpty()?true:sql.value("itemname").toString()==ui->namelineEdit_5->text())&&
            (ui->numberlineEdit_5->text().isEmpty()?true:sql.value("itemnumber").toString()==ui->numberlineEdit_5->text())){
            int row= ui->tableWidget_3->rowCount(); // 获取当前的行数，将在最后插入一行
            //qDebug()<<row;
            //表格新建一行
            ui->tableWidget_3->insertRow(row);
            //每行中各个方格的数据
            QVector<QTableWidgetItem*>item;
            QTableWidgetItem *item1 = new QTableWidgetItem(sql.value("itemnumber").toString());
            QTableWidgetItem *item2 = new QTableWidgetItem(sql.value("itemname").toString());
            QTableWidgetItem *item3 = new QTableWidgetItem(sql.value("itemcategory").toString());
            QTableWidgetItem *item4 = new QTableWidgetItem(sql.value("itembrand").toString());
            //QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemsupplier").toString());
            QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemprice").toString());
            QTableWidgetItem *item6 = new QTableWidgetItem(sql.value("salenumber").toString());
            QTableWidgetItem *item7 = new QTableWidgetItem(QString::number((sql.value("itemprice").toInt())*(sql.value("salenumber").toInt())));
            QTableWidgetItem *item8 = new QTableWidgetItem(sql.value("saletime").toString());
            QTableWidgetItem *item9 = new QTableWidgetItem("删除");
            qDebug()<<QString::number((sql.value("itemprice").toInt())*(sql.value("salenumber").toInt()));
            item.push_back(item1);
            item.push_back(item2);
            item.push_back(item3);
            item.push_back(item4);
            item.push_back(item5);
            item.push_back(item6);
            item.push_back(item7);
            item.push_back(item8);
            item.push_back(item9);
            int col=0;
            //qDebug()<<"1111";
            foreach(QTableWidgetItem *i,item){
                ui->tableWidget_3->setItem(row,col,i);
                col++;
            }
            count++;
        }
    }
    sdb.close();
    //右下角
    ui->number->setText("共"+QString::number(ui->tableWidget_3->rowCount())+"条");
    if(count==0){
        QMessageBox::information(this,tr("提示"),tr("没有符合条件的出售记录"));
    }
}
//检测时间搜索
bool MainWindow::isvaild(QString inputtime,QString time){
    if(inputtime.split(" ").length()==1){
        return inputtime==time.split(" ").at(0);
    }else if(inputtime.split(" ").length()==2){
        qDebug()<<inputtime<<" "<<time;
        return inputtime==time;
    }else{
        return false;
    }
}
//导出表格功能
void MainWindow::DatatoXlsx_for_salerecord(){
    //操作表格对象，第三方库
    //存入数据

    QVector<QString>head={"商品编号","商品名称","商品类别","品牌",
                           "单价","出售数量","总金额","时间"};
    QXlsx::Document xlsx;
    int col=1;
    foreach(QString s,head){
        xlsx.write(1,col,s);
        col++;
    }
    int row=2;
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    sql.exec("select * from salerecord");
    while(sql.next()){
        xlsx.write(row,1,sql.value(0).toString());
        xlsx.write(row,2,sql.value(1).toString());
        xlsx.write(row,3,sql.value(2).toString());
        xlsx.write(row,4,sql.value(3).toString());
        xlsx.write(row,5,sql.value(5).toString());
        xlsx.write(row,6,sql.value(6).toString());
        xlsx.write(row,7,sql.value(5).toInt()*sql.value(6).toInt());
        xlsx.write(row,8,sql.value(8).toString());
        row++;
    }
    //保存文件夹位置
    QString dirpath=QFileDialog::getExistingDirectory(this,"选择文件保存路径","D:/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QDir filepath(dirpath);
    if(!filepath.exists()){
        //文件路径不存在
        QMessageBox::warning(this,tr("错误"),tr("导出失败，文件夹不存在"));
        return;
    }
    //文件路径存在
    //输入文件名
    QString filename=QInputDialog::getText(this,"输入文件名","请输入文件名（不用加后缀，指定.xlsx）");
    if(filename.isEmpty()){
        QMessageBox::warning(this,tr("错误"),tr("文件名不能为空"));
        return;
    }
    //文件绝对路径
    QString fileabsoluteoath=filepath.absoluteFilePath(filename+".xlsx");
    QFile file(fileabsoluteoath);
    if (file.open(QIODevice::WriteOnly))
    {
        if (!xlsx.saveAs(&file))
        {
            qDebug() << "文件导出失败";
        }
        else
        {
            qDebug() << "文件导出成功";
        }
        file.close();
    }else
    {
        qDebug() << "打开失败";
    }
}
//显示图表
void MainWindow::datatograph(){
    salerecordgraph=new Graph(this);
    salerecordgraph->salerecord_graph_show();
    salerecordgraph->show();
}
//全部清除功能
void MainWindow::DataClear(){
    //删除操作
    int result=QMessageBox::information(this,tr("警告"),tr("确定清空数据吗")
                                          ,QMessageBox::Yes,QMessageBox::No);
    if(result==QMessageBox::Yes){
        QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
        if(sdb.open()){
            qDebug()<<"数据库打开成功cccc";
        }else{
            qDebug()<<"数据库打开失败aaaa";
        }
        QSqlQuery sql(sdb);
        sql.exec("delete from salerecord");
        sdb.close();
        this->windowsFlush_for_salerecordDiag();
    }
}
//下载图表
void MainWindow::downloadGraph(){
    salerecordgraph=new Graph(this);
    salerecordgraph->salerecord_graph_show();

    salerecordgraph->download_information();
}

//以下为库存界面功能
//初始化
void MainWindow::init_for_inventory(){
    //工具菜单按钮初始化
    datatoxlsx=new QAction("导出表格",this);
    inputrecord=new QAction("入库记录",this);
    outputrecord=new QAction("出库记录",this);
    inputstorage=new QAction("入库",this);
    clearAction=new QAction("清空数据",this);
    WarningThreshold1=new QAction("预警阈值（小）",this);
    WarningThreshold2=new QAction("预警阈值（大）",this);
    toolMenu_2=new QMenu(this);

    toolMenu_2->addAction(datatoxlsx);
    toolMenu_2->addAction(inputstorage);
    toolMenu_2->addAction(inputrecord);
    toolMenu_2->addAction(outputrecord);
    toolMenu_2->addAction(clearAction);
    toolMenu_2->addAction(WarningThreshold1);
    toolMenu_2->addAction(WarningThreshold2);
    ui->toolButton_2->setMenu(toolMenu_2);
    //设置打开模式
    ui->toolButton_2->setPopupMode(QToolButton::InstantPopup);

    TableSet(ui->tableWidget_2);

}

//预警阈值（小）设置
void MainWindow::setWarningThreshold1(){
    bool ok=true;
    int value=QInputDialog::getInt(this,tr("预警阈值（小）设置"),tr("输入要设置预警阈值（小）"),0,0,INT_MAX,ok);
    if(ok){
        QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/val.db");
        if(sdb.open()){
            qDebug()<<"数据库打开成功";
        }else{
            qDebug()<<"数据库打开失败";
            return;
        }
        QSqlQuery sql(sdb);
        bool tableExists = sql.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='WarningThreshold'");
        if(tableExists && sql.next()) {
            // 表已存在，不执行创建操作
        } else {
            // 表不存在，执行创建操作
            sql.exec("create table WarningThreshold(val1 INTEGER,val2 INTEGER)");
            sql.exec("insert into WarningThreshold(val1,val2) values(0,"+QString::number(INT_MAX)+")");
        }
        sql.exec("select * from WarningThreshold");
        int val2=INT_MAX;
        while(sql.next()){
            val2=sql.value("val2").toInt();
            //qDebug()<<sql.value("val2").toInt();
        }
        if(value>=val2){
            QMessageBox::information(this,tr("提示"),tr("预警阈值（小）不能比预警阈值（大）大"));
            sdb.close();
            return;
        }
        if(sql.exec("update WarningThreshold set val1="+QString::number(value))&&
            (sql.exec("update WarningThreshold set val2="+QString::number(val2)))){
            QMessageBox::information(this,tr("提示"),tr("设置成功"));
        }else{
            QMessageBox::information(this,tr("提示"),tr("出现故障，设置失败"));
        }
        sdb.close();
    }else{
        QMessageBox::information(this,tr("提示"),tr("出现故障，设置失败"));
    }
}
//预警阈值（大）设置
void MainWindow::setWarningThreshold2(){
    bool ok=true;
    int value=QInputDialog::getInt(this,tr("预警阈值（大）设置"),tr("输入要设置预警阈值（大）"),0,0,INT_MAX,ok);
    if(ok){
        QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName(Tools::projectPath + "/database/" + Tools::folderName + "/val.db");

        if (sdb.open()) {
            qDebug() << "数据库打开成功";
        } else {
            qDebug() << "数据库打开失败";
            return; // 如果数据库打开失败，直接返回，避免后续操作
        }

        QSqlQuery sql(sdb);

        // 检查 WarningThreshold 表是否存在
        bool tableExists = sql.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='WarningThreshold'");
        if (tableExists && sql.next()) {
            // 表已存在，不执行创建操作
            qDebug() << "表已存在";
        } else {
            // 表不存在，执行创建操作
            qDebug() << "表不存在，创建新表";
            sql.exec("CREATE TABLE WarningThreshold (val1 INTEGER, val2 INTEGER)");
            sql.exec("INSERT INTO WarningThreshold (val1, val2) VALUES (0, " + QString::number(INT_MAX) + ")");
        }

        sql.exec("SELECT * FROM WarningThreshold");
        int val1 = 0;
        while (sql.next()) {
            val1 = sql.value("val1").toInt();
        }

        if (value <= val1) {
            QMessageBox::information(this, tr("提示"), tr("预警阈值（大）不能比预警阈值（小）小"));
            sdb.close();
            return;
        }

        if (sql.exec("UPDATE WarningThreshold SET val2 = " + QString::number(value)) &&
            sql.exec("UPDATE WarningThreshold SET val1 = " + QString::number(val1))) {
            QMessageBox::information(this, tr("提示"), tr("设置成功"));
        } else {
            QMessageBox::information(this, tr("提示"), tr("出现故障，设置失败"));
        }

        sdb.close();
    }else{
        QMessageBox::information(this,tr("提示"),tr("出现故障，设置失败"));
    }
}
//全部清除功能
void MainWindow::DataClear_for_inventory(){
    //删除操作
    int result=QMessageBox::information(this,tr("警告"),tr("确定清空数据吗")
                                          ,QMessageBox::Yes,QMessageBox::No);
    if(result==QMessageBox::Yes){
        QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
        if(sdb.open()){
            qDebug()<<"数据库打开成功cccc";
        }else{
            qDebug()<<"数据库打开失败aaaa";
        }
        QSqlQuery sql(sdb);
        sql.exec("delete from inventoryproduct");
        sdb.close();
        this->windowsFlush_for_inventoryDiag();
    }
}
//库存界面表格重建
void MainWindow::windowsFlush_for_inventoryDiag(){
    Tools::inventorykinds.clear();
    Tools::inventorykinds.insert("全部");
    ui->comboBox_2->clear();
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    connectSql_for_Inventory();
}
//连接数据库
void MainWindow::connectSql_for_Inventory(){
    int val1=0,val2=INT_MAX;
    QSqlDatabase cdb=QSqlDatabase::addDatabase("QSQLITE");
    cdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/val.db");
    if(cdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery cql(cdb);
    //bool tableExists = cql.exec("SELECT name FROM val WHERE type='table' AND name='WarningThreshold'");
    // if(tableExists && cql.next()) {
    //     // 表已存在，不执行创建操作
    // } else {
    //     // 表不存在，执行创建操作
    //     cql.exec("create table WarningThreshold(val1,val2)");
    //     cql.exec("insert into WarningThreshold(val1,val2) values(0,"+QString::number(INT_MAX)+")");
    // }
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
    bool tableExists = sql.exec("SELECT name FROM productinformation WHERE type='table' AND name= 'inventoryproduct'");
    if(tableExists && sql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
        sql.exec("create table inventoryproduct(itemnumber UNIQUE NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                                               "itemprice NOT NULL,number NOT NULL,productdescription)");
    }
    sql.exec("select * from inventoryproduct");
    while(sql.next()){
        int row= ui->tableWidget_2->rowCount(); // 获取当前的行数，将在最后插入一行
        //qDebug()<<row;
        //表格新建一行
        ui->tableWidget_2->insertRow(row);
        //每行中各个方格的数据
        QVector<QTableWidgetItem*>item;
        QTableWidgetItem *item1 = new QTableWidgetItem(sql.value("itemnumber").toString());
        QTableWidgetItem *item2 = new QTableWidgetItem(sql.value("itemname").toString());
        QTableWidgetItem *item3 = new QTableWidgetItem(sql.value("itemcategory").toString());
        Tools::inventorykinds.insert(sql.value("itemcategory").toString());
        QTableWidgetItem *item4 = new QTableWidgetItem(sql.value("itembrand").toString());
        QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemsupplier").toString());
        QTableWidgetItem *item6 = new QTableWidgetItem(sql.value("itemprice").toString());
        QTableWidgetItem *item7 = new QTableWidgetItem(sql.value("number").toString());
        QTableWidgetItem *item8 = new QTableWidgetItem(QString::number((sql.value("itemprice").toInt())*(sql.value("number").toInt())));
        QTableWidgetItem *item9 = new QTableWidgetItem("出库");
        QTableWidgetItem *item10 = new QTableWidgetItem("修改");
        QTableWidgetItem *item11 = new QTableWidgetItem("删除");
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
        item.push_back(item11);
        int col=0;
        //qDebug()<<"1111";
        foreach(QTableWidgetItem *i,item){
            ui->tableWidget_2->setItem(row,col,i);
            col++;
        }
        if(sql.value("number").toInt()<val1){
            ID1.insert(sql.value("itemnumber").toString());
        }
        if(sql.value("number").toInt()>val2){
            ID2.insert(sql.value("itemnumber").toString());
        }
    }

    sdb.close();
    foreach(QString s,Tools::inventorykinds){
        ui->comboBox_2->addItem(s);
    }
}

//表格点击事件
// 获取被点击的单元格的行和列
void MainWindow::item_clicked_for_Inventory(QTableWidgetItem*item){
int row = item->row();
int column = item->column();
if(column==8){
    //出库操作实现
    bool ok;
    int minValue = 0;       // 最小值
    int maxValue = ui->tableWidget_2->item(row,6)->text().toInt();// 最大值
    int defaultValue = 1;  // 默认值
    int step = 1;           // 步长
    QString id=ui->tableWidget_2->item(row,0)->text();//库存ID
    int value = QInputDialog::getInt(this, tr("出库数量"),
                                     tr("请输入出库的数量:"),
                                     defaultValue, minValue, maxValue, step, &ok);

    if (ok) {
        //提示消息
        if(value!=0){
            QMessageBox::information(this,tr("提示"),tr("出库成功"));
        }
        QSqlDatabase qdb=QSqlDatabase::addDatabase("QSQLITE");
        //修改表格数据
        qdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
        if(qdb.open()){
            qDebug()<<"数据库打开成功xxxxx";
        }else{
            qDebug()<<"数据库打开失败";
            return;
        }
        QSqlQuery sql(qdb);
        //修改库存数量
        sql.exec( "UPDATE inventoryproduct SET number = " + QString::number(maxValue - value) + " WHERE itemnumber = '" + id + "'");

        //出库记录
        this->windowsFlush_for_inventoryDiag();
        if(value==0){
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
        bool tableExists = qsql.exec("SELECT name FROM record WHERE type='table' AND name='outputrecord'");
        if(tableExists && qsql.next()) {
            // 表已存在，不执行创建操作
        } else {
            // 表不存在，执行创建操作
            qsql.exec("create table outputrecord(itemnumber NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                      "itemprice NOT NULL,number NOT NULL,time)");
        }

        // 获取当前的日期和时间
        QDateTime currentDateTime = QDateTime::currentDateTime();

        // 将当前的日期和时间转换为字符串，这里使用格式 "yyyy/MM/dd hh:mm:ss"
        QString formattedDateTime = currentDateTime.toString("yyyy/MM/dd hh:mm:ss");
        if(!qsql.exec("insert into outputrecord(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                       "itemprice,number,time)"
                       " values('"+ui->tableWidget_2->item(row,0)->text()+"','"+ui->tableWidget_2->item(row,1)->text()+"','"+ui->tableWidget_2->item(row,2)->text()+"','"
                       +ui->tableWidget_2->item(row,3)->text()+"','"+ui->tableWidget_2->item(row,4)->text()+"',"+ui->tableWidget_2->item(row,5)->text()+","
                       +QString::number(value)+",'"+formattedDateTime+"')"))
        {
            qDebug()<<"无法录入出库记录,有数据为空";
        }
        sdb.close();
        QSqlDatabase adb=QSqlDatabase::addDatabase("QSQLITE");

        adb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
        if(adb.open()){
            qDebug()<<"数据库打开成功xxxxx";
        }else{
            qDebug()<<"数据库打开失败";
            return;
        }
        QSqlQuery csql(adb);
        csql.exec("select * from product");
        int val=0;
        bool isexist=false;
            while(csql.next()){
            if(csql.value("itemnumber").toString()==id){
                val=csql.value("Inventoryquantity").toInt();
                isexist=true;
                }
            }
            if(!isexist){
                csql.exec("insert into product(itemnumber,itemname,itemcategory,itembrand,itemsupplier,"
                               "itemprice,Inventoryquantity，productdescription)"
                               " values('"+ui->tableWidget_2->item(row,0)->text()+"','"+ui->tableWidget_2->item(row,1)->text()+"','"+ui->tableWidget_2->item(row,2)->text()+"','"
                               +ui->tableWidget_2->item(row,3)->text()+"','"+ui->tableWidget_2->item(row,4)->text()+"',"+ui->tableWidget_2->item(row,5)->text()+","
                               +QString::number(value)+",'暂无描述')");
            }else{
            csql.exec( "UPDATE product SET Inventoryquantity = " + QString::number(val+value) + " WHERE itemnumber = '" + id + "'");
            }
            adb.close();
        }
    }else if(column==9){
    //修改操作实现
        InventoryproductChange*ic=new InventoryproductChange(ui->tableWidget_2->item(row,0)->text());
        ic->exec();
        this->windowsFlush_for_inventoryDiag();

    }
    else if(column==10){
        QString id=ui->tableWidget_2->item(row,0)->text();
        int result=QMessageBox::information(this,tr("警告"),tr("确定删除吗")
                                              ,QMessageBox::Yes,QMessageBox::No);
        if(result==QMessageBox::Yes){
            QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
            if(sdb.open()){
                qDebug()<<"数据库打开成功cccc";
            }else{
                qDebug()<<"数据库打开失败aaaa";
            }
            QSqlQuery sql(sdb);
            sql.exec("delete from inventoryproduct where itemnumber= '"+id+"'");
            sdb.close();
            this->windowsFlush_for_inventoryDiag();
        }
    }
    else{
        QMessageBox::information(this,tr("内容"),item->text());
    }
}
//入库
void MainWindow::inputAction(){
    InventoryproductChange*ic=new InventoryproductChange(this);
    //ic->setParent(this);
    ic->exec();
    this->windowsFlush_for_inventoryDiag();
}
//导出表格
//存储信息到xlsx表格文件
void MainWindow::DatatoXlsx_for_inventory(){
    //操作表格对象，第三方库
    //存入数据
    QVector<QString>head={"库存ID","名称","类别","品牌",
                             "供应商","单价","在库数量","总价值"};
    QXlsx::Document xlsx;
    int col=1;
    foreach(QString s,head){
        xlsx.write(1,col,s);
        col++;
    }
    int row=2;
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
        xlsx.write(row,1,sql.value(0).toString());
        xlsx.write(row,2,sql.value(1).toString());
        xlsx.write(row,3,sql.value(2).toString());
        xlsx.write(row,4,sql.value(3).toString());
        xlsx.write(row,5,sql.value(4).toString());
        xlsx.write(row,6,sql.value(5).toString());
        xlsx.write(row,7,sql.value(6).toString());
        xlsx.write(row,8,sql.value(5).toInt()*sql.value(6).toInt());
        row++;
    }
    qDebug()<<"到这了";
    //保存文件夹位置
    QString dirpath=QFileDialog::getExistingDirectory(this,"选择文件保存路径","D:/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QDir filepath(dirpath);
    if(!filepath.exists()){
        //文件路径不存在
        QMessageBox::warning(this,tr("错误"),tr("导出失败，文件夹不存在"));
        return;
    }
    //文件路径存在
    //输入文件名
    QString filename=QInputDialog::getText(this,"输入文件名","请输入文件名（不用加后缀，指定.xlsx）");
    if(filename.isEmpty()){
        QMessageBox::warning(this,tr("错误"),tr("文件名不能为空"));
        return;
    }
    //文件绝对路径
    QString fileabsoluteoath=filepath.absoluteFilePath(filename+".xlsx");
    QFile file(fileabsoluteoath);
    if (file.open(QIODevice::WriteOnly))
    {
        if (!xlsx.saveAs(&file))
        {
            qDebug() << "文件导出失败";
        }
        else
        {
            qDebug() << "文件导出成功";
        }
        file.close();
    }else
    {
        qDebug() << "打开失败";
    }
}

//搜索功能
void MainWindow::SearchForCondition_for_inventory(){
    //清空表格数据
    ui->tableWidget_2->clearContents();
    ui->tableWidget_2->setRowCount(0);
    //与数据库建立连接
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/productinformation.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    //符合条件的数目
    int count=0;
    sql.exec("select * from inventoryproduct");
    while(sql.next()){
        if((ui->comboBox_2->currentText()=="全部"?true:sql.value("itemcategory").toString()==ui->comboBox_2->currentText())&&
            (ui->namelineEdit_2->text().isEmpty()?true:sql.value("itemname").toString()==ui->namelineEdit_2->text())&&
            (ui->numberlineEdit_2->text().isEmpty()?true:sql.value("itemnumber").toString()==ui->numberlineEdit_2->text())){
            int row= ui->tableWidget_2->rowCount(); // 获取当前的行数，将在最后插入一行
            //qDebug()<<row;
            //表格新建一行
            ui->tableWidget_2->insertRow(row);
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
            QTableWidgetItem *item9 = new QTableWidgetItem("出库");
            QTableWidgetItem *item10 = new QTableWidgetItem("修改");
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
            //qDebug()<<"1111";
            foreach(QTableWidgetItem *i,item){
                ui->tableWidget_2->setItem(row,col,i);
                col++;
            }
            count++;
        }
    }
    sdb.close();
    //右下角
    ui->number->setText("共"+QString::number(ui->tableWidget_2->rowCount())+"条");
    if(count==0){
        QMessageBox::information(this,tr("提示"),tr("没有符合条件的库存"));
    }
}

//出库记录
void MainWindow::outputRecord(){
    Record*rd=new Record("outputrecord");
    //rd->setParent(this);
    rd->setTableTitle("出库记录");
    rd->setWindowTitle("出库记录");
    rd->show();
}

//入库记录
void MainWindow::inputRecord(){
    Record*rd=new Record("inputrecord");
    //rd->setParent(this);
    rd->setTableTitle("入库记录");
    rd->setWindowTitle("入库记录");
    rd->show();
}


//以下为进货记录
//连接数据库导入表格
void MainWindow::connectSql_for_restockRecord(){
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    bool tableExists = sql.exec("SELECT name FROM record WHERE type='table' AND name='restockrecord'");
    if(tableExists && sql.next()) {
        // 表已存在，不执行创建操作
    } else {
        // 表不存在，执行创建操作
        sql.exec("create table restockrecord(itemnumber NOT NULL,itemname NOT NULL,itemcategory NOT NULL,itembrand NOT NULL,itemsupplier NOT NULL,"
                 "itemprice NOT NULL,number NOT NULL,productdescription,time)");
    }
    sql.exec("select * from restockrecord");
    while(sql.next()){
        int row= ui->tableWidget_7->rowCount(); // 获取当前的行数，将在最后插入一行
        //qDebug()<<row;
        //表格新建一行
        ui->tableWidget_7->insertRow(row);
        //每行中各个方格的数据
        QVector<QTableWidgetItem*>item;
        QTableWidgetItem *item1 = new QTableWidgetItem(sql.value("itemnumber").toString());
        QTableWidgetItem *item2 = new QTableWidgetItem(sql.value("itemname").toString());
        QTableWidgetItem *item3 = new QTableWidgetItem(sql.value("itemcategory").toString());
        QTableWidgetItem *item4 = new QTableWidgetItem(sql.value("itembrand").toString());
        //QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemsupplier").toString());
        QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemprice").toString());
        QTableWidgetItem *item6 = new QTableWidgetItem(sql.value("number").toString());
        QTableWidgetItem *item7 = new QTableWidgetItem(QString::number((sql.value("itemprice").toInt())*(sql.value("number").toInt())));
        QTableWidgetItem *item8 = new QTableWidgetItem(sql.value("time").toString());
        QTableWidgetItem *item9 = new QTableWidgetItem("删除");
        qDebug()<<QString::number((sql.value("itemprice").toInt())*(sql.value("number").toInt()));
        item.push_back(item1);
        item.push_back(item2);
        item.push_back(item3);
        item.push_back(item4);
        item.push_back(item5);
        item.push_back(item6);
        item.push_back(item7);
        item.push_back(item8);
        item.push_back(item9);
        int col=0;
        //qDebug()<<"1111";
        foreach(QTableWidgetItem *i,item){
            ui->tableWidget_7->setItem(row,col,i);
            col++;
        }
    }

    sdb.close();
    //右下角
    ui->number_3->setText("共"+QString::number(ui->tableWidget_7->rowCount())+"条");
}

//出售记录页的初始化
void MainWindow::init_for_restockRecord(){
    //工具菜单按钮初始化
    Datatoxlsx=new QAction("导出表格",this);
    Cleardata=new QAction("清空数据",this);
    datagraph=new QAction("显示图表",this);
    graphdownload=new QAction("下载图表",this);
    toolMenu_7=new QMenu(this);
    toolMenu_7->addAction(Datatoxlsx);
    toolMenu_7->addAction(datagraph);
    toolMenu_7->addAction(graphdownload);
    toolMenu_7->addAction(Cleardata);

    ui->toolButton_7->setMenu(toolMenu_7);
    //设置打开模式
    ui->toolButton_7->setPopupMode(QToolButton::InstantPopup);

    TableSet(ui->tableWidget_7);
}
//方格点击事件
void MainWindow::item_clicked_for_restocktable(QTableWidgetItem*item){
    // 获取被点击的单元格的行和列
    int row = item->row();
    int column = item->column();
    if(column==8){
        //删除操作
        int result=QMessageBox::information(this,tr("警告"),tr("确定删除吗")
                                              ,QMessageBox::Yes,QMessageBox::No);
        qDebug()<<result<<":"<<QMessageBox::Yes;
        if(result==QMessageBox::Yes){
            QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
            if(sdb.open()){
                qDebug()<<"数据库打开成功cccc";
            }else{
                qDebug()<<"数据库打开失败aaaa";
            }
            QSqlQuery sql(sdb);
            sql.exec("delete from restockrecord where "
                     "time='"+ui->tableWidget_7->item(row,7)->text()+"'");
            sdb.close();
            this->windowsFlush_for_restockDiag();
        }
    }else{
        //显示内容
        QMessageBox::information(this,tr("内容"),item->text());
    }
}

//出售记录界面表格重建
void MainWindow::windowsFlush_for_restockDiag(){
    ui->tableWidget_7->clearContents();
    ui->tableWidget_7->setRowCount(0);
    connectSql_for_restockRecord();
}
//出售界面搜索按钮事件
void MainWindow::SearchForCondition_for_restockRecord(){

    //清空表格数据
    ui->tableWidget_7->clearContents();
    ui->tableWidget_7->setRowCount(0);
    //与数据库建立连接
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    //符合条件的数目
    int count=0;
    sql.exec("select * from restockrecord");
    while(sql.next()){
        // qDebug()<<sql.value("itemname");
        // qDebug()<<(ui->timelineEdit_3->text().isEmpty());
        if((ui->timelineEdit_3->text().isEmpty()?true:isvaild(ui->timelineEdit_3->text().trimmed(),sql.value("time").toString()))&&
            (ui->namelineEdit_7->text().isEmpty()?true:sql.value("itemname").toString()==ui->namelineEdit_7->text())&&
            (ui->numberlineEdit_7->text().isEmpty()?true:sql.value("itemnumber").toString()==ui->numberlineEdit_7->text())){
            int row= ui->tableWidget_7->rowCount(); // 获取当前的行数，将在最后插入一行
            //qDebug()<<row;
            //表格新建一行
            ui->tableWidget_7->insertRow(row);
            //每行中各个方格的数据
            QVector<QTableWidgetItem*>item;
            QTableWidgetItem *item1 = new QTableWidgetItem(sql.value("itemnumber").toString());
            QTableWidgetItem *item2 = new QTableWidgetItem(sql.value("itemname").toString());
            QTableWidgetItem *item3 = new QTableWidgetItem(sql.value("itemcategory").toString());
            QTableWidgetItem *item4 = new QTableWidgetItem(sql.value("itembrand").toString());
            //QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemsupplier").toString());
            QTableWidgetItem *item5 = new QTableWidgetItem(sql.value("itemprice").toString());
            QTableWidgetItem *item6 = new QTableWidgetItem(sql.value("number").toString());
            QTableWidgetItem *item7 = new QTableWidgetItem(QString::number((sql.value("itemprice").toInt())*(sql.value("number").toInt())));
            QTableWidgetItem *item8 = new QTableWidgetItem(sql.value("time").toString());
            QTableWidgetItem *item9 = new QTableWidgetItem("删除");
            qDebug()<<QString::number((sql.value("itemprice").toInt())*(sql.value("number").toInt()));
            item.push_back(item1);
            item.push_back(item2);
            item.push_back(item3);
            item.push_back(item4);
            item.push_back(item5);
            item.push_back(item6);
            item.push_back(item7);
            item.push_back(item8);
            item.push_back(item9);
            int col=0;
            //qDebug()<<"1111";
            foreach(QTableWidgetItem *i,item){
                ui->tableWidget_7->setItem(row,col,i);
                col++;
            }
            count++;
        }
    }
    sdb.close();
    //右下角
    ui->number_3->setText("共"+QString::number(ui->tableWidget_7->rowCount())+"条");
    if(count==0){
        QMessageBox::information(this,tr("提示"),tr("没有符合条件的进货记录"));
    }
}

//导出表格功能
void MainWindow::DatatoXlsx_for_restockRecord(){
    //操作表格对象，第三方库
    //存入数据

    QVector<QString>head={"商品编号","商品名称","商品类别","品牌",
                             "单价","进货数量","总金额","时间"};
    QXlsx::Document xlsx;
    int col=1;
    foreach(QString s,head){
        xlsx.write(1,col,s);
        col++;
    }
    int row=2;
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功gggg";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(sdb);
    sql.exec("select * from restockrecord");
    while(sql.next()){
        xlsx.write(row,1,sql.value(0).toString());
        xlsx.write(row,2,sql.value(1).toString());
        xlsx.write(row,3,sql.value(2).toString());
        xlsx.write(row,4,sql.value(3).toString());
        xlsx.write(row,5,sql.value(5).toString());
        xlsx.write(row,6,sql.value(6).toString());
        xlsx.write(row,7,sql.value(5).toInt()*sql.value(6).toInt());
        xlsx.write(row,8,sql.value(8).toString());
        row++;
    }
    //保存文件夹位置
    QString dirpath=QFileDialog::getExistingDirectory(this,"选择文件保存路径","D:/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QDir filepath(dirpath);
    if(!filepath.exists()){
        //文件路径不存在
        QMessageBox::warning(this,tr("错误"),tr("导出失败，文件夹不存在"));
        return;
    }
    //文件路径存在
    //输入文件名
    QString filename=QInputDialog::getText(this,"输入文件名","请输入文件名（不用加后缀，指定.xlsx）");
    if(filename.isEmpty()){
        QMessageBox::warning(this,tr("错误"),tr("文件名不能为空"));
        return;
    }
    //文件绝对路径
    QString fileabsoluteoath=filepath.absoluteFilePath(filename+".xlsx");
    QFile file(fileabsoluteoath);
    if (file.open(QIODevice::WriteOnly))
    {
        if (!xlsx.saveAs(&file))
        {
            qDebug() << "文件导出失败";
        }
        else
        {
            qDebug() << "文件导出成功";
        }
        file.close();
    }else
    {
        qDebug() << "打开失败";
    }
}
//显示图表
void MainWindow::datatograph_for_restockDiag(){
    restockrecordgraph=new Graph(this);
    restockrecordgraph->restockrecord_graph_show();
    restockrecordgraph->show();
}
//全部清除功能
void MainWindow::DataClear_for_restockDiag(){
    //删除操作
    int result=QMessageBox::information(this,tr("警告"),tr("确定清空数据吗")
                                          ,QMessageBox::Yes,QMessageBox::No);
    if(result==QMessageBox::Yes){
        QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
        if(sdb.open()){
            qDebug()<<"数据库打开成功cccc";
        }else{
            qDebug()<<"数据库打开失败aaaa";
        }
        QSqlQuery sql(sdb);
        sql.exec("delete from restockrecord");
        sdb.close();
        this->windowsFlush_for_restockDiag();
    }
}
//下载图表
void MainWindow::downloadGraph_for_restockDiag(){
    restockrecordgraph=new Graph(this);
    restockrecordgraph->restockrecord_graph_show();

    restockrecordgraph->download_information();
}
//以下为主界面功能的实现
void MainWindow::windowFlush_for_home(){
    init_for_home();
}
void MainWindow::init_for_home(){
    //构建两个图表
    //第一个图表
    firstcharts();
    secondcharts();
}
//构建第一个图表
void MainWindow::firstcharts(){
    //进货记录
    values1=init_values("restockrecord","number","time");
    //销售记录
    values2=init_values("salerecord","salenumber","saletime");

    // 创建横坐标轴
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("时间"); // 设置横坐标轴标题

    // 设置横坐标轴的范围
    axisX->setRange(1, 10);// 设置从1到10
    axisX->setTickCount(10);
    axisX->setLabelFormat("%d");

    // 创建纵坐标轴
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("进货额/销售额"); // 设置纵坐标轴标题

    // 设置纵坐标轴的范围
    axisY->setRange(0, 1000000); // 假设数据范围是从0到1000000
    //纵坐标数目
    axisY->setTickCount(9);
    //axisY->setTickInterval(50000);
    // 设置纵坐标轴的标签格式
    axisY->setLabelFormat("%d");
    //qDebug()<<"崩溃点";
    // 创建数据点
    //第一条折线
    series1 = new QLineSeries();
    for(int i=1;i<=10;++i){
        series1->append(i,values1[i-1]);
        //qDebug()<<values1[i-1];
    }
    series1->setName("进货额");
    // 设置第一条折线的颜色
    QPen pen1(Qt::red);
    pen1.setWidth(2);
    series1->setPen(pen1);

    //第二条折线
    series2 = new QLineSeries();
    for(int i=1;i<=10;++i){
        series2->append(i,values2[i-1]);
        //qDebug()<<values1[i-1];
    }
    series2->setName("销售额");
    // 设置第二条折线的颜色
    QPen pen2(Qt::blue);
    pen2.setWidth(2);
    series2->setPen(pen2);
    // 创建图表
    chart = new QChart();
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->setTitle("近十分钟出售额与进货额折线图");
    // 将横坐标轴添加到图表中
    chart->addAxis(axisX, Qt::AlignBottom); // Qt::AlignBottom 表示横坐标轴位于底部
    // 将系列与横坐标轴关联
    series1->attachAxis(axisX);
    series2->attachAxis(axisX);
    // 将纵坐标轴添加到图表中
    chart->addAxis(axisY, Qt::AlignLeft); // Qt::AlignLeft 表示纵坐标轴位于左侧

    // 将系列与纵坐标轴关联
    series1->attachAxis(axisY);
    series2->attachAxis(axisY);
    // 创建图表视图
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    //chartView->setMinimumSize(600, 600);
    // 设置窗口布局
    ui->gridLayout_2->addWidget(chartView, Qt::AlignCenter, Qt::AlignCenter);
}

//构建第二个图表
void MainWindow::secondcharts(){
    int*values=new int[10];
    for(int i=0;i<10;i++){
        values[i]=values2[i]-values1[i];
    }
    // 创建横坐标轴
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("时间"); // 设置横坐标轴标题

    // 设置横坐标轴的范围
    axisX->setRange(1, 10);// 设置从1到10
    axisX->setTickCount(9);
    axisX->setLabelFormat("%d");

    // 创建纵坐标轴
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("利润"); // 设置纵坐标轴标题

    // 设置纵坐标轴的范围
    axisY->setRange(-500000,500000); // 假设数据范围是从0到1000000
    //纵坐标数目
    axisY->setTickCount(10);
    //axisY->setTickInterval(50000);
    // 设置纵坐标轴的标签格式
    axisY->setLabelFormat("%d");
    //qDebug()<<"崩溃点";
    // 创建数据点
    series3 = new QLineSeries();
    for(int i=1;i<=10;++i){
        series3->append(i,values[i-1]);
        //qDebug()<<values1[i-1];
    }
    series3->setName("利润");
    // 设置第一条折线的颜色
    QPen pen1(Qt::green);
    pen1.setWidth(2);
    series3->setPen(pen1);

    // 创建图表
    chart2 = new QChart();
    chart2->addSeries(series3);
    chart2->setTitle("近十分钟利润变化折线图");
    // 将横坐标轴添加到图表中
    chart2->addAxis(axisX, Qt::AlignBottom); // Qt::AlignBottom 表示横坐标轴位于底部
    // 将系列与横坐标轴关联
    series3->attachAxis(axisX);
    // 将纵坐标轴添加到图表中
    chart2->addAxis(axisY, Qt::AlignLeft); // Qt::AlignLeft 表示纵坐标轴位于左侧

    // 将系列与纵坐标轴关联
    series3->attachAxis(axisY);

    // 创建图表视图
    chartView2 = new QChartView(chart2);
    chartView2->setRenderHint(QPainter::Antialiasing);
    //chartView->setMinimumSize(600, 600);
    // 设置窗口布局
    ui->gridLayout_3->addWidget(chartView2, Qt::AlignCenter, Qt::AlignCenter);
}
//获得图表数据
int*MainWindow::init_values(QString recordname,QString numbername,QString timename){
    // 获取当前的日期和时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    //折线图从十分钟前开始
    QDateTime starttime=currentDateTime.addSecs(-600);
    qDebug()<<starttime.toString();

    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    //qDebug()<<"这里";
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功7777";
    }else{
        qDebug()<<"数据库打开失败";
        return nullptr;
    }
    int*values=new int[10]{0};
    QSqlQuery sql(sdb);
    sql.exec("select * from "+recordname);
    int index=0;
    while(sql.next()){
        QDateTime time=QDateTime::fromString(sql.value(timename).toString(),"yyyy/MM/dd hh:mm:ss");
        if(starttime.date()!=time.date()){
            continue;
        }
        if(starttime.time().hour()!=time.time().hour()){
            continue;
        }
        qDebug()<<time.toString();
        qDebug()<<starttime.time().minute()<<" "<<time.time().minute();
        while(starttime.time().minute()<time.time().minute()){
            if(index<10){
                starttime=starttime.addSecs(60);
                index++;
                qDebug()<<index;
            }else{
                break;
            }
        }
        if(starttime.time().minute()==time.time().minute()){
            if(index<10){
                values[index]+=sql.value(numbername).toInt()*sql.value("itemprice").toInt();
            }
        }
    }
    sdb.close();
    return values;
}
//查看总销售额按钮功能
void MainWindow::salevolumeButton_clicked(){
    //连接销售记录数据库
    int totalvalue=0;
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery sql(sdb);
    sql.exec("select * from salerecord");
    while(sql.next()){
        totalvalue+=sql.value("itemprice").toInt()*sql.value("salenumber").toInt();
    }
    sdb.close();
    //salevolume=totalvalue;
    QMessageBox::information(this,tr("总销售额"),tr("总销售额为：")+QString::number(totalvalue));
}

//查看总成本按钮功能
void MainWindow::costButton_clicked(){
    //连接进货记录数据库
    int totalvalue=0;
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery sql(sdb);
    sql.exec("select * from inputrecord");
    while(sql.next()){
        totalvalue+=sql.value("itemprice").toInt()*sql.value("number").toInt();
    }
    sdb.close();
    //cost=totalvalue;
    QMessageBox::information(this,tr("总成本"),tr("总成本为：")+QString::number(totalvalue));
}

//查看总利润按钮功能
void MainWindow::profitButton_clicked(){
    //连接销售记录数据库
    int salevolume=0;
    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery sql(sdb);
    sql.exec("select * from salerecord");
    while(sql.next()){
        salevolume+=sql.value("itemprice").toInt()*sql.value("salenumber").toInt();
    }
    sdb.close();
    //连接进货记录数据库
    int cost=0;
    QSqlDatabase cdb=QSqlDatabase::addDatabase("QSQLITE");
    cdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(cdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery qsql(cdb);
    qsql.exec("select * from inputrecord");
    while(qsql.next()){
        cost+=qsql.value("itemprice").toInt()*qsql.value("number").toInt();
    }
    cdb.close();
    //qDebug()<<cost;
    QMessageBox::information(this,tr("总利润"),tr("总利润为：")+QString::number(salevolume-cost));
}
//预警通知
void MainWindow::informationButton_clicked(){
    Information*im=new Information(this);
    im->show();
}
//第一个图表下载按钮
void MainWindow::downloadButton1_clicked(){
    QString path=selectDownloadPath();
    if(path.isNull()){
        QMessageBox::information(this,tr("提示"),tr("下载失败"));
        return;
    }
    // 创建一个QPixmap来捕获QChartView的内容
    QPixmap chartPixmap = chartView->grab();

    // 现在保存QPixmap为图片文件
    bool result = chartPixmap.save(path, "PNG");

    if (result) {
        QMessageBox::information(this,tr("提示"),tr("下载成功"));
    } else {
        QMessageBox::information(this,tr("提示"),tr("下载失败"));
    }
}
//第二个图表下载按钮
void MainWindow::downloadButton2_clicked(){
    QString path=selectDownloadPath();
    if(path.isNull()){
        QMessageBox::information(this,tr("提示"),tr("下载失败"));
        return;
    }
    // 创建一个QPixmap来捕获QChartView的内容
    QPixmap chartPixmap = chartView2->grab();

    // 现在保存QPixmap为图片文件
    bool result = chartPixmap.save(path, "PNG");

    if (result) {
        QMessageBox::information(this,tr("提示"),tr("下载成功"));
    } else {
        QMessageBox::information(this,tr("提示"),tr("下载失败"));
    }
}


//选择下载文件夹
QString MainWindow::selectDownloadPath(){
    QString dirpath=QFileDialog::getExistingDirectory(this,"选择文件保存路径","D:/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QDir filepath(dirpath);
    if(!filepath.exists()){
        //文件路径不存在
        QMessageBox::warning(this,tr("错误"),tr("导出失败，文件夹不存在"));
        return NULL;
    }
    //文件路径存在
    //输入文件名
    QString filename=QInputDialog::getText(this,"输入文件名","请输入文件名（不用加后缀，指定.png）");
    if(filename.isEmpty()){
        QMessageBox::warning(this,tr("错误"),tr("文件名不能为空"));
        return NULL;
    }
    //文件绝对路径
    QString fileabsolutepath=filepath.absoluteFilePath(filename+".png");
    return fileabsolutepath;
}
MainWindow::~MainWindow()
{
    delete ui;
}

