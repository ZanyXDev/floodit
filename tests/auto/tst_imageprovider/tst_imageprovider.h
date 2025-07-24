#pragma once

#include <QObject>
#include <QtTest/QtTest>
#include <QQmlEngine>
#include "src/imageprovider.h"

// add necessary includes here

class TestImageProvider: public QObject
{
    Q_OBJECT

public:
    TestImageProvider(){};
    ~TestImageProvider(){};

private slots:
    // вызывается перед первой тестовой функцией
    void initTestCase();
    // вызывается перед каждой тестовой функцией
    void init();
    // вызывается после каждой тестовой функции
    void cleanup(){};
    // вызывается после последней тестовой функции
    void cleanupTestCase();
    void testRequestImage();
    void testRequestImage_data();
    void testGetImageFindKey();
    void testGetImageFindKey_data();
private:
    ImageProvider *m_imageProvider;
    QQmlEngine engine;
};

