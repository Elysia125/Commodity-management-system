#include "changepwd.h"
#include "ui_changepwd.h"
#include<QKeyEvent>
#include<QMessageBox>
#include"tools.h"
#include"login.h"
#include<QRegularExpression>
ChangePwd::ChangePwd(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangePwd)
{
    ui->setupUi(this);
    connections();
    tipsbutton_clicked();
}
//Enter键功能
void ChangePwd::keyPressEvent(QKeyEvent*event){
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        // 用户按下了 Enter 键
        // 在这里处理 Enter 键被按下的情况
        // 光标指向下一个输入框
        // 获取当前焦点所在的QLineEdit对象
        QWidget *focusedWidget = QApplication::focusWidget();
        if (focusedWidget != nullptr && focusedWidget->inherits("QLineEdit")) {
            if(focusedWidget == ui->pblineEdit){
                ui->pnlineEdit->setFocus();
            }else if(focusedWidget == ui->pnlineEdit){
                ui->apnlineEdit->setFocus();
            }else{
                YesButton_clicked();
            }
        }
    }
}
//重置按钮功能实现
void ChangePwd::resetButton_clicked(){
    ui->pblineEdit->clear();
    ui->pnlineEdit->clear();
    ui->apnlineEdit->clear();
    ui->pblineEdit->setFocus();
}
//返回功能实现
void ChangePwd::reButton_clicked(){
    //(new MainWindow)->show();
    this->hide();
}
//确认功能实现
void ChangePwd::YesButton_clicked(){
    QString pwd_bef=ui->pblineEdit->text();
    QString pwd_now=ui->pnlineEdit->text();
    QString apwd_now=ui->apnlineEdit->text();
    if(pwd_bef.isEmpty()||pwd_now.isEmpty()||apwd_now.isEmpty()){
        QMessageBox::warning(this,tr("警告"),tr("各输入不能为空"));
        return;
    }
    if(pwd_bef!=Tools::password){
        QMessageBox::warning(this,tr("警告"),tr("原密码错误"));
        return;
    }
    if(pwd_now!=apwd_now){
        QMessageBox::warning(this,tr("警告"),tr("两次密码输入不一致"));
        return;
    }
    //判断密码是否符合要求
    if(!isValidPassword(pwd_now)){
        QMessageBox::warning(this,tr("警告"),tr("密码不符合要求"));
        return;
    }
    //打开数据库进行密码更改
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(Tools::projectPath+"/database/"+"userinfo.db");
    if(db.open()){
        qDebug()<<"数据库已打开";
    }else{
        qDebug()<<"数据库打开失败";
        return;
    }
    QSqlQuery sql(db);
    sql.exec("update user set password='"+pwd_now+"' where username='"+Tools::username+"'");
    db.close();
    QMessageBox::information(this,tr("提示"),tr("密码已更改，请重新登录"));
    //使exec()返回Accepted
    accept();
    //打开登陆界面，并隐藏该界面
    Login *loginDialog = new Login;
    loginDialog->show();
    this->hide();
}
//信号与槽功能实现
void ChangePwd::connections(){
    //确认按钮
    connect(ui->YesButton,&QPushButton::clicked,
            this,&ChangePwd::YesButton_clicked);
    //重置按钮
    connect(ui->resetButton,&QPushButton::clicked,
            this,&ChangePwd::resetButton_clicked);
    //返回按钮
    connect(ui->reButton,&QPushButton::clicked,
            this,&ChangePwd::reButton_clicked);
    //提示按钮
    connect(ui->tipsButton,&QPushButton::clicked,
            this,&ChangePwd::tipsbutton_clicked);
}
//密码检测
bool ChangePwd::isValidPassword(const QString& password) {
    QRegularExpression re("^(?=.*[A-Za-z].*[A-Za-z])(?=.*[0-9].*[0-9].*[0-9].*[0-9])(?!(?:\\p{L}{4}|\\d{4}))[A-Za-z0-9]{8,20}$");

    return re.match(password).hasMatch();
}
//
void ChangePwd::tipsbutton_clicked(){
    QMessageBox::information(this,tr("注意"),tr("1.密码长度大于等于8并且小于等于20\n2.密码中字母数量不小于2，数字数量不小于4\n3.密码中最多不大于4个数字或字母相邻并连续"));
}
ChangePwd::~ChangePwd()
{
    delete ui;
}
