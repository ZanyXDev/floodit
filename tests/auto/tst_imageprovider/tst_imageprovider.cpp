#include "tst_imageprovider.h"

#include <QtTest/QtTest>


void TestImageProvider::initTestCase()
{
    m_imageProvider = new ImageProvider();
    engine.addImageProvider("dynamic_image",  m_imageProvider );
}

void TestImageProvider::init()
{

}

void TestImageProvider::cleanupTestCase()
{

}

