#include "boardgenerator.h"

BoardGenerator::BoardGenerator(QObject *parent)
    : QObject{parent}
    , m_maxColors{4}
    , m_boardModel(new BoardModel(this))
{
    if (m_boardModel){
        QObject::connect(m_boardModel,&BoardModel::dataChanged,this,&BoardGenerator::dataChanged);
    }
}

BoardGenerator::~BoardGenerator()
{
    if (m_boardModel){
        m_boardModel->deleteLater();
    }
}

//--------------------------- Private --------------------------------------------------
QColor BoardGenerator::getCellColor(int index) const
{
    QColor m_color;
    if (index >= 0 && index >= m_boardModel->rowCount() ) {

    }
    return m_color;
}
