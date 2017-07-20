#include "qradpluginaction.h"
#include "qradshared.h"
#include "qradconfig.h"

QRadPluginAction::QRadPluginAction( const QString& text, QObject* parent )
        : QAction( text, parent )
{
    connect( this, SIGNAL(triggered()), this, SLOT(trigger()) );
}

void QRadPluginAction::trigger()
{
    QApplication::processEvents();

    emit triggered( m_action );
}

void QRadPluginAction::setActionDescriptor( const QString& action )
{
    m_action = action;
}
void QRadPluginAction::setPermission(const QString &permission)
{
    m_permission = permission;
}
void QRadPluginAction::setEnabledEx()
{
    setEnabled( QRadConfig::GetUserProfile(m_permission));
}
