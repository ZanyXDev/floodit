#pragma once

#include <QObject>
#include <QDebug>
#include <QtQml/qqml.h>
#include <QVariantMap>
#include <QVariant>

#include "boardmodel.h"

//"Детальное описание алгоритма смотри в тетрадке у Хуня"
// Размерность: 8x8, 12x12, 16x16, 20x20 и 24x24.
// Для каждой размерности поля есть три режима: 4 цвета, 6 цветов и 8 цветов. Итого 15 режимов.

class DataManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(BoardModel* m_boardModel READ boardModel CONSTANT)
    Q_PROPERTY(int maxColors READ getMaxColors WRITE setMaxColors NOTIFY myMaxColorsChanged)
    Q_PROPERTY(int boardSize READ getBoardSize WRITE setBoardSize NOTIFY myBoardSizeChanged)

public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    BoardModel* boardModel() const { return m_boardModel; }
    Q_INVOKABLE void startNewGame();
    int getMaxColors() const;
    void setMaxColors(int newMaxColors);

    int getBoardSize() const;
    void setBoardSize(int newBoardSize);

signals:
    void dataChanged();
    void myMaxColorsChanged();

    void myBoardSizeChanged();

private:
    BoardModel* m_boardModel;

    int m_maxColors;
    int m_boardSize;
};
