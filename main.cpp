
#include <QApplication>
#include"login.h"
//#include"horizontaltexttabbar.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //qRegisterMetaType<HorizontalTextTabBar*>("HorizontalTextTabBar*");
    Login login;
    login.show();
    return a.exec();
}
