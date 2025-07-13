#pragma once

#include <QObject>
#include "src/palette.h"

// add necessary includes here

class TestPalette : public QObject
{
    Q_OBJECT

public:
    TestPalette();
    ~TestPalette();

private :
    Palette m_palette;

private slots:
    // вызывается перед первой тестовой функцией
    void initTestCase();
    // вызывается перед каждой тестовой функцией
    void init();
    // вызывается после каждой тестовой функции
    void cleanup();
    // вызывается после последней тестовой функции
    void cleanupTestCase();

    void test_getRandomColor();
};

