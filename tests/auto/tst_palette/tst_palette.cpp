#include "tst_palette.h"

#include <QtTest/QtTest>

TestPalette::TestPalette() {}

TestPalette::~TestPalette() {}

void TestPalette::initTestCase()
{
    auto colorList = m_palette.allColors();
    QVERIFY2(!colorList.isEmpty(),"All colors list is empty!");

}

void TestPalette::init()
{
}

void TestPalette::cleanup()
{

}

void TestPalette::cleanupTestCase()
{

}

void TestPalette::test_getRandomColor()
{
    auto randomColor = m_palette.getRandomColor();
    QVERIFY2(!randomColor.isEmpty(),"Get empty string.");
}


