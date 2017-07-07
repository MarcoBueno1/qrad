#include "qradplugininterface.h"

QRadPluginInterface::QRadPluginInterface()
    : QObject()
{
	m_actionList = 0;
        m_descriptor = 0;
}

QRadPluginInterface::~QRadPluginInterface()
{
    if (NULL != m_actionList)
    {
        m_actionList->clear();
        delete m_actionList;
    }
}

QStringList* QRadPluginInterface::actionList()
{
	if (!m_actionList) {
		m_actionList = new QStringList();
	}

	return m_actionList;
}


void QRadPluginInterface::appendAction(QString action)
{
    actionList()->append(action);
}

void QRadPluginInterface::setDescriptor(QRadPluginDescriptor *desc)
{
    m_descriptor = desc;
}

QRadPluginDescriptor* QRadPluginInterface::getDescriptor()
{
    return m_descriptor;
}
