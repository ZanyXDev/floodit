#include "tst_boardmodel.h"

#include <QtTest/QtTest>

TestBoardModel::TestBoardModel() {}

TestBoardModel::~TestBoardModel() {}

void TestBoardModel::initTestCase()
{
    m_tester = new QAbstractItemModelTester(&m_model);
}

void TestBoardModel::init()
{
    this->testClear();
}

void TestBoardModel::testRowCount()
{
    QCOMPARE(m_model.rowCount(), 0);
    m_model.addCell("#FF0000");
    QCOMPARE(m_model.rowCount(), 1);
}

void TestBoardModel::testData()
{
    m_model.addCell("#00FF00");
    QModelIndex idx = m_model.index(0, 0);
    QVERIFY2(idx.isValid(),"For first item index must be 0 and valid.");

    QCOMPARE(m_model.data(idx, BoardModel::FilledRole).toBool(), false);
    QCOMPARE(m_model.data(idx, BoardModel::ColorRole).value<QColor>(), QColor("#00FF00"));
}

void TestBoardModel::testSetData()
{
    m_model.addCell("#000000");
    QModelIndex idx = m_model.index(0, 0);
    QVERIFY2(idx.isValid(),"For first item index must be 0 and valid.");

    QVERIFY(m_model.setData(idx, "#FF0000", BoardModel::ColorRole));
    QCOMPARE(m_model.data(idx, BoardModel::ColorRole).value<QColor>(), QColor("#FF0000"));

    QVERIFY(m_model.setData(idx, true, BoardModel::FilledRole));
    QCOMPARE(m_model.data(idx, BoardModel::FilledRole).toBool(), true);

    //Проверка что не изменился цвет для не поддерживаемых ролей
    QCOMPARE(m_model.setData(idx, "#FF0003", Qt::DisplayRole),false);
    QCOMPARE(m_model.data(idx, Qt::DisplayRole), QVariant());
}

void TestBoardModel::testClear()
{
    m_model.addCell("#0000FF");
    m_model.clear();
    QCOMPARE(m_model.rowCount(), 0);
}

void TestBoardModel::testAddCell()
{
    QSignalSpy rowInsertedSpy(&m_model, &BoardModel::rowsInserted);

    m_model.addCell("#0000FF");

    QVERIFY(rowInsertedSpy.isValid());
    QCOMPARE(m_model.rowCount(), 1);
    QVERIFY(rowInsertedSpy.count() == 1);
}

void TestBoardModel::testDataChanged()
{
    QSignalSpy dataChangedSpy(&m_model, &BoardModel::dataChanged);

    m_model.addCell("#000000");
    QModelIndex idx = m_model.index(0, 0);
    QVERIFY2(idx.isValid(),"For first item index must be 0 and valid.");
    QVERIFY(dataChangedSpy.isValid());

    auto test_role = [&](bool mode) {
        // Проверяем, что сигнал испускался один раз
        QCOMPARE(dataChangedSpy.count(), 1);

        // Получаем аргументы сигнала
        auto args = dataChangedSpy.takeFirst();

        // Проверяем topLeft и bottomRight
        QCOMPARE(args.at(0).value<QModelIndex>(), idx);
        QCOMPARE(args.at(1).value<QModelIndex>(), idx);

        // Проверяем, что роль была в списке изменённых ролей
        QVector<int> roles = args.at(2).value<QVector<int>>();
        qDebug() << "roles:" <<roles;
        if ( mode ) {
            QVERIFY( roles.contains(BoardModel::ColorRole) ) ;
        }
        else{
            QVERIFY( roles.contains(BoardModel::FilledRole) );
        }
    };
    // Изменяем цвет
    QVERIFY(m_model.setData(idx, "#FF0000", BoardModel::ColorRole));
    test_role(true);
    QVERIFY(m_model.setData(idx, false, BoardModel::FilledRole));
    test_role(false);

    //Проверка что не изменился цвет
    QCOMPARE(m_model.setData(idx, "#FF0003", Qt::DisplayRole),false);
    QCOMPARE(m_model.data(idx, Qt::DisplayRole), QVariant());

    // Проверяем что не поддерживаемые роли не вызвают сигнал dataChanged
    QCOMPARE(m_model.setData(idx, "#FF0003", Qt::DisplayRole),false);
    QCOMPARE(dataChangedSpy.count(), 0);
}

