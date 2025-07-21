#include "boardmodel.h"

BoardModel::BoardModel(QObject *parent)
    : QAbstractListModel{parent}
{}

QHash<int, QByteArray> BoardModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    if (roles.isEmpty()) {
        roles[FilledRole] = "filled";
        roles[ColorRole] = "color";
    }
    return roles;
}

int BoardModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_data.size();
}

int BoardModel::columnCount(const QModelIndex &parent) const
{
    // QAbstractListModel предполагает одну колонку
    // (т.е. columnCount() == 1)
    // А Роли это не ячейки в строке, а дополнительные ствойства
    // конкретной ячейки!!!!!
    Q_UNUSED(parent);
    return 1;
}

QVariant BoardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_data.size())
        return QVariant();

    const CellItem& cellItem = m_data[index.row()];

    switch(role) {
    case FilledRole:
        return cellItem.m_filled;
    case ColorRole:
        return cellItem.m_color;
    default:
        return QVariant();
    }
}

bool BoardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_data.size())
        return false;

    if (role != FilledRole && role != ColorRole)
        return false;

    CellItem& cellItem = m_data[index.row()];

    bool flag{false};

    switch(role) {
    case FilledRole:
        flag = value.canConvert<bool>();
        if (flag)
            cellItem.m_filled = value.toBool();
        break;
    case ColorRole:
        flag = value.canConvert<QString>();
        if (flag)
            cellItem.m_color  = value.toString();
        break;
    default:
        flag = false;
    }

    /**
 * Необязательный аргумент roles можно использовать для указания того, какие роли
 * данных были фактически изменены. Пустой вектор в аргументе roles означает,
 * что все роли следует считать измененными. Порядок элементов в аргументе
 * roles не имеет значения.
 */
    if (flag) emit dataChanged(index, index, {role} );

    return flag;
}


Qt::ItemFlags BoardModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

}

QModelIndex BoardModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();
    return createIndex(row, column);
}

QModelIndex BoardModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex(); // Плоская структура данных
}

void BoardModel::clear()
{
    beginResetModel();
    m_data.clear();
    endResetModel();
}

void BoardModel::addCell(const QString &v_color)
{
    if ( v_color.isEmpty() || !QColor(v_color).isValid() ) return;

    CellItem cellItem;
    cellItem.m_filled= false;
    cellItem.m_color = v_color;

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data.append(cellItem);
    endInsertRows();
}


