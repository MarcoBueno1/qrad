#ifndef QRadPLUGINCONTAINER_H
#define QRadPLUGINCONTAINER_H

#include <QObject>
#include <QMap>

#include "qradplugininterface.h"

#define QRad_PLUGINCONTAINER_PORT 9501

class QRadPluginContainer : public QObject
{
    Q_OBJECT

private:
    QRadPluginContainer();
    ~QRadPluginContainer();

    static QRadPluginContainer* m_instance;
    static int                 m_instanceCount;

public:
    static QRadPluginContainer* getInstance(QRadPluginContainer* container);
    static QRadPluginContainer* getInstance();
    void destroy();

    void insert(QString pluginName, QRadPluginInterface* iface);
    void executeAction(QString pluginName, QString action);
    void setParam(QString pluginName, QString paramName, QVariant paramValue);
    QVariant getParam(QString pluginName, QString paramName);
    QRadPluginInterface* plugin(QString pluginName);
    QMap<QString, QRadPluginInterface*> *getMap();

private:
    QMap<QString, QRadPluginInterface*> *m_plugins;
};

#endif // QRadPLUGINCONTAINER_H
