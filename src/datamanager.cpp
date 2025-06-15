#include "datamanager.h"

DataManager::DataManager(QObject *parent)
    : QObject{parent}
    , m_boardModel(new BoardModel(this))
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
