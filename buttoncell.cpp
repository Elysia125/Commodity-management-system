// #include<QMessageBox>
// #include<QSqlDatabase>
// #include<QSqlQuery>
// #include<mainwindow.h>
// #include"tools.h"
// ButtonCell::ButtonCell(QWidget *parent)
//     : QWidget{parent}
// {}

// ButtonCell::ButtonCell(productItem*item, QWidget *parent) : QWidget(parent),
//     item(item)
// {
//         this->resize(100,40);
//         QHBoxLayout *layout = new QHBoxLayout(this);
//         layout->setContentsMargins(0, 0, 0, 0); // 去掉内边距
//         item->change=new QPushButton(this);
//         item->change->setText("修改");
//         item->change->resize(40,30);
//         item->del=new QPushButton(this);
//         item->del->setText("删除");
//         item->del->resize(40,30);
//         layout->addWidget(item->change);
//         layout->addWidget(item->del);
//         connect(item->change,&QPushButton::clicked,
//                 this,&ButtonCell::changeButton_clicked);
//         connect(item->del,&QPushButton::clicked,
//                 this,&ButtonCell::delButton_clicked);
//         //qDebug()<<item->pro.getItembrand();
// }

// void ButtonCell::changeButton_clicked(){
//     QPoint point=this->getCellPosition();
//     qDebug()<<point.x();
//     //qDebug()<<Tools::proitem.length();
//     productItem prod=Tools::proitem.at(1);
//     PriductChange diag(prod.pro);
//     diag.exec();
//     (new MainWindow)->show();
// }

// void ButtonCell::delButton_clicked(){
//     QPoint point=this->getCellPosition();
//     productItem prod=Tools::proitem.at(point.x());
//     int result=QMessageBox::warning(this,tr("警告"),tr("确定删除吗")
//                          ,QMessageBox::Yes,QMessageBox::No);

//     if(result==QMessageBox::Yes){
//         QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
//         db.setDatabaseName("productinformation.db");
//         if(db.open()){
//             qDebug()<<"数据库打开成功";
//         }else{
//             qDebug()<<"数据库打开失败";
//         }
//         QSqlQuery sql(db);
//         sql.exec("delete from product where "
//                  "itemnember='"+prod.pro.getItemnumber()+"'");
//         db.close();
//         (new MainWindow)->show();
//     }
// }
