#include "graph.h"
#include "tools.h"
#include <QValueAxis>
#include <QFile>
#include <QImage>
#include <QBuffer>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QDateTime>
#include <QDateTimeAxis>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
Graph::Graph(QWidget *parent)
    : QWidget{parent}
{
    this->resize(800,800);
    connect(this,&Graph::download_graph,this,&Graph::DownloadButton_clicked);
}
//销售日金额折线图
void Graph::salerecord_graph_show(){
    // // 获取当前的日期和时间
    // QDateTime currentDateTime = QDateTime::currentDateTime();
    // //折线图从十分钟前开始
    // QDateTime starttime=currentDateTime.addSecs(-600);
    // qDebug()<<starttime.toString();

    // QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    // //qDebug()<<"这里";
    // sdb.setDatabaseName("record.db");
    // if(sdb.open()){
    //     qDebug()<<"数据库打开成功7777";
    // }else{
    //     qDebug()<<"数据库打开失败";
    //     return;
    // }
    // int*values=new int[10]{0};
    // QSqlQuery sql(sdb);
    // sql.exec("select * from salerecord");
    // int index=0;
    // while(sql.next()){
    //     QDateTime time=QDateTime::fromString(sql.value("saletime").toString(),"yyyy/MM/dd hh:mm:ss");
    //     if(starttime.date()!=time.date()){
    //         continue;
    //     }
    //     if(starttime.time().hour()!=time.time().hour()){
    //         continue;
    //     }
    //     qDebug()<<time.toString();
    //     qDebug()<<starttime.time().minute()<<" "<<time.time().minute();
    //     while(starttime.time().minute()<time.time().minute()){
    //         if(index<10){
    //             starttime=starttime.addSecs(60);
    //             index++;
    //             qDebug()<<index;
    //         }else{
    //             break;
    //         }
    //     }
    //     if(starttime.time().minute()==time.time().minute()){
    //         if(index<10){
    //             values[index]+=sql.value("salenumber").toInt()*sql.value("itemprice").toInt();
    //         }
    //     }
    // }
    // sdb.close();
    int*values=init_values("salerecord","salenumber","saletime");
    if(values==nullptr){
        return;
    }
    // 创建横坐标轴
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("时间"); // 设置横坐标轴标题

    // 设置横坐标轴的范围
    axisX->setRange(1, 10);// 设置从1到10
    axisX->setTickCount(10);
    axisX->setLabelFormat("%d");

    // 创建纵坐标轴
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("销售额"); // 设置纵坐标轴标题

    // 设置纵坐标轴的范围
    axisY->setRange(0, 1000000); // 假设数据范围是从0到1000000
    //纵坐标数目
    axisY->setTickCount(18);
    //axisY->setTickInterval(50000);
    // 设置纵坐标轴的标签格式
    axisY->setLabelFormat("%d");
    qDebug()<<"崩溃点";
    // 创建数据点
    series = new QLineSeries();
    for(int i=1;i<=10;++i){
        series->append(i,values[i-1]);
        qDebug()<<values[i-1];
    }
    // 创建图表
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("十分钟销售金额折线图");
    // 将横坐标轴添加到图表中
    chart->addAxis(axisX, Qt::AlignBottom); // Qt::AlignBottom 表示横坐标轴位于底部
    // 将系列与横坐标轴关联
    series->attachAxis(axisX);
    // 将纵坐标轴添加到图表中
    chart->addAxis(axisY, Qt::AlignLeft); // Qt::AlignLeft 表示纵坐标轴位于左侧

    // 将系列与纵坐标轴关联
    series->attachAxis(axisY);
    // 创建图表视图
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 600);

    // 设置窗口布局
    layout = new QGridLayout();
    layout->addWidget(chartView, Qt::AlignCenter, Qt::AlignCenter);
}
int*Graph::init_values(QString recordname,QString numbername,QString timename){
    // 获取当前的日期和时间
    QDateTime currentDateTime = QDateTime::currentDateTime();
    //折线图从十分钟前开始
    QDateTime starttime=currentDateTime.addSecs(-600);
    qDebug()<<starttime.toString();

    QSqlDatabase sdb=QSqlDatabase::addDatabase("QSQLITE");
    //qDebug()<<"这里";
    sdb.setDatabaseName(Tools::projectPath+"/database/"+Tools::folderName+"/record.db");
    if(sdb.open()){
        qDebug()<<"数据库打开成功7777";
    }else{
        qDebug()<<"数据库打开失败";
        return nullptr;
    }
    int*values=new int[10]{0};
    QSqlQuery sql(sdb);
    sql.exec("select * from "+recordname);
    int index=0;
    while(sql.next()){
        QDateTime time=QDateTime::fromString(sql.value(timename).toString(),"yyyy/MM/dd hh:mm:ss");
        if(starttime.date()!=time.date()){
            continue;
        }
        if(starttime.time().hour()!=time.time().hour()){
            continue;
        }
        qDebug()<<time.toString();
        qDebug()<<starttime.time().minute()<<" "<<time.time().minute();
        while(starttime.time().minute()<time.time().minute()){
            if(index<10){
                starttime=starttime.addSecs(60);
                index++;
                qDebug()<<index;
            }else{
                break;
            }
        }
        if(starttime.time().minute()==time.time().minute()){
            if(index<10){
                values[index]+=sql.value(numbername).toInt()*sql.value("itemprice").toInt();
            }
        }
    }
    sdb.close();
    return values;
}
//进货额
void Graph::restockrecord_graph_show(){
    int*values=init_values("restockrecord","number","time");
    if(values==nullptr){
        return;
    }
    // 创建横坐标轴
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("时间"); // 设置横坐标轴标题

    // 设置横坐标轴的范围
    axisX->setRange(1, 10);// 设置从1到10
    axisX->setTickCount(10);
    axisX->setLabelFormat("%d");

    // 创建纵坐标轴
    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("进货额"); // 设置纵坐标轴标题

    // 设置纵坐标轴的范围
    axisY->setRange(0, 1000000); // 假设数据范围是从0到1000000
    //纵坐标数目
    axisY->setTickCount(18);
    //axisY->setTickInterval(50000);
    // 设置纵坐标轴的标签格式
    axisY->setLabelFormat("%d");
    qDebug()<<"崩溃点";
    // 创建数据点
    series = new QLineSeries();
    for(int i=1;i<=10;++i){
        series->append(i,values[i-1]);
        qDebug()<<values[i-1];
    }
    // 创建图表
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("十分钟进货金额折线图");
    // 将横坐标轴添加到图表中
    chart->addAxis(axisX, Qt::AlignBottom); // Qt::AlignBottom 表示横坐标轴位于底部
    // 将系列与横坐标轴关联
    series->attachAxis(axisX);
    // 将纵坐标轴添加到图表中
    chart->addAxis(axisY, Qt::AlignLeft); // Qt::AlignLeft 表示纵坐标轴位于左侧

    // 将系列与纵坐标轴关联
    series->attachAxis(axisY);
    // 创建图表视图
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 600);

    // 设置窗口布局
    layout = new QGridLayout();
    layout->addWidget(chartView, Qt::AlignCenter, Qt::AlignCenter);
}
void Graph::DownloadButton_clicked(){
    // 导出为图片
    // QImage image(800, 600, QImage::Format_ARGB32);
    // QPainter painter(&image);
    // chartView->render(&painter);
    // painter.end();


    //保存文件夹位置
    QString dirpath=QFileDialog::getExistingDirectory(this,"选择文件保存路径","D:/",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    QDir filepath(dirpath);
    if(!filepath.exists()){
        //文件路径不存在
        QMessageBox::warning(this,tr("错误"),tr("导出失败，文件夹不存在"));
        return;
    }
    //文件路径存在
    //输入文件名
    QString filename=QInputDialog::getText(this,"输入文件名","请输入文件名（不用加后缀，指定.png）");
    if(filename.isEmpty()){
        QMessageBox::warning(this,tr("错误"),tr("文件名不能为空"));
        return;
    }
    //文件绝对路径
    QString fileabsoluteoath=filepath.absoluteFilePath(filename+".png");
    // 保存图片到文件
    // if (!image.save(fileabsoluteoath)) {
    //     qDebug() << "保存失败";
    // } else {
    //     qDebug() << "保存成功";
    // }
    // 创建一个QPixmap来捕获QChartView的内容
    QPixmap chartPixmap = chartView->grab();

    // 现在保存QPixmap为图片文件
    bool result = chartPixmap.save(fileabsoluteoath, "PNG");

    if (result) {
        QMessageBox::information(this,tr("提示"),tr("保存成功"));
    } else {
        QMessageBox::information(this,tr("提示"),tr("保存失败"));
    }
}
void Graph::show(){

    // 显示图表
    chartView->show();
}
void Graph::download_information(){
    emit download_graph();
}
