#include "qradplugin.h"

QRadPlugin::QRadPlugin()
{
    m_QRad = 0;
}

QRadPlugin::~QRadPlugin()
{
    delete m_QRad;
}

void QRadPlugin::onLoad(QRadPluginContainer* container)
{
    m_container = container;

    appendAction("show");
}

void QRadPlugin::show()
{
    if (!m_QRad)
        m_QRad = new QRad(m_parent);

    QRadConfig::centralizarWidget(m_QRad);
    m_QRad->show();
}

void QRadPlugin::Process( const QString& action )
{
    if (action == "show") {
        show();
    }
}

void QRadPlugin::setParam(QString str, QVariant v)
{
    Q_UNUSED(str);
    Q_UNUSED(v);
}

QVariant QRadPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}
