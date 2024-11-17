#include "login.h"
#include "ui_login.h"
#include"register.h"
#include<QMessageBox>
#include"mainwindow.h"
#include<QKeyEvent>
#include"tools.h"
#include <QCryptographicHash>
Login::Login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
    //w->show();
    // 加载背景图片
    backgroundImage.load(":/images/images/loginbackground.jpg");
    init();
    connectSQlite();
    connections();
}
// bool Login::isright(){
//     return right;
// }
void Login::init(){
    //QString projectPath = QDir::currentPath();
    QDir dir(Tools::projectPath);
    if (!dir.exists("database")) {
        if (dir.mkdir("database")) {
            qDebug() << "创建文件夹成功";
        } else {
            qDebug() << "创建文件夹失败";
            return;
        }
    }
}
void Login::connections(){
    connect(ui->LoginButton,&QPushButton::clicked,
            this,&Login::loginbutton_clicked);
    connect(ui->RegisterButton,&QPushButton::clicked,
            this,&Login::registerbutton_clicked);
    connect(ui->isvisible,&QPushButton::clicked,
            this,&Login::visiblebutton_clicked);
}
void Login::keyPressEvent(QKeyEvent*event){
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        // 用户按下了 Enter 键
        // 在这里处理 Enter 键被按下的情况
        // 获取当前焦点所在的QLineEdit对象
        QWidget *focusedWidget = QApplication::focusWidget();
        if (focusedWidget != nullptr && focusedWidget->inherits("QLineEdit")) {
            if(focusedWidget == ui->usrLineEdit){
                ui->passLineEdit->setFocus();
            }else{
                loginbutton_clicked();
            }
        }
    }
}
void Login::visiblebutton_clicked(){
    isvisible=!isvisible;
    if(isvisible){
        ui->isvisible->setStyleSheet("QPushButton {"
                                      "    border-image: url(:/images/images/visible.png) 0 0 0 0 stretch stretch;"
                                      "}");
        ui->passLineEdit->setEchoMode(QLineEdit::Normal);
    }else{
        ui->isvisible->setStyleSheet("QPushButton {"
                                     "    border-image: url(:/images/images/invisible.png) 0 0 0 0 stretch stretch;"
                                     "}");
        ui->passLineEdit->setEchoMode(QLineEdit::Password);
    }
}
void Login::loginbutton_clicked(){
    QString username=ui->usrLineEdit->text().trimmed();
    QString password=ui->passLineEdit->text();
    User us(username,password);
    foreach (User u, user) {
        if(u.equal(us)){
            Tools::username=username;
            Tools::password=password;
            //根据用户名创建文件夹保存数据库文件
            // 将输入字符串转换为 UTF-8 编码的字节数组
            QByteArray byteArray = username.toUtf8();

            // 计算 SHA-256 哈希值
            QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha256);

            // 使用 Base64 编码将哈希值转换为字符串
            QString encodedString = QString::fromUtf8(hash.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals));

            QSqlDatabase cdb=QSqlDatabase::addDatabase("QSQLITE");
            cdb.setDatabaseName(Tools::projectPath+"/database"+"/path.db");
            if(cdb.open()){
                qDebug()<<"数据库打开成功";
            }else{
                qDebug()<<"数据库打开失败";
            }
            QSqlQuery qsql(cdb);
            bool tableExists = qsql.exec("SELECT name FROM path WHERE type='table' AND name= 'userpath'");
            if(tableExists && qsql.next()) {
                // 表已存在，不执行创建操作
            } else {
                // 表不存在，执行创建操作
                qsql.exec("create table userpath(username UNIQUE,path)");
            }
            int count=0;
            qsql.exec("select * from userpath");
            while(qsql.next()){
                if(qsql.value("username").toString()==username){
                    Tools::folderName=qsql.value("path").toString();
                    count++;
                }
            }
            if(count==0){
               qsql.exec("insert into userpath(username,path) values('"+username+"','"+encodedString+"')");
               Tools::folderName=encodedString;
            }
            cdb.close();
            QDir dir(Tools::projectPath+"/database");
            if (!dir.exists(Tools::folderName)) {
                if (dir.mkdir(Tools::folderName)) {
                    qDebug() <<"创建成功cccc";
                } else {
                    qDebug() <<"创建失败aaaaa"<<Tools::folderName;
                }
            }




            QSqlDatabase sdb;
            sdb=QSqlDatabase::addDatabase("QSQLITE");
            sdb.setDatabaseName(Tools::projectPath+"/database"+"/selfinfromation.db");
            if(sdb.open()){
                qDebug()<<"数据库打开成功lllll";
            }else{
                qDebug()<<"数据库打开失败";
            }
            QSqlQuery sql(sdb);
            sql.exec("SELECT * FROM user");
            while(sql.next()){
                if(sql.value("username")==Tools::username){
                    Tools::userimage=sql.value("userimage").toString();
                }
            }
            sdb.close();
            qDebug()<<Tools::userimage;
            w=new MainWindow;
            w->show();
            this->hide();
            return;
        }
    }
    if(!username.isEmpty()&&!password.isEmpty()){
        QMessageBox::warning(this,tr("警告"),tr("用户名或密码错误，请重新输入"));
    }else{
        QMessageBox::warning(this,tr("警告"),tr("用户名或密码不能为空，请重新输入"));
    }
    ui->usrLineEdit->clear();
    ui->passLineEdit->clear();
    ui->usrLineEdit->setFocus();
}
void Login::connectSQlite(){
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database"+"/userinfo.db");
    db.open();
    QSqlQuery sql;
    sql.exec("SELECT * FROM user");

    while (sql.next()) {
        QString username = sql.value(0).toString();
        QString password = sql.value(1).toString();
        User temp(username,password);
        user.push_back(temp);
    }
    db.close();
}
void Login::registerbutton_clicked(){
    (new Register)->show();
    this->hide();
}
Login::~Login()
{
    delete ui;
}
