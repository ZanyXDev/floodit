#include "tst_palette.h"
#include "src/palette.h"

#include <QtTest/QtTest>

TestPalette::TestPalette() {}

TestPalette::~TestPalette() {}

void TestPalette::initTestCase()
{
    Palette p;
    //  Test setup defalt value
    QVERIFY2(p.colors().size() == 4,"colors array wrong size"); // default is 4
    QVERIFY2(p.allColors().size() == 16,"allcolors array wrong size");
}

void TestPalette::init()
{

}

void TestPalette::test_getRandomColor()
{

    Palette p;
    QString color = p.getRandomColor();
    QVERIFY2(!color.isEmpty(),"Always need return color");
    QVERIFY2(p.colors().contains(color),"Returns color is outside allColors array");
}

void TestPalette::test_setMaxColors()
{
    Palette p;
    QFETCH(int, maxColor);
    QFETCH(int, result);

    p.setMaxColors(maxColor);
    QCOMPARE(p.colors().size(), result);

}

void TestPalette::test_setMaxColors_data()
{
    QTest::addColumn<int>("maxColor");
    QTest::addColumn<int>("result");

    QTest::newRow("maxColor_-1") << -1 << 4;
    QTest::newRow("maxColor_0") <<  0 << 4;
    QTest::newRow("maxColor_1") <<  1 << 4;
    QTest::newRow("maxColor_2") <<  2 << 4;
    QTest::newRow("maxColor_3") <<  3 << 4;
    QTest::newRow("maxColor_4") <<  4 << 4;
    QTest::newRow("maxColor_5") <<  5 << 4;
    QTest::newRow("maxColor_6") <<  6 << 6;
    QTest::newRow("maxColor_7") <<  7  << 6;
    QTest::newRow("maxColor_8") <<  8  << 8;
    QTest::newRow("maxColor_9") <<  9  << 8;
}

void TestPalette::test_getColorFromId()
{

}

void TestPalette::test_getColorFromId_data()
{
    QTest::addColumn<int>("id");
    QTest::addColumn<bool>("result");

    QTest::newRow("id_-1") << -1 << 4;
}





