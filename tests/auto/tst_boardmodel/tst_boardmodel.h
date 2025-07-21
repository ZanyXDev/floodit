#pragma once

#include <QObject>
#include "src/boardmodel.h"

// add necessary includes here
#include <QAbstractItemModelTester>

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
    void testRowCount();
    void testData();
    void testSetData();
    void testClear();
    void testAddCell();
    void testDataChanged();
private:
    BoardModel m_model;
    QAbstractItemModelTester* m_tester;
};

