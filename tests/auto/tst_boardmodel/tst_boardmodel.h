#pragma once

#include <QObject>

// add necessary includes here

class TestBoardModel: public QObject
{
    Q_OBJECT

public:
    TestBoardModel();
    ~TestBoardModel();

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

