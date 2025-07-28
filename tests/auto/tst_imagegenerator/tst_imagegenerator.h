#pragma once

#include <QObject>
#include <QtTest/QtTest>
#include "src/imagenerator.h"

// add necessary includes here

class TestImageGenerator: public QObject
{
    Q_OBJECT

public:
    TestImageGenerator(){};
    ~TestImageGenerator(){};

private slots:
    // вызывается перед первой тестовой функцией
    void initTestCase();
    // вызывается перед каждой тестовой функцией
    void init();
    // вызывается после каждой тестовой функции
    void cleanup(){};
    // вызывается после последней тестовой функции
    void cleanupTestCase();
private:

};

