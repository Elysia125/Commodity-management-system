#include <QApplication>
#include <QTabWidget>
#include <QStylePainter>
#include <QStyleOptionTab>

class CustomTabWidget : public QTabWidget
{
public:
    CustomTabWidget(QWidget *parent = nullptr) : QTabWidget(parent) {}

protected:
    void paintEvent(QPaintEvent *event) override
    {
        QStylePainter painter(this);
        for (int i = 0; i < count(); ++i)
        {
            QStyleOptionTab tab;
            initStyleOption(&tab);
            tab.rect.translate(-tab.rect.width(), 0); // 将tab的位置移动到右侧
            painter.drawControl(QStyle::CE_TabBarTabShape, tab);
            painter.drawText(tab.rect, Qt::AlignCenter, tabText(i));
        }
    }
};
