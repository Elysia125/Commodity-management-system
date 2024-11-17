#include "tools.h"
QString Tools::projectPath=QDir::currentPath();
QString Tools::folderName=NULL;
QString Tools::username="username";
QString Tools::password="000000";
QString Tools::userimage=":/images/images/user.png";
QString Tools::birthday=NULL;
QString Tools::phonenumber=NULL;
QString Tools::emailnumber=NULL;
QString Tools::sex=NULL;
QString Tools::selfedit=NULL;
QVector<productItem> Tools::proitem(0);
QVector<QString>Tools::kinds={"全部"};
QVector<Product>Tools::delrecord(0);
QSet<QString>Tools::inventorykinds={"全部"};
