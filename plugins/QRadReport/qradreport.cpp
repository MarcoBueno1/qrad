#include "qradreport.h"

QRadReportPlugin::QRadReportPlugin()
{


}

QRadReportPlugin::~QRadReportPlugin()
{


}

void QRadReportPlugin::onLoad(QRadPluginContainer* container)
{
    m_container = container;

    appendAction("Manage");
    appendAction("Edit");
}

void QRadReportPlugin::Process( const QString& action )
{

}

void QRadReportPlugin::setParam(QString str, QVariant v)
{
    Q_UNUSED(str);
    Q_UNUSED(v);
}

QVariant QRadReportPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}

