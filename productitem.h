#ifndef PRODUCTITEM_H
#define PRODUCTITEM_H

#include <QObject>
#include "priductchange.h"
class productItem : public QObject
{
    Q_OBJECT
public:
    explicit productItem(QObject *parent = nullptr);
    productItem(const Product &pro);
    productItem(const productItem &proitem);
    // Product getPro() const;
    // void setPro(const Product &newPro);
    //该类将商品信息界面每一行的商品信息与操作及修改界面绑定起来
    productItem&operator=(const productItem &proitem);
    Product pro;
    //PriductChange*diag;
    //QPushButton*change;//修改操作
    //QPushButton*del;//删除操作
signals:

};

#endif // PRODUCTITEM_H
