#include "tst_imagegenerator.h"

#include <QtTest/QtTest>


void TestImageGenerator::initTestCase()
{
    m_imageGenerator = new ImageGenerator();
    QVERIFY2( m_imageGenerator!= nullptr,"ImageGenerator is not created!" );
}

void TestImageGenerator::init()
{

}

void TestImageGenerator::cleanupTestCase()
{

}

void TestImageGenerator::testcreateGameBoardImage()
{
    QFETCH(int,cells);
    QFETCH(int,colors);
    QFETCH(bool,lightmode);
    QFETCH(int,imageSize);

    QImage *image = new QImage( imageSize, imageSize, QImage::Format_ARGB32) ;
    QVERIFY2(!image->isNull(),"Image not created!");

    m_imageGenerator->createGameBoardImage(qMakePair(cells, colors),
                                           lightmode,image);

    QCOMPARE(image->width(), imageSize); // Verify image width
    QCOMPARE(image->height(), imageSize); // Verify image height

}

void TestImageGenerator::testcreateGameBoardImage_data()
{
    QTest::addColumn<int>("cells");
    QTest::addColumn<int>("colors");
    QTest::addColumn<bool>("lightmode");
    QTest::addColumn<int>("imageSize");
    QTest::addColumn<bool>("isOK");
    //Valid cases [cells, colors, lightmode, cellsize]
    QTest::newRow("valid: 4_cells, 4_colors, true, 208") <<4 << 4<< true <<208 <<true;
    QTest::newRow("valid: 4_cells, 4_colors, false, 208") <<4 << 4<< true <<208 <<true;

}
