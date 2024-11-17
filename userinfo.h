#ifndef USERINFO_H
#define USERINFO_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
#include"login.h"
namespace Ui {
class userinfo;
}

class userinfo : public QDialog
{
    Q_OBJECT

public:
    explicit userinfo(QWidget *parent = nullptr);
    ~userinfo();

    void init();

    void connections();
    void connectSQL();
    void datatoSQL();
    void closeEvent(QCloseEvent*)override;
public slots:
    void exitloginButton_clicked();
    void userimageButton_clicked();
    void maleradioButton_clicked();
    void femaleradioButton_clicked();
    void usernameButton_clicked();
    void changephoneButton_clicked();
    void changeemailButton_clicked();
    void birthdayedit();
    void selfedit();
    void reAndclose_clicked();
    void restoreButton_clicked();
private:
    Ui::userinfo *ui;
    //数据库对象
    QSqlDatabase db;
    //暂存用户名，方便后续修改
    QString name;
    //判断修改是否保存
    bool isrestore=false;
    //判断是否修改
    bool ischange=false;
    Login*login;
};

#endif // USERINFO_H
