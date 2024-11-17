#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>
#include
class MyButton : public QObject
{
    Q_OBJECT
public:
    explicit MyButton(QObject *parent = nullptr);

signals:
};

#endif // MYBUTTON_H
