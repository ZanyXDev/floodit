#include "tst_imageprovider.h"

#include <QtTest/QtTest>


void TestImageProvider::initTestCase()
{
    m_imageProvider = new ImageProvider();
    engine.addImageProvider("tst_imageprovider_1",  m_imageProvider );

    auto tst_imageprovider_1 = engine.imageProvider("tst_imageprovider_1");
    QVERIFY2(tst_imageprovider_1 != nullptr,"Custom tst_imageprovider_1 not set");

    auto tst_imageprovider_2 = engine.imageProvider("tst_imageprovider_2");
    QVERIFY2(tst_imageprovider_2 == nullptr,"Custom tst_imageprovider_2 set but not added");
}

void TestImageProvider::init()
{

}

void TestImageProvider::cleanupTestCase()
{

}

void TestImageProvider::testRequestImage()
{
    QFETCH(QString, reqImage);
    QFETCH(bool,isExist);
    QFETCH(int,sizeWidth);
    QFETCH(int,sizeHeight);
    QFETCH(int,reqSizeWidth);
    QFETCH(int,reqSizeHeight);

    QSize actualSize;
    QImage image;

    image = m_imageProvider->requestImage(reqImage, &actualSize,
                                          QSize(reqSizeWidth, reqSizeHeight));

    QCOMPARE(image.isNull(),isExist); // Verify image is not null
    QCOMPARE(image.width(), sizeWidth); // Verify image width
    QCOMPARE(image.height(), sizeHeight); // Verify image height
    QCOMPARE(actualSize, QSize(reqSizeWidth, reqSizeHeight)); // Verify reported size

    //image = m_imageProvider->requestImage("unused", nullptr, QSize());

}

void TestImageProvider::testRequestImage_data()
{
    QTest::addColumn<QString>("reqImage");
    QTest::addColumn<bool>("isExist");
    QTest::addColumn<int>("sizeWidth");
    QTest::addColumn<int>("sizeHeight");
    QTest::addColumn<int>("reqSizeWidth");
    QTest::addColumn<int>("reqSizeHeight");

    QTest::newRow("request_wrong_image") << "wrong_image" << true << -1 << -1 << 0 << 0;
}

void TestImageProvider::testGetImageFindKey()
{
    QFETCH(QString, imageId);
    QFETCH(bool,isParsed);
    QFETCH(QString, findKey);

    bool ok = false;
    QString m_findKey = m_imageProvider->getImageFindKey(imageId,ok);

    QCOMPARE(ok ,isParsed );
    if (isParsed) {
        QCOMPARE(m_findKey ,findKey);
    }
}

void TestImageProvider::testGetImageFindKey_data()
{
    QTest::addColumn<QString>("imageId");
    QTest::addColumn<bool>("isParsed");
    QTest::addColumn<QString>("findKey");

    // Valid cases
    QTest::newRow("valid_8x8x4")  << "true_false_8x8x4"    << true << "8x8x4";
    QTest::newRow("valid_8x8x6")  << "true_false_8x8x6"    << true << "8x8x6";
    QTest::newRow("valid_8x8x6")  << "true_false_8x8x8"    << true << "8x8x8";

    QTest::newRow("valid_12x12x4") << "false_true_12x12x4"  << true << "12x12x4";
    QTest::newRow("valid_12x12x6") << "false_true_12x12x6"  << true << "12x12x6";
    QTest::newRow("valid_12x12x8") << "false_true_12x12x8"  << true << "12x12x8";

    QTest::newRow("valid_16x16x4") << "true_true_16x16x4"   << true << "16x16x4";
    QTest::newRow("valid_16x16x6") << "true_true_16x16x6"   << true << "16x16x6";
    QTest::newRow("valid_16x16x8") << "true_true_16x16x8"   << true << "16x16x8";

    QTest::newRow("valid_20x20x4") << "false_false_20x20x4" << true << "20x20x4";
    QTest::newRow("valid_20x20x6") << "false_false_20x20x6" << true << "20x20x6";
    QTest::newRow("valid_20x20x8") << "false_false_20x20x8" << true << "20x20x8";

    // Invalid cases
    QTest::newRow("invalid_format") << "invalid_string" << false << "";
    QTest::newRow("invalid_cells_1") << "true_false_1x1x4" << false << "";
    QTest::newRow("invalid_cells_2") << "true_false_2x3x4" << false << "";
    QTest::newRow("invalid_cells_3") << "true_false_3x3x4" << false << "";
    QTest::newRow("invalid_cells_5") << "true_false_5x5x4" << false << "";
    QTest::newRow("invalid_cells_7") << "true_false_7x7x4" << false << "";
    QTest::newRow("invalid_cells_9") << "true_false_9x9x4" << false << "";
    QTest::newRow("invalid_cells_24") << "true_false_24x24x4" << false << "";
    QTest::newRow("invalid_colors_3") << "false_true_8x8x3" << false << "";
    QTest::newRow("invalid_colors_9") << "false_false_12x12x9" << false << "";
    QTest::newRow("empty_string") << "" << false << "";
    QTest::newRow("wrong_delimiters") << "true.false.8x8x4" << false << "";
}

