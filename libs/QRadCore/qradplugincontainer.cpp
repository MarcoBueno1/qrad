#include "qradplugincontainer.h"

QRadPluginContainer* QRadPluginContainer::m_instance      = NULL;
int                 QRadPluginContainer::m_instanceCount = 0;

QRadPluginContainer* QRadPluginContainer::getInstance(QRadPluginContainer* /* Deprecated */)
{
    return QRadPluginContainer::getInstance();
}

QRadPluginContainer* QRadPluginContainer::getInstance()
{
    if (NULL == m_instance)
        m_instance = new QRadPluginContainer();

    return m_instance;
}

void QRadPluginContainer::destroy()
{
    // deprecated
}

QRadPluginContainer::QRadPluginContainer()
{
    m_plugins = new QMap<QString, QRadPluginInterface*>();
}

QRadPluginContainer::~QRadPluginContainer()
{
    //delete m_plugins;
}

void QRadPluginContainer::insert(QString pluginName, QRadPluginInterface* iface)
{
    if (!m_plugins->contains(pluginName))
    {
        m_plugins->insert(pluginName, iface);
    }
}

void QRadPluginContainer::executeAction(QString pluginName, QString action)
{
    if (m_plugins->contains(pluginName))
    {
        m_plugins->value(pluginName)->Process(action);
    }
}

QRadPluginInterface* QRadPluginContainer::plugin(QString pluginName)
{
    if (m_plugins->contains(pluginName))
    {
        return m_plugins->value(pluginName);
    }

    return NULL;
}

void QRadPluginContainer::setParam(QString pluginName, QString paramName, QVariant paramValue)
{
    if (m_plugins->contains(pluginName))
    {
        m_plugins->value(pluginName)->setParam(paramName, paramValue);
    }
}

QVariant QRadPluginContainer::getParam(QString pluginName, QString paramName)
{
    QVariant lvariant;

    if (m_plugins->contains(pluginName))
    {
        return m_plugins->value(pluginName)->getParam(paramName);
    }
    return lvariant;
}

QMap<QString, QRadPluginInterface*> *QRadPluginContainer::getMap()
{
    return m_plugins;
}
