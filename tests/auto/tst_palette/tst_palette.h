#include <QtTest>
#include "src/palette.h"

// add necessary includes here

class TestPalette : public QObject
{
    Q_OBJECT

public:
    TestPalette();
    ~TestPalette();

private slots:
    Palette m_palette;

    void test_case1();
};

