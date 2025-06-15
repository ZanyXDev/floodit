#pragma once

#include <QObject>
#include <QtQml/qqml.h>
#include <QVariantMap>
#include <QVariant>
#include <QColor>



class BoardGenerator : public QObject
{
    Q_OBJECT


public:
    explicit BoardGenerator(QObject *parent = nullptr);
    ~BoardGenerator();


signals:
    void dataChanged();

private:
    int m_maxColors; // variant 4,6,8

    QColor getCellColor(int index) const;
};
