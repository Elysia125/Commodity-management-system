// #include <QApplication>
// #include <QTableWidget>
// #include <QWidget>
// #include <QPushButton>
// #include <QHBoxLayout>
// #include"productitem.h"
// class ButtonCell : public QWidget
// {
//     Q_OBJECT
// public:
//     ButtonCell(productItem*item, QWidget *parent = nullptr);

//     ButtonCell(QWidget *parent = nullptr);


// public slots:
//     void changeButton_clicked();
//     void delButton_clicked();
// signals:


// private:
//     productItem*item;
//     QPoint getCellPosition() const {
//         auto *tableWidget = qobject_cast<QTableWidget*>(parentWidget());
//         if (!tableWidget) {
//             return {-1, -1}; // 或者抛出异常，取决于你的错误处理策略
//         }

//         // 将本地坐标转换为全局坐标，再转换为相对于tableWidget的坐标
//         QPoint posInTable = mapTo(tableWidget, pos());

//         // 使用indexAt方法找到单元格位置
//         QModelIndex index = tableWidget->indexAt(posInTable);
//         if (!index.isValid()) {
//             return {-1, -1}; // 或者抛出异常
//         }

//         return {index.row(), index.column()};
//     }
// };
