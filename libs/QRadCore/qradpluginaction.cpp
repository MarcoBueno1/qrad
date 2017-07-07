#include "qradpluginaction.h"
#include "qradshared.h"

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
