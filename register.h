#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include"login.h"
#include<QSqlDatabase>
#include<QSqlQuery>
#include<Qvector>
#include"user.h"
#include <QPixmap>
#include <QPalette>
#include<QPainter>
namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    void connections();
    void connectSQlite();
    bool isValidPassword(const QString &password);
    void keyPressEvent(QKeyEvent*)override;
public slots:
    void registerbutton_clicked();
    void resetbutton_clicked();
    void retbutton_clicked();
    void visiblebutton_1_clicked();
    void visiblebutton_2_clicked();
    void tipsbutton_clicked();
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        // 绘制背景图片，缩放以适应窗口大小
        painter.drawPixmap(rect(), backgroundImage.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
private:
    Ui::Register *ui;
    QSqlDatabase db;
    QVector<User>user;
    bool isvisible_1=false;
    bool isvisible_2=false;
    QPixmap backgroundImage;
};

#endif // REGISTER_H
