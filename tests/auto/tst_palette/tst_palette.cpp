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
    qDebug() << Q_FUNC_INFO <<"Returns colors:" << p.colors();
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

    Palette p;
    auto test_fetch = [&]() {
        QFETCH(int, id);
        QFETCH(int, max_color); /// 4,6,8 colors
        QFETCH(int, return_id);
        p.setMaxColors(max_color);
        QString color = p.getColorFromId(id);
        qDebug() << "Returns color:" << color;
        QVERIFY2(!color.isEmpty(),"Always need return color");
        QVERIFY2(p.colors().contains(color),"Returns color is outside allColors array");
        qDebug() << "Returns colors array:" << p.colors();
        qDebug()  << "Returns colors_id:" << p.colors().indexOf(color);
        QVERIFY2(p.colors().indexOf(color)==return_id,"Returns color id not eq color id in list");

    };

    qDebug("Test lightMode == true [LIGHT]");
    p.setLightMode(true);
    test_fetch();
    qDebug("Test lightMode == false [Dark]");
    p.setLightMode(false);
    test_fetch();
}

void TestPalette::test_getColorFromId_data()
{
    QTest::addColumn<int>("id");
    QTest::addColumn<int>("max_color");
    QTest::addColumn<int>("return_id");

    QTest::newRow("id_-1,4") << -1 << 4  << 1;
    QTest::newRow("id_0,4")  << 0  << 4  << 0;
    QTest::newRow("id_1,4")  << 1  << 4  << 1;
    QTest::newRow("id_2,4")  << 2  << 4  << 2;
    QTest::newRow("id_3,4")  << 3  << 4  << 3;
    QTest::newRow("id_4,4")  << 4  << 4  << 0;
    QTest::newRow("id_5,4")  << 5  << 4  << 1;
    QTest::newRow("id_6,4")  << 6  << 4  << 2;
    QTest::newRow("id_7,4")  << 7  << 4  << 3;
    QTest::newRow("id_8,4")  << 8  << 4  << 0;
    QTest::newRow("id_9,4")  << 9  << 4  << 1;
    QTest::newRow("id_10,4") << 10 << 4  << 2;
    QTest::newRow("id_11,4") << 11 << 4  << 3;

    QTest::newRow("id_-1,6") << -1 << 6  << 1;
    QTest::newRow("id_0,6")  << 0  << 6  << 0;
    QTest::newRow("id_1,6")  << 1  << 6  << 1;
    QTest::newRow("id_2,6")  << 2  << 6  << 2;
    QTest::newRow("id_3,6")  << 3  << 6  << 3;
    QTest::newRow("id_4,6")  << 4  << 6  << 4;
    QTest::newRow("id_5,6")  << 5  << 6  << 5;
    QTest::newRow("id_6,6")  << 6  << 6  << 0;
    QTest::newRow("id_7,6")  << 7  << 6  << 1;
    QTest::newRow("id_8,6")  << 8  << 6  << 2;
    QTest::newRow("id_9,6")  << 9  << 6  << 3;
    QTest::newRow("id_10,6")  << 10  << 6  << 4;
    QTest::newRow("id_11,6")  << 11  << 6  << 5;
    QTest::newRow("id_12,6")  << 12  << 6  << 0;

}





