#include "datamanager.h"

DataManager::DataManager(QObject *parent)
    : QObject{parent}
    , m_boardModel(new BoardModel(this))
    , m_maxColors(4)
    , m_boardSize(8)
{
    if (m_boardModel){
        QObject::connect(m_boardModel,&BoardModel::dataChanged,this,&DataManager::dataChanged);
    }
}

DataManager::~DataManager()
{
    if (m_boardModel){
        m_boardModel->deleteLater();
    }
}

void DataManager::startNewGame()
{
    if (m_boardModel){
        m_boardModel->clear();

    }
}

int DataManager::getMaxColors() const
{
    return m_maxColors;
}

void DataManager::setMaxColors(int newMaxColors)
{
    if (m_maxColors == newMaxColors)
        return;
    m_maxColors = newMaxColors;
    emit myMaxColorsChanged();
}

int DataManager::getBoardSize() const
{
    return m_boardSize;
}

void DataManager::setBoardSize(int newBoardSize)
{
    if (m_boardSize == newBoardSize)
        return;
    m_boardSize = newBoardSize;
    emit myBoardSizeChanged();
}
