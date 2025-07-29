#include "tst_imagegenerator.h"

#include <QtTest/QtTest>


void TestImageGenerator::initTestCase()
{
    m_imageGenerator = new ImageGenerator();
    QVERIFY2( m_imageGenerator!= nullptr,"Imageprovider not created" );
}

void TestImageGenerator::init()
{

}

void TestImageGenerator::cleanupTestCase()
{

}
