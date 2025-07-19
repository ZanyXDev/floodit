#pragma once

#include <QObject>

// add necessary includes here

class TestPalette : public QObject
{
    Q_OBJECT

public:
    TestPalette();
    ~TestPalette();

private slots:
    // вызывается перед первой тестовой функцией
    void initTestCase();
    // вызывается перед каждой тестовой функцией
    void init();
    // вызывается после каждой тестовой функции
    void cleanup(){};
    // вызывается после последней тестовой функции
    void cleanupTestCase(){};

    void test_getRandomColor();

    void test_setMaxColors();
    void test_setMaxColors_data();

    void test_getColorFromId();
    void test_getColorFromId_data();
};

