#include "reportplugin.h" 
#include "qraddebug.h"

ReportPlugin::ReportPlugin()
{
}

ReportPlugin::~ReportPlugin()
{
}

void ReportPlugin::onLoad(QRadPluginContainer* container)
{
    m_container = container;

    appendAction("Build");
}

void ReportPlugin::Process( const QString& action )
{
    if (action.toLower() == QString("Build").toLower())
    {
    }
}

void ReportPlugin::setParam(QString str, QVariant v)
{
    Q_UNUSED(str);
    Q_UNUSED(v);
}

QVariant ReportPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}

