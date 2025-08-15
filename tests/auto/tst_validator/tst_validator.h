#pragma once

#include <QObject>

// add necessary includes here

class TestValidator : public QObject
{
    Q_OBJECT

public:
    TestValidator();
    ~TestValidator();

private slots:
    // вызывается перед первой тестовой функцией
    void initTestCase();
    // вызывается перед каждой тестовой функцией
    void init();
    // вызывается после каждой тестовой функции
    void cleanup(){};
    // вызывается после последней тестовой функции
    void cleanupTestCase(){};

};

