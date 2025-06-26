#pragma once

#include <QObject>
#include <QStringList>

class Palette : public QObject
{
    Q_OBJECT
public:
    explicit Palette(QObject *parent = nullptr);

    QString getRandomColor() const;
    QString getColorFromId(int id) const;
    const QStringList& colors() const;
    const QStringList& allColors() const;
    void setMaxColors(int a_maxColors);
    void setLightMode (bool lightMode);

private:
    int m_maxColors;
    bool m_lightMode;
    QStringList m_lightColors;
    QStringList m_darkColors;
    QStringList m_allColors;

    bool isInRange(int value, int min, int max);
};
