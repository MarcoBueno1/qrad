#ifndef PROPERTYTABLEMODEL_H
#define PROPERTYTABLEMODEL_H


#include <QAbstractTableModel>

#include "qradplugininterface.h"

class PropertyTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    PropertyTableModel(QRadPluginInterface *iface = NULL);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void clear();
    void appendProperty(QString propName);
    void setInterface(QRadPluginInterface *iface);

private:
    QStringList m_propList;
    QRadPluginInterface *m_iface;

public slots:
    void update();
};


#endif // PROPERTYTABLEMODEL_H
