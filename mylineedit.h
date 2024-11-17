#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QApplication>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>

class MyLineEdit : public QLineEdit {
    Q_OBJECT

public:
    MyLineEdit(QWidget *parent = nullptr):QLineEdit(parent) {
        setPlaceholderText("时间格式:年/月/日 时/分/秒");
    }

protected:
    //当文本框获得焦点时
    void focusInEvent(QFocusEvent *event) override {
        QLineEdit::focusInEvent(event);
        if (text().isEmpty()) {
            setPlaceholderText("");
        }
    }
    //当文本框失去焦点时
    void focusOutEvent(QFocusEvent *event) override {
        QLineEdit::focusOutEvent(event);
        if (text().isEmpty()) {
            setPlaceholderText("时间格式:年/月/日 时/分/秒");
        }
    }
};

#endif // MYLINEEDIT_H
