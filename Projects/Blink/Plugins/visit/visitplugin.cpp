#include "visitplugin.h" 
#include "qraddebug.h"
#include "emailconf.h"
#include "managerBrand.h"
#include "managerveicname.h"
#include "managerreason.h"

visitPlugin::visitPlugin()
{
    m_manager = 0;
    m_edit = 0;
    m_managerVisitante = 0;
    m_editDweller = 0;
    m_managerDweller = 0;
    m_managerPreAut = 0;
    m_managerProfile = 0 ;
    m_managerUser =0;
    m_emailConfig = 0;
    m_maincompany = 0; 

}

visitPlugin::~visitPlugin()
{
    if( m_manager )
        delete m_manager;

    if( m_edit )
        delete m_edit;

    if( m_managerVisitante )
        delete m_managerVisitante;

    if( m_editDweller )
        delete m_editDweller;

    if(m_managerDweller)
        delete m_managerDweller;

    if(m_managerPreAut)
        delete m_managerPreAut ;

    if(m_managerProfile)
        delete m_managerProfile;
 
    if(m_managerUser)
        delete m_managerUser;

    if(m_emailConfig)
        delete m_emailConfig;
   
    if( m_maincompany )
        delete m_maincompany;

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
        m_manager->exec();
    }
    else if (action.toLower() == QString("Edit").toLower())
    {
        if (m_edit)
            delete m_edit;

            m_edit = new Editvisit(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_edit->exec();
    }
    else if (action.toLower() == QString("ManageVisitante").toLower())
    {
        if (!m_managerVisitante)
            m_managerVisitante = new ManagerVisitante(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_managerVisitante->exec();
    }
    else if (action.toLower() == QString("EditMorador").toLower())
    {
        debug_message("-->EditMorador\n");
        if (m_editDweller )
            delete m_editDweller;

        m_editDweller = new EditDweller(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_editDweller->exec();
        debug_message("<--EditMorador\n");
    }
    else if (action.toLower() == QString("ManagerMorador").toLower())
    {
        debug_message("-->ManagerMorador\n");
        if (m_managerDweller )
            delete m_managerDweller;

        m_managerDweller = new ManagerDweller(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_managerDweller->exec();
        debug_message("<--ManagerMorador\n");
    }
    else if (action.toLower() == QString("ManagerPreAut").toLower())
    {
        debug_message("-->ManagerPreAut\n");
        if (m_managerPreAut )
            delete m_managerPreAut;

        m_managerPreAut = new Managerpreaut(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_managerPreAut->exec();
        debug_message("<--ManagerPreAut\n");
    }
    else if (action.toLower() == QString("ManagerProfile").toLower())
    {
        debug_message("-->ManagerProfile\n");
        if (m_managerProfile )
            delete m_managerProfile;

        m_managerProfile = new Managerprofile(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_managerProfile->exec();
        debug_message("<--ManagerProfile\n");
    }
    else if (action.toLower() == QString("ManagerUser").toLower())
    {
        debug_message("-->ManagerUser\n");
        if (m_managerUser )
            delete m_managerUser;

        m_managerUser = new Manageruser(m_parent);

    //    QRadConfig::centralizarWidget(m_edit);
        m_managerUser->exec();
        debug_message("<--ManagerUser\n");
    }
    else if(action.toLower() == QString("EmailConfig").toLower())
    {
        debug_message("-->EmailConfig\n");
        if( m_emailConfig )
            delete m_emailConfig;

        m_emailConfig =  new Editemailconf(m_parent);
        emailconf *conf = emailconf::findByid(1);
        if( conf )
            m_emailConfig->SetModel(conf);
        m_emailConfig->exec();
        debug_message("<--EmailConfig\n");
    }
    else if(action.toLower() == QString("MainCompany").toLower())
    {
        debug_message("-->MainCompany\n");
        if( m_maincompany )
            delete m_maincompany;

        m_maincompany  =  new Editmaincompany(m_parent);
        maincompany *conf = maincompany::findByid(1);
        if( conf )
            m_maincompany->SetModel(conf);
        m_maincompany->exec();
        debug_message("<--MainCompany\n");
    }

    else if(action.toLower() == QString("ManageBrand").toLower())
    {
        debug_message("-->ManageBrand\n");

        ManagerBrand *Brand  =  new ManagerBrand(m_parent);
        Brand->exec();
        delete Brand;
        debug_message("<--ManageBrand\n");
    }

    else if(action.toLower() == QString("ManageVeicname").toLower())
    {
        debug_message("-->ManageVeicname\n");

        Managerveicname *VeicName  =  new Managerveicname(m_parent);
        VeicName->exec();
        delete VeicName;
        debug_message("<--ManageVeicname\n");
    }
    else if(action.toLower() == QString("ManageReason").toLower())
    {
        debug_message("-->ManageReason\n");

        Managerreason *reason  =  new Managerreason(m_parent);
        reason->exec();
        delete reason;
        debug_message("<--ManageReason\n");
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

