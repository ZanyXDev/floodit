#pragma once

#include <QObject>
#include "src/myclass.h"

class TestMyClass : public QObject
{
    Q_OBJECT
private:
    MyClass myClass;
private slots:
    void init();
    void testAdd();
    void testMultiply();
};
