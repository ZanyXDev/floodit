#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QVariantMap>
#include <QVariant>
#include <QColor>

#include "boardmodel.h"

//"Детальное описание алгоритма смотри в тетрадке у Хуня"

class BoardGenerator : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(BoardModel* m_boardModel READ boardModel CONSTANT)

public:
    explicit BoardGenerator(QObject *parent = nullptr);
    ~BoardGenerator();

    BoardModel* boardModel() const { return m_boardModel; }

signals:
    void dataChanged();

private:
    int m_maxColors; // variant 4,6,8
    BoardModel* m_boardModel;

    QColor getCellColor(int index) const;
};
