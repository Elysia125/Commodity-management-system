#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<Qvector>
#include"user.h"
#include"mainwindow.h"
#include <QPixmap>
#include <QPalette>
#include<QPainter>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void connections();
    //bool isright();
    void connectSQlite();
    void keyPressEvent(QKeyEvent*)override;
    void init();
public slots:
    void loginbutton_clicked();
    void registerbutton_clicked();
    void visiblebutton_clicked();
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        // 绘制背景图片，缩放以适应窗口大小
        painter.drawPixmap(rect(), backgroundImage.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
private:
    Ui::Login *ui;
    //bool right=false;
    QSqlDatabase db;
    QVector<User>user;
    MainWindow*w;
    bool isvisible=false;
    QPixmap backgroundImage;
};

#endif // LOGIN_H
