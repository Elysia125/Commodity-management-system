#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QGridLayout>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QPushButton>
class Graph : public QWidget
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = nullptr);

    void salerecord_graph_show();
    void download_information();
    void show();
    void restockrecord_graph_show();
    int *init_values(QString recordname, QString numbername, QString timename);
public slots:
    void DownloadButton_clicked();
signals:
    void download_graph();
private:
    //void connectSQL(QString dbname, QString tablename, QString itemname);

    QGridLayout*layout;
    QChart *chart;
    QChartView *chartView;
    QLineSeries *series;

};

#endif // GRAPH_H
