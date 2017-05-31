#include "propertytablemodel.h"

PropertyTableModel::PropertyTableModel(QRadPluginInterface *iface)
{
    m_iface = iface;
}

int PropertyTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_propList.count();
}

int PropertyTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 2;
}

QVariant PropertyTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ((orientation == Qt::Vertical) || (role != Qt::DisplayRole))
        return QVariant();

    switch (section)
    {
        case 0: return QString::fromUtf8("Nome");
        case 1: return QString::fromUtf8("Valor");
    } // switch

    return QVariant();
}

QVariant PropertyTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (index.column())
    {
        case 0:
        {
            return m_propList.at(index.row());
        }
        break;

        case 1:
        {
            if (NULL != m_iface)
            {
                return m_iface->property(m_propList.at(index.row()).toUtf8().data());
            }
        }
        break;
    } // break;

    return QVariant();
}

void PropertyTableModel::appendProperty(QString propName)
{
    m_propList.append(propName);
    beginResetModel();
    endResetModel();
}

void PropertyTableModel::clear()
{
    m_propList.clear();
    beginResetModel();
    endResetModel();
}

void PropertyTableModel::setInterface(QRadPluginInterface *iface)
{
    m_iface = iface;
}

void PropertyTableModel::update()
{
    beginResetModel();
    endResetModel();;
}

