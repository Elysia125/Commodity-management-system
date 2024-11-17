
#include <QObject>
#include<QPushButton>
class Product {
public:
    Product();
    Product(const QString &itemnumber, const QString &itemname,
            const QString &itemcategory, const QString &itembrand,
            const QString &itemsupplier, int itemprice,
            int Inventoryquantity, const QString &productdescription);
    Product(const Product&pro);
    //运算符重载
    void operator=(const Product&pro);
    QString getItemnumber() const;
    void setItemnumber(const QString &newItemnumber);

    QString getProductdescription() const;
    void setProductdescription(const QString &newProductdescription);

    QString getItemname() const;
    void setItemname(const QString &newItemname);

    QString getItemcategory() const;
    void setItemcategory(const QString &newItemcategory);

    QString getItembrand() const;
    void setItembrand(const QString &newItembrand);

    QString getItemsupplier() const;
    void setItemsupplier(const QString &newItemsupplier);

    int getItemprice() const;
    void setItemprice(int newItemprice);

    int getInventoryquantity() const;
    void setInventoryquantity(const int &newInventoryquantity);

private:
    QString itemnumber;//商品编号
    QString itemname;//商品名称
    QString itemcategory;//商品类别
    QString itembrand;//品牌
    QString itemsupplier;//供应商
    int itemprice=0;//单价
    int Inventoryquantity=0;//库存数量
    QString productdescription;//商品描述
};

