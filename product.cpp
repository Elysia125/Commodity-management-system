#include "product.h"

Product::Product(){
}
Product::Product(const Product&pro):
    itemnumber(pro.itemnumber),
    itemname(pro.itemname),
    itemcategory(pro.itemcategory),
    itembrand(pro.itembrand),
    itemsupplier(pro.itemsupplier),
    itemprice(pro.itemprice),
    Inventoryquantity(pro.Inventoryquantity),
    productdescription(pro.productdescription){

}
QString Product::getItemnumber() const
{
    return itemnumber;
}

void Product::setItemnumber(const QString &newItemnumber)
{
    itemnumber = newItemnumber;
}

QString Product::getProductdescription() const
{
    return productdescription;
}

void Product::setProductdescription(const QString &newProductdescription)
{
    productdescription = newProductdescription;
}

QString Product::getItemname() const
{
    return itemname;
}

void Product::setItemname(const QString &newItemname)
{
    itemname = newItemname;
}

QString Product::getItemcategory() const
{
    return itemcategory;
}

void Product::setItemcategory(const QString &newItemcategory)
{
    itemcategory = newItemcategory;
}

QString Product::getItembrand() const
{
    return itembrand;
}

void Product::setItembrand(const QString &newItembrand)
{
    itembrand = newItembrand;
}

QString Product::getItemsupplier() const
{
    return itemsupplier;
}

void Product::setItemsupplier(const QString &newItemsupplier)
{
    itemsupplier = newItemsupplier;
}

int Product::getItemprice() const
{
    return itemprice;
}

void Product::setItemprice(int newItemprice)
{
    itemprice = newItemprice;
}

int Product::getInventoryquantity() const
{
    return Inventoryquantity;
}

void Product::setInventoryquantity(const int &newInventoryquantity)
{
    Inventoryquantity = newInventoryquantity;
}

Product::Product(const QString &itemnumber, const QString &itemname,
                 const QString &itemcategory, const QString &itembrand, const QString &itemsupplier,
                 int itemprice, int Inventoryquantity, const QString &productdescription) :
    itemnumber(itemnumber),
    itemname(itemname),
    itemcategory(itemcategory),
    itembrand(itembrand),
    itemsupplier(itemsupplier),
    itemprice(itemprice),
    Inventoryquantity(Inventoryquantity),
    productdescription(productdescription)
{}

void Product::operator=(const Product&pro){
    itemnumber=pro.itemnumber;
    itemname=pro.itemname;
    itemcategory=pro.itemcategory;
    itembrand=pro.itembrand;
    itemsupplier=pro.itemsupplier;
    itemprice=pro.itemprice;
    Inventoryquantity=pro.Inventoryquantity;
    productdescription=pro.productdescription;
}
