#ifndef TOOLS_H
#define TOOLS_H

#include <QDir>
#include<QString>
#include<QVector>
#include"productitem.h"
#include<QSet>
class Tools{
public:
    //记录当前项目文件路径
    static QString projectPath;
    //记录存放文件的文件夹名
    static QString folderName;
    //记录用户名
    static QString username;
    //记录用户密码
    static QString password;
    //记录用户头像
    static QString userimage;
    //记录用户电话
    static QString phonenumber;
    //记录用户邮箱
    static QString emailnumber;
    //记录用户性别
    static QString sex;
    //记录用户个人简介
    static QString selfedit;
    //记录用户生日
    static QString birthday;

    //存储商品信息
    static QVector<productItem>proitem;

    //存储商品种类
    static QVector<QString>kinds;

    //删除记录
    static QVector<Product>delrecord;

    //库存种类
    static QSet<QString>inventorykinds;
};

#endif // TOOLS_H
