#include <QTabBar>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include<QStyle>
#include<QStyleOption>
class HorizontalTextTabBar : public QTabBar {
    Q_OBJECT
public:
    HorizontalTextTabBar(QWidget *parent = nullptr);

protected:
    QSize tabSizeHint(int index) const override {
        QStyleOptionTab option;
        initStyleOption(&option, index);
        QRect visualRect = style()->subElementRect(QStyle::SE_TabBarTabShape, &opt, this);
        int tabWidth = visualRect.height();
        int tabHeight = visualRect.width();
        return QSize(tabWidth, tabHeight);
    }

    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::TextAntialiasing);

        QFontMetrics fm = fontMetrics();
        int tabWidth = fm.horizontalAdvance("W");
        int tabHeight = height();

        for (int i = 0; i < count(); ++i) {
            QString text = tabText(i);
            int textWidth = fm.horizontalAdvance(text);
            int textHeight = fm.height();
            int x = (tabWidth - textWidth) / 2;
            int y = (tabHeight - textHeight) / 2;

            QRect rect(0, i * tabHeight, tabWidth, tabHeight);
            painter.drawText(rect.adjusted(x, y, x, y), Qt::AlignCenter, text);
        }
    }
};
