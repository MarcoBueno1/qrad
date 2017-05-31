#ifndef PLUGINMODEL_H
#define PLUGINMODEL_H


#include <QAbstractTableModel>

#include "qradplugininterface.h"

class PluginModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PluginModel(QObject *parent = NULL);
    ~PluginModel();

    void setPlugin(QRadPluginInterface *iface);

    QString action(const QModelIndex &index);
    QString action(int index);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

private:
    QStringList* m_actionList;
};


#endif // PLUGINMODEL_H
