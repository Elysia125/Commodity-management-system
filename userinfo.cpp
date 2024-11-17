#include "userinfo.h"
#include "ui_userinfo.h"
#include"login.h"
#include"tools.h"
#include<QFileDialog>
#include<QInputDialog>
#include<QMessageBox>
userinfo::userinfo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::userinfo)
{
    ui->setupUi(this);
    connectSQL();
    init();
    connections();
    //datatoSQL();
}
//连接数据库
void userinfo::connectSQL(){
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+"selfinfromation.db");
    if(db.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(db);
    sql.exec("create table user(username,password,userimage,birthday"
             ",phonenumber,emailnumber,sex,selfedit)");
    sql.exec("SELECT * FROM user");
    //判断用户是否存在
    bool isexit=false;
    while(sql.next()){
        if(sql.value("username")==Tools::username){
            Tools::userimage=sql.value("userimage").toString();
            Tools::birthday=sql.value("birthday").toString();
            Tools::phonenumber=sql.value("phonenumber").toString();
            Tools::emailnumber=sql.value("emailnumber").toString();
            Tools::sex=sql.value("sex").toString();
            Tools::selfedit=sql.value("selfedit").toString();
            isexit=true;
        }
    }
    if(!isexit){
        sql.exec("insert into user(username,password,userimage)"
                 " values('"+Tools::username+"','"+Tools::password+"','"+Tools::userimage+"')");
    }
    db.close();
    //暂存用户名
    name=Tools::username;
}
//将数据修改至数据库
void userinfo::datatoSQL(){
    //修改个人资料的数据库
    if(db.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(db);
    sql.exec("delete from user where username='"+name+"'");
    sql.exec("insert into user(username,password,userimage,birthday"
             ",phonenumber,emailnumber,sex,selfedit)"
             " values('"+Tools::username+"','"+Tools::password+"','"+Tools::userimage+"','"
             +Tools::birthday+"','"+Tools::phonenumber+"','"+Tools::emailnumber+"','"
             +Tools::sex+"','"+Tools::selfedit+"')");
    db.close();
    //修改登录时用的数据库
    QSqlDatabase sdb;
    sdb=QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName(Tools::projectPath+"/database/"+"userinfo.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql2(sdb);
    sql2.exec("delete from user where username='"+name+"'");
    sql2.exec("insert into user(username,password)"
              " values('"+Tools::username+"','"+Tools::password+"')");
    sdb.close();

    //修改存放数据库文件位置的数据库
    QSqlDatabase cdb=QSqlDatabase::addDatabase("QSQLITE");
    cdb.setDatabaseName(Tools::projectPath+"/database"+"/path.db");
    if(cdb.open()){
        qDebug()<<"数据库打开成功";
    }else{
        qDebug()<<"数据库打开失败";
    }
    QSqlQuery qsql(cdb);
    qsql.exec("select * from userpath");
    QString path=NULL;
    while(qsql.next()){
        if(qsql.value("username").toString()==name){
            path=qsql.value("path").toString();
        }
    }
    qsql.exec("delete from userpath where username='"+name+"'");
    qsql.exec("insert into userpath(username,path)"
         " values('"+Tools::username+"','"+path+"')");
    cdb.close();
}
//初始化界面
void userinfo::init(){
    //设置用户名
    ui->usernameButton->setText(Tools::username);
    //设置用户头像
    ui->userimageButton->setStyleSheet( "QPushButton {"
                                       "   border-image: url("+Tools::userimage+") 0 0 0 0 stretch stretch;"
                                                            "   border-radius: 43px;"
                                                            "}");
    if(Tools::sex=="男"){
        ui->maleradioButton->setChecked(true);
    }else if(Tools::sex=="女"){
        ui->femaleradioButton->setChecked(true);
    }
    //设置邮箱
    ui->showemaillabel->setText(Tools::emailnumber);
    //设置电话
    ui->showphonelabel->setText(Tools::phonenumber);
    //在个人简介上显示内容
    ui->selftextEdit->setPlainText(Tools::selfedit);
    //设置生日
    ui->birthdayEdit->setDate(QDate::fromString(Tools::birthday,"yyyy/MM/dd"));
}
//信号与槽功能实现
void userinfo::connections(){
    //修改头像
    connect(ui->userimageButton,&QPushButton::clicked,
            this,&userinfo::userimageButton_clicked);
    //修改用户名
    connect(ui->usernameButton,&QPushButton::clicked,
            this,&userinfo::usernameButton_clicked);
    //修改性别
    connect(ui->maleradioButton,&QPushButton::clicked,
            this,&userinfo::maleradioButton_clicked);
    connect(ui->femaleradioButton,&QPushButton::clicked,
            this,&userinfo::femaleradioButton_clicked);
    //修改电话
    connect(ui->changphoneButton,&QPushButton::clicked,
            this,&userinfo::changephoneButton_clicked);
    //修改邮箱
    connect(ui->changemailButton,&QPushButton::clicked,
            this,&userinfo::changeemailButton_clicked);
    //修改个人简介
    connect(ui->selftextEdit,&QTextEdit::textChanged,
            this,&userinfo::selfedit);
    //退出功能
    connect(ui->exitButton,&QPushButton::clicked,
            this,&userinfo::exitloginButton_clicked);
    //返回按钮或关闭按钮功能
    connect(ui->reButton,&QPushButton::clicked,
            this,&userinfo::reAndclose_clicked);
    //保存按钮功能
    connect(ui->restoreButton,&QPushButton::clicked,
            this,&userinfo::restoreButton_clicked);
    //修改生日功能
    connect(ui->birthdayEdit,&QDateEdit::dateChanged,
            this,&userinfo::birthdayedit);

}
//关闭按钮功能重写
 void userinfo::closeEvent(QCloseEvent*){
     reAndclose_clicked();
 }
//返回按钮功能
void userinfo::reAndclose_clicked(){
    if(ischange&&!isrestore){
        int result=QMessageBox::information(this,tr("提示"),tr("你所做的修改尚未保存，是否进行保存"),
                                              QMessageBox::Yes,QMessageBox::No);
        if(result==QMessageBox::Yes){
            datatoSQL();
            QMessageBox::information(this,tr("提示"),tr("保存成功"),
                                     QMessageBox::Ok);
            isrestore=true;
        }else{
            this->close();
        }
        accept();
    }
    accept();
}
//保存按钮功能
void userinfo::restoreButton_clicked(){
    datatoSQL();
    QMessageBox::information(this,tr("提示"),tr("保存成功"),
                             QMessageBox::Ok);
    isrestore=true;
}
//退出登录功能
void userinfo::exitloginButton_clicked(){
    //this->hide();
    db.close();
    Tools::username="username";
    Tools::password="000000";
    Tools::userimage=":/images/images/user.png";
    Tools::birthday=NULL;
    Tools::phonenumber=NULL;
    Tools::emailnumber=NULL;
    Tools::sex=NULL;
    Tools::selfedit=NULL;
    this->reject();
    login=new Login;
    login->show();
}
//修改用户名功能实现
void userinfo::usernameButton_clicked(){
    ischange=true;
    //创建输入框
    QInputDialog qid(this);
    QString username=qid.getText(this,tr("修改用户名"),tr("请输入修改后的用户名"));
    if(!username.isEmpty()){
        Tools::username=username;
    }
    //设置新的用户名
    ui->usernameButton->setText(Tools::username);
}
//更换头像功能
void userinfo::userimageButton_clicked(){
    ischange=true;
    QFileDialog qfd;
    //限制选取图片
    qfd.setNameFilter("Images (*.png *.jpg *.jpeg *.bmp *.gif)");
    //只能选取一个文件
    qfd.setFileMode(QFileDialog::ExistingFile);
    // 打开文件对话框，并等待用户选择文件
    if (qfd.exec())
    {
        // 获取用户选择的文件路径
        QStringList selectedFiles = qfd.selectedFiles();
        QString selectedFile = selectedFiles.at(0);
        Tools::userimage=selectedFile;
    }
    //设置用户头像
    //ui->userimageButton->setIcon(QIcon(Tools::userimage));
    ui->userimageButton->setStyleSheet( "QPushButton {"
                                       "   border-image: url("+Tools::userimage+") 0 0 0 0 stretch stretch;"
                                       "   border-radius: 43px;"
                                       "}");
}
//两个单元按钮功能实现
void userinfo::maleradioButton_clicked(){
    ischange=true;
    Tools::sex="男";
}
void userinfo::femaleradioButton_clicked(){
    ischange=true;
    Tools::sex="女";
}
//修改电话功能
void userinfo::changephoneButton_clicked(){
    ischange=true;
    //创建输入框
    QInputDialog qid(this);
    QString phonenumber=qid.getText(this,tr("修改电话"),tr("请输入修改后的电话"));
    if(!phonenumber.isEmpty()){
        Tools::phonenumber=phonenumber;
    }
    //设置新的电话
    ui->showphonelabel->setText(Tools::phonenumber);
}
//修改邮箱功能
void userinfo::changeemailButton_clicked(){
    ischange=true;
    //创建输入框
    QInputDialog qid(this);
    QString emailnumber=qid.getText(this,tr("修改邮箱"),tr("请输入修改后的邮箱"));
    if(!emailnumber.isEmpty()){
        Tools::emailnumber=emailnumber;
    }
    //设置新的邮箱
    ui->showemaillabel->setText(Tools::emailnumber);
}
//个人简介功能实现
void userinfo::selfedit(){
    ischange=true;
    QString text=ui->selftextEdit->toPlainText().trimmed();
    if(!text.isEmpty()){
        Tools::selfedit=text;
    }
    //在个人简介上显示内容
    //ui->selftextEdit->setPlainText(Tools::selfedit);
}
//生日功能实现
void userinfo::birthdayedit(){
    ischange=true;
    QString birthday=ui->birthdayEdit->date().toString("yyyy/MM/dd");
    Tools::birthday=birthday;
    qDebug()<<birthday;
    //设置生日
    QDate date=QDate::fromString(Tools::birthday,"yyyy/MM/dd");
    ui->birthdayEdit->setDate(date);
}
userinfo::~userinfo()
{
    delete ui;
}
