#include "visitplugin.h" 

visitPlugin::visitPlugin()
{
    m_manager = 0;
    m_edit = 0;
    m_managerVisitante = 0;
}

visitPlugin::~visitPlugin()
{
    if( m_manager )
        delete m_manager;

    if( m_edit )
        delete m_edit;

    if( m_managerVisitante )
        delete m_managerVisitante;
}

void visitPlugin::onLoad(QRadPluginContainer* container)
{
    m_container = container;

        appendAction("Manage");
    appendAction("Edit");
}

void visitPlugin::Process( const QString& action )
{
    if (action.toLower() == QString("Manage").toLower())
    {
        if (!m_manager)
            m_manager = new Managervisit(m_parent);

    //    QRadConfig::centralizarWidget(m_manager);
        m_manager->show();
    }
    else if (action.toLower() == QString("Edit").toLower())
    {
        if (!m_edit)
            m_edit = new Editvisit(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_edit->show();
    }
    else if (action.toLower() == QString("ManageVisitante").toLower())
    {
        if (!m_managerVisitante)
            m_managerVisitante = new ManagerVisitante(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_managerVisitante->show();
    }

}

void visitPlugin::setParam(QString str, QVariant v)
{
    Q_UNUSED(str);
    Q_UNUSED(v);
}

QVariant visitPlugin::getParam(QString str)
{
    Q_UNUSED(str);
    return QVariant();
}

