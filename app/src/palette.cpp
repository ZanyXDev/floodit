#include "palette.h"
#include <QRandomGenerator>
#include <algorithm>

Palette::Palette(QObject *parent)
    : QObject{parent}
    , m_maxColors (4)
    , m_lightMode (true)
{
    m_allColors << "#eb0000" << "#ffa305" << "#003beb" << "#00eb3b" << "#00e6e6" << "#8005ab" << "#f4f512" << "#eb00b0"
                << "#b30000" << "#b37d20" << "#1842bf" << "#00b32d" << "#00b3b3" << "#6f0096" << "#b4b523" << "#b30086";

    setMaxColors(m_maxColors);
    // m_lightColors << "#eb0000" << "#ffa305"<< "#003beb"<< "#00eb3b"<< "#00e6e6"<< "#8005ab" <<"#f4f512"<< "#eb00b0";
    // m_darkColors << "#b30000" << "#b37d20" << "#1842bf" << "#00b32d" << "#00b3b3" << "#6f0096" << "#b4b523" << "#b30086";
}


const QStringList& Palette::colors() const
{
    return m_lightMode ? m_lightColors: m_darkColors ;
}

const QStringList& Palette::allColors() const
{
    return m_allColors;
}

QString Palette::getRandomColor() const
{
    if ( (m_darkColors.isEmpty() ||  m_lightColors.isEmpty())  ) return QString(); // fallback

    int m_rnd = QRandomGenerator::global()->bounded(m_maxColors);

    return (m_lightMode) ? m_lightColors[m_rnd]: m_darkColors[m_rnd];
}

QString Palette::getColorFromId(int id) const
{
    if ( (m_darkColors.isEmpty() ||  m_lightColors.isEmpty())  ) return QString(); // fallback

    if (id < 0)
        id = -id; // make positive

    int m_idx = id % m_maxColors;

    return (m_lightMode) ? m_lightColors[m_idx]: m_darkColors[m_idx];

}

void Palette::setMaxColors(int a_maxColors)
{
    m_maxColors = putInRang(a_maxColors);

    m_lightColors.clear();
    m_darkColors.clear();

    // Создаём список индексов от 0 до  m_maxColors
    QVector<int> indices;
    for (int i = 0; i < m_maxColors; ++i) {
        indices.append(i);
    }
    // Перемешиваем индексы
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    // Берём первые m_maxColors уникальных индексов
    for (int i = 0; i < m_maxColors; ++i) {
        int idx = indices[i];
        m_lightColors.append(m_allColors[idx]);
        m_darkColors.append(m_allColors[idx + 7]); // темный аналог
    }
}

void Palette::setLightMode(bool lightMode)
{
    m_lightMode = lightMode;
}


bool Palette::isInRange(int value, int min, int max) {
    return (value >= min) && (value <= max);
}

int Palette::putInRang(int value) const
{
    if (value <= 4) return 4;
    if (value < 6) return 4;
    if (value < 8) return 6;
    return 8;
}
