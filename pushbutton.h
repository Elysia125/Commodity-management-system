#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QObject>
#include<QPushButton>
class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = nullptr);

signals:
};

#endif // PUSHBUTTON_H
