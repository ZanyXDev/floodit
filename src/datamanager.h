#pragma once

#include <QObject>
#include <QDebug>
#include <QtQml/qqml.h>
#include <QVariantMap>
#include <QVariant>

#include "boardmodel.h"
#include "palette.h"

// "Детальное описание алгоритма смотри в тетрадке у Хуня"
// Размерность: 8x8, 12x12, 16x16, 20x20 и 24x24.
// Для каждой размерности поля есть три режима: 4 цвета, 6 цветов и 8 цветов. Итого 15 режимов.

class DataManager : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(BoardModel* boardModel READ boardModel CONSTANT)
    Q_PROPERTY(int maxColors READ getMaxColors WRITE setMaxColors NOTIFY maxColorsChanged)
    Q_PROPERTY(int boardSize READ getBoardSize WRITE setBoardSize NOTIFY boardSizeChanged)
    Q_PROPERTY(bool colorMode READ getColorMode WRITE setColorMode NOTIFY colorModeChanged)
public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    BoardModel* boardModel() const { return m_boardModel; }

    Q_INVOKABLE void startNewGame(int v_boardSize, int v_maxColors, bool v_mode);

    void setBoardSize(int v_boardSize);
    void setMaxColors(int v_maxColors);
    void setColorMode(bool v_mode);

    int  getMaxColors() const;
    int  getBoardSize() const;
    bool getColorMode() const;

    bool setCellColor(int index, const QVariant &value);
signals:
    void dataChanged();
    void maxColorsChanged();
    void boardSizeChanged();
    void colorModeChanged();
    void errorInfo(const QString &info);

private:
    BoardModel* m_boardModel;
    Palette m_pallete;

    int m_maxColors;
    int m_boardSize;
    bool m_colorMode;
    bool setCellProperty(int index, const QVariant &value, int role);
};
