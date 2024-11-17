QT       += core gui sql charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttoncell.cpp \
    changepwd.cpp \
    delrecord.cpp \
    graph.cpp \
    information.cpp \
    inventoryproductchange.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    mylineedit.cpp \
    priductchange.cpp \
    product.cpp \
    productitem.cpp \
    record.cpp \
    register.cpp \
    tools.cpp \
    user.cpp \
    userinfo.cpp

HEADERS += \
    buttoncell.h \
    changepwd.h \
    delrecord.h \
    graph.h \
    information.h \
    inventoryproductchange.h \
    login.h \
    mainwindow.h \
    mylineedit.h \
    priductchange.h \
    product.h \
    productitem.h \
    record.h \
    register.h \
    tools.h \
    user.h \
    userinfo.h

FORMS += \
    changepwd.ui \
    delrecord.ui \
    information.ui \
    inventoryproductchange.ui \
    login.ui \
    mainwindow.ui \
    priductchange.ui \
    record.ui \
    register.ui \
    userinfo.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    myimages.qrc

# DISTFILES += \
#     ../../QTXlsxWriter/QtXlsxWriter-master/README.md

# SUBDIRS += \
#     ../../QTXlsxWriter/QtXlsxWriter-master/qtxlsx.pro

#include(qtxlsx\src\xlsx\qtxlsx.pri)
include(QXlsx/QXlsx.pri)
