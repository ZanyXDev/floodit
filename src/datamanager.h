#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QtQml/qqml.h>
#include "boardmodel.h"

//"Детальное описание алгоритма смотри в тетрадке у Хуня"

class DataManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT    
    Q_PROPERTY(BoardModel* m_boardModel READ boardModel CONSTANT)
public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();
    BoardModel* boardModel() const { return m_boardModel; }

signals:
    void dataChanged();
private:
    BoardModel* m_boardModel;
};
