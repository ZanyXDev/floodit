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
    void init();
    void test_case1();
};

