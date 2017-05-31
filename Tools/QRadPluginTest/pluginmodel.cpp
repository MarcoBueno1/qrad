#include "pluginmodel.h"

PluginModel::PluginModel(QObject *parent)
     : QAbstractTableModel(parent)
{
    m_actionList = new QStringList();
}

PluginModel::~PluginModel()
{
    delete m_actionList;
}

void PluginModel::setPlugin(QRadPluginInterface *iface)
{
    m_actionList->clear();

    if (NULL == iface)
    {
        beginResetModel();
        endResetModel();
//        reset();
        return;
    }

    if (!iface->actionList())
        return;

    foreach(QString action, *iface->actionList())
    {
        m_actionList->append(action);
    }

    beginResetModel();
    endResetModel();
//    reset();
}

QString PluginModel::action(int index)
{
    if (index < 0 || index >= m_actionList->count())
        return "";

    return m_actionList->at(index);
}

QString PluginModel::action(const QModelIndex &index)
{
    if (!index.isValid())
        return "";

    return action(index.row());
}

int PluginModel::rowCount(const QModelIndex &) const
{
    return m_actionList->count();
}

int PluginModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant PluginModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || Qt::DisplayRole != role)
        return QVariant();

    if (index.row() < 0)
        return QVariant();

    switch(index.column())
    {
        case 0: return m_actionList->at(index.row());
    }

    return QVariant();
}

QVariant PluginModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (Qt::Horizontal == orientation)
    {
        return QString("Action");
    }
    else
    {
        return QString("%1").arg(section);
    }
}


