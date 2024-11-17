#include "pushbutton.h"
#include <QtWidgets>
#include<QPushButton>
PushButton::PushButton(QWidget *parent)
    : QPushButton{parent}
{
    this->setStyleSheet("QPushButton {"
                         "    border: 2px solid;"  // 设置边框样式
                         "    border-radius: 5px;"  // 设置圆角
                         "}");
}
