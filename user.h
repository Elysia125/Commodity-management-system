#ifndef USER_H
#define USER_H

#include <QObject>
#include<QString>
class User
{
public:
    User();
    User(QString name,QString pwd){
        this->username=name;
        this->password=pwd;
    }
    User(const User& other) // 拷贝构造函数
        : username(other.username), password(other.password)
    {
    }
    void operator=(const User& other){
        username=other.username;
        password=other.password;
    }
    void setusername(QString name){
        this->username=name;
    }
    QString getusername(){
        return username;
    }
    void setpassword(QString password){
        this->password=password;
    }
    QString getpassword(){
        return this->password;
    }

    bool equal(User other){
        if(this->username==other.username&&
            this->password==other.password){
            return true;
        }
        return false;
    }
private:
    QString username;
    QString password;
signals:
};

#endif // USER_H
