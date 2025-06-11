#pragma once

#include <QObject>

class BoardGenerator : public QObject
{
    Q_OBJECT
public:
    explicit BoardGenerator(QObject *parent = nullptr);

signals:
};
