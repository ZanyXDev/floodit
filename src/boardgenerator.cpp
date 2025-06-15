#include "boardgenerator.h"

BoardGenerator::BoardGenerator(QObject *parent)
    : QObject{parent}
    , m_maxColors{4}

{

}

BoardGenerator::~BoardGenerator()
{

}

//--------------------------- Private --------------------------------------------------
QColor BoardGenerator::getCellColor(int index) const
{
    QColor m_color;

    return m_color;
}
