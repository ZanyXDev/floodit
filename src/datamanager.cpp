#include "datamanager.h"

DataManager::DataManager(QObject *parent)
    : QObject{parent}
    , m_boardModel(new BoardModel(this))
    , m_maxColors (4) // 4 uniq colors
    , m_boardSize (8) // board 8x8 cells
    , m_colorMode (false) // use light mode
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

void DataManager::startNewGame(int v_boardSize, int v_maxColors, bool v_mode)
{
    if (m_boardModel){
        m_boardModel->clear();
        this->setBoardSize( v_boardSize );
        this->setMaxColors( v_maxColors );
        this->setColorMode( v_mode );
        for (int i=0; i< v_boardSize*v_boardSize; ++i){
            setCellColor( i, m_pallete.getRandomColor());
        }
    }else {
        QString data = tr("Error: Board model not found !!!");
        emit errorInfo( data );
    }
}

void DataManager::setBoardSize(int v_boardSize)
{
    m_boardSize = v_boardSize;
    emit boardSizeChanged();
}

void DataManager::setMaxColors(int v_maxColors)
{
    m_maxColors = v_maxColors;
    m_pallete.setMaxColors( m_maxColors );
    emit maxColorsChanged();
}

void DataManager::setColorMode(bool v_mode)
{
    m_colorMode = v_mode;
    m_pallete.setColorMode( v_mode );
    emit colorModeChanged();
}

int DataManager::getBoardSize() const
{
    return m_boardSize;
}

int DataManager::getMaxColors() const
{
    return m_maxColors;
}

bool DataManager::getColorMode() const
{
    return m_colorMode;
}

bool DataManager::setCellColor(int index, const QVariant &value)
{
    qDebug() << "index:" <<index <<" value:" <<value;
    return setCellProperty(index,value, m_boardModel->ColorRole);
}

bool DataManager::setCellProperty(int index, const QVariant &value, int role) {
    if (index < 0 || index >= m_boardModel->rowCount()) return false;
    QModelIndex idx = m_boardModel->index(index, 0);
    return m_boardModel->setData(idx, value, role);
}
