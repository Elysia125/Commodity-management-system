#include "productitem.h"

productItem::productItem(QObject *parent)
    : QObject{parent}
{
    //this->change=new QPushButton;
   // this->del=new QPushButton;
}

// Product productItem::getPro() const
// {
//     return pro;
// }

// void productItem::setPro(const Product &newPro)
// {
//     pro = newPro;
// }

productItem::productItem(const Product &pro) : pro(pro)
{
    //修改商品界面与商品信息绑定起来
    //this->diag=new PriductChange(pro);
}
productItem::productItem(const productItem&proitem){
    this->pro=proitem.pro;
    //this->diag=new PriductChange(this->pro);
    // this->change=new QPushButton;
    // this->del=new QPushButton;
}
productItem&productItem::operator=(const productItem&proitem){
    if(this!=&proitem){
        this->pro=proitem.pro;
        //this->diag=new PriductChange(this->pro);
        // this->change=new QPushButton;
        // this->del=new QPushButton;
    }
    return*this;
}
