#include "register.h"
#include "tools.h"
#include "ui_register.h"
#include<QMessageBox>
#include<QLabel>
#include<QTimer>
#include<QRegularExpression>
#include <QKeyEvent>

Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    // 加载背景图片
    backgroundImage.load(":/images/images/registerbackground.jpg");
    connectSQlite();
    connections();
    this->tipsbutton_clicked();
}
void Register::keyPressEvent(QKeyEvent*event){
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        // 用户按下了 Enter 键
        // 在这里处理 Enter 键被按下的情况
        // 获取当前焦点所在的QLineEdit对象
        QWidget *focusedWidget = QApplication::focusWidget();
        if (focusedWidget != nullptr && focusedWidget->inherits("QLineEdit")) {
            if (focusedWidget == ui->usrLineEdit) {
                ui->pwdLineEdit->setFocus();
            } else if(focusedWidget == ui->pwdLineEdit) {
                ui->apwdLineEdit->setFocus();
            }else{
                registerbutton_clicked();
            }
        }
    }
}
void Register::tipsbutton_clicked(){
    QMessageBox::information(this,tr("注意"),tr("1.密码长度大于等于8并且小于等于20\n2.密码中字母数量不小于2，数字数量不小于4\n3.密码中最多不大于4个数字或字母相邻并连续"));
}
void Register::visiblebutton_1_clicked(){
    isvisible_1=!isvisible_1;
    if(isvisible_1){
        ui->isvisible->setStyleSheet("QPushButton {"
                                     "    border-image: url(:/images/images/visible.png) 0 0 0 0 stretch stretch;"
                                     "}");
        ui->pwdLineEdit->setEchoMode(QLineEdit::Normal);
    }else{
        ui->isvisible->setStyleSheet("QPushButton {"
                                     "    border-image: url(:/images/images/invisible.png) 0 0 0 0 stretch stretch;"
                                     "}");
        ui->pwdLineEdit->setEchoMode(QLineEdit::Password);
    }
}
void Register::visiblebutton_2_clicked(){
    isvisible_2=!isvisible_2;
    if(isvisible_2){
        ui->isvisible_2->setStyleSheet("QPushButton {"
                                     "    border-image: url(:/images/images/visible.png) 0 0 0 0 stretch stretch;"
                                     "}");
        ui->apwdLineEdit->setEchoMode(QLineEdit::Normal);
    }else{
        ui->isvisible_2->setStyleSheet("QPushButton {"
                                     "    border-image: url(:/images/images/invisible.png) 0 0 0 0 stretch stretch;"
                                     "}");
        ui->apwdLineEdit->setEchoMode(QLineEdit::Password);
    }
}
bool Register::isValidPassword(const QString& password) {
    QRegularExpression re("^(?=.*[A-Za-z].*[A-Za-z])(?=.*[0-9].*[0-9].*[0-9].*[0-9])(?!(?:\\p{L}{4}|\\d{4}))[A-Za-z0-9]{8,20}$");

    return re.match(password).hasMatch();
}
void Register::connections(){
    connect(ui->register_2,&QPushButton::clicked,
            this,&Register::registerbutton_clicked);
    connect(ui->reset,&QPushButton::clicked,
            this,&Register::resetbutton_clicked);
    connect(ui->ret,&QPushButton::clicked,
            this,&Register::retbutton_clicked);
    connect(ui->isvisible,&QPushButton::clicked,
            this,&Register::visiblebutton_1_clicked);
    connect(ui->isvisible_2,&QPushButton::clicked,
            this,&Register::visiblebutton_2_clicked);
    connect(ui->pwdRequire,&QPushButton::clicked,
            this,&Register::tipsbutton_clicked);
}
void Register::registerbutton_clicked(){
    QString username=ui->usrLineEdit->text().trimmed();
    QString password=ui->pwdLineEdit->text();
    QString apassword=ui->apwdLineEdit->text();
    if(username.isEmpty()||password.isEmpty()||apassword.isEmpty()){
        QMessageBox::warning(this,tr("警告"),tr("用户名或密码不能为空！！！"));
        return;
    }
    foreach (User u, user) {
        if(u.getusername()==username){
            QMessageBox::warning(this,tr("警告"),tr("该用户名已存在"));
            return;
        }
    }
    if(password!=apassword){
        QMessageBox::warning(this,tr("警告"),tr("两次密码不一致，请重新输入"));
        ui->pwdLineEdit->clear();
        ui->apwdLineEdit->clear();
    }else if(!isValidPassword(password)){
        QMessageBox::warning(this,tr("提示"),tr("密码格式不符合要求"));
    }else{
        QString dbName = "my_db_connection";
        db=QSqlDatabase::addDatabase("QSQLITE",dbName);
        db.setDatabaseName(Tools::projectPath+"/database/"+"userinfo.db");
        if(db.open()){
            qDebug()<<"打开数据库成功";
        }else{
            qDebug()<<"打开数据库失败";
        }
        QSqlQuery sql(db);
        QString query="insert into user(username,password) values('"+username+"','"+password+"')";
        sql.exec(query);
        db.close();
        QMessageBox::information(this,tr("提示"),tr("账号注册成功"));
        (new Login)->show();
        this->hide();
    }
}
void Register::resetbutton_clicked(){
    ui->usrLineEdit->clear();
    ui->pwdLineEdit->clear();
    ui->apwdLineEdit->clear();
    QMessageBox::information(this,tr("提示"),tr("重置成功"));
}
void Register::retbutton_clicked(){
    (new Login())->show();
    ui->ret->setStyleSheet("border: none; text-decoration: underline;");
    this->hide();
}
void Register::connectSQlite(){
    QString dbName = "my_db_connection";
    db=QSqlDatabase::addDatabase("QSQLITE",dbName);
    db.setDatabaseName(Tools::projectPath+"/database/"+"userinfo.db");
    if(db.open()){
        qDebug()<<"打开数据库成功";
    }else{
        qDebug()<<"打开数据库失败";
    }
    QSqlQuery sql(db);
    sql.exec("create table user(username,password)");
    sql.exec("SELECT * FROM user");

    while (sql.next()) {
        QString username = sql.value(0).toString();
        QString password = sql.value(1).toString();
        User temp(username,password);
        user.push_back(temp);
    }
    db.close();
}
Register::~Register()
{
    delete ui;
}
