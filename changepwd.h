#ifndef CHANGEPWD_H
#define CHANGEPWD_H

#include <QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>
namespace Ui {
class ChangePwd;
}

class ChangePwd : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePwd(QWidget *parent = nullptr);
    ~ChangePwd();
    //键盘事件重写
    void keyPressEvent(QKeyEvent*)override;
    void connections();
    bool isValidPassword(const QString &password);
public slots:
    //确定按钮功能
    void YesButton_clicked();
    //重置按钮功能
    void resetButton_clicked();
    //返回按钮功能
    void reButton_clicked();
    //提示按钮功能（提示用户密码要求）
    void tipsbutton_clicked();
private:
    Ui::ChangePwd *ui;
    QSqlDatabase db;
};

#endif // CHANGEPWD_H
