#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QToolButton>
#include<Qwidget>
#include"productitem.h"
#include<QVector>
#include<QSqlDatabase>
#include<QSqlQuery>
#include"buttoncell.h"
#include<QTableWidgetItem>
#include"graph.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void connections();
    void menus_to_usertoolbutton();
    void init();
    void connectSql();
    void inserttoTable(productItem*pro, int row);
    void connectSql_for_saleRecord();
    void init_for_saleRecord();
    void windowsFlush_for_salerecordDiag();
    void init_for_inventory();
    void windowsFlush_for_inventoryDiag();
    void connectSql_for_Inventory();
    void windowsFlush_for_restockDiag();
    void init_for_restockRecord();
    void connectSql_for_restockRecord();
    int *init_values(QString recordname, QString numbername, QString timename);
    void firstcharts();
    void secondcharts();
    void windowFlush_for_home();
    QString selectDownloadPath();

public slots:
    void exitAction_clicked();
    void userinfoAction_clicked();
    void changepwdAction_clicked();
    void homepageButton_clicked();
    void commoditypageButton_clicked();
    void inventoryquantitypageButton_clicked();
    void purchasepageButton_clicked();
    void salerecordpageButton_clicked();
    void addproductButton_clicked();
    void windowsFlush();
    void windowsFlush_for_commodityDiag();
    void item_clicked(QTableWidgetItem *item);
    void SearchForCondition();
    void DatatoXlsx();
    void delRecord();
    void item_clicked_for_saletable(QTableWidgetItem *item);
    void SearchForCondition_forsaleRecord();
    void DatatoXlsx_for_salerecord();
    void DataClear();
    void datatograph();
    void downloadGraph();
    void item_clicked_for_Inventory(QTableWidgetItem *item);
    void inputAction();
    void DatatoXlsx_for_inventory();
    //void DataClear_for_inventory();
    void SearchForCondition_for_inventory();
    void outputRecord();
    void inputRecord();
    void DataClear_for_inventory();
    void downloadGraph_for_restockDiag();
    void DataClear_for_restockDiag();
    void datatograph_for_restockDiag();
    void DatatoXlsx_for_restockRecord();
    void SearchForCondition_for_restockRecord();
    void item_clicked_for_restocktable(QTableWidgetItem *item);
    void setWarningThreshold2();
    void setWarningThreshold1();
    void salevolumeButton_clicked();
    void costButton_clicked();
    void profitButton_clicked();
    void informationButton_clicked();
    void downloadButton1_clicked();
    void downloadButton2_clicked();
    void init_for_home();
private:
    Ui::MainWindow *ui;
    QMenu*usermenu;

    QAction*userinfo;
    QAction*changepwd;
    QAction*exitlogin;

    QToolButton*user;
    // QMenu*changepwd;
    // QMenu*exitlogin;

    //定义五个界面
    //首页
    QWidget*homepage;
    //商品信息页
    QWidget*commoditypage;
    //库存数量页
    QWidget*inventoryquantitypage;
    //进货记录页
    QWidget*purchaserecordpage;
    //销售记录页
    QWidget*salerecordpage;

    //存储上商品页每一行商品信息
    QVector<productItem> proitem;
    //数据库
    QSqlDatabase db;

    //添加商品
    QAction*addProductAction;
    //删除的商品的历史记录
    QAction*delProductAction;
    //将数据保存到表格中
    QAction*restoreDatatoCsv;
    //工具栏菜单
    QMenu*toolMenu;


    //出售记录页的QAction
    //导出表格
    QAction*restoreDatatoxlsx;
    //下载图表
    QAction*dataforgraph;
    //清空数据
    QAction*dataClear;
    //下载图表
    QAction *downgraph;
    //工具栏菜单
    QMenu*toolMenu_5;

    //图表
    Graph*salerecordgraph;


    bool isvaild(QString inputtime, QString time);


    //库存界面
    QAction*inputstorage;//入库
    QAction*datatoxlsx;  //导出表格
    QAction*inputrecord;//入库记录
    QAction*outputrecord;//出库记录
    QAction*clearAction;//清库操作
    QAction*WarningThreshold1;//预警阈值（小）
    QAction*WarningThreshold2;//预警阈值（大）
    //工具栏菜单
    QMenu*toolMenu_2;

    //进货记录页的QAction
    //导出表格
    QAction*Datatoxlsx;
    //下载图表
    QAction*datagraph;
    //清空数据
    QAction*Cleardata;
    //下载图表
    QAction *graphdownload;
    //工具栏菜单
    QMenu*toolMenu_7;

    //图表
    Graph*restockrecordgraph;
    void TableSet(QTableWidget *tableWidget);


    //销售额
    int salevolume;
    //成本
    int cost;
    //分别记录低于预警阈值（小）和高于（大）的库存编号
    QSet<QString>ID1;

    QSet<QString>ID2;
    //数据点
    QLineSeries *series1;
    QLineSeries *series2;
    //图表
    QChart *chart;
    QChartView *chartView;

    //数据
    int*values1;
    int*values2;


    QLineSeries *series3;
    QChart *chart2;
    QChartView *chartView2;

    //定义一个定时器
    QTimer*time;
};
#endif // MAINWINDOW_H
