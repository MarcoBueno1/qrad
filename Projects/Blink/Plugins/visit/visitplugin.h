#ifndef visitPLUGIN_H
#define visitPLUGIN_H

#include <QtCore>

#include "qradplugininterface.h"
#include "qradplugincontainer.h"
#include "managervisit.h"
#include "editvisit.h"
#include "managerVisitante.h"
#include "editdweller.h"
#include "managerdweller.h"
#include "managerpreaut.h"
#include "managerprofile.h"
#include "manageruser.h"
#include "editemailconf.h"
#include "editmaincompany.h"


#define VISIT_MANAGE QString("Manage")
#define VISIT_EDIT QString("Edit")
#define VISIT_MANAGE_VISITANTE QString("ManageVisitante")
#define VISIT_EDIT_DWELLER QString("EditMorador")
#define VISIT_MANAGE_DWELLER QString("ManagerMorador")
#define VISIT_MANAGE_PRE_AUT  QString("ManagerPreAut")
#define VISIT_MANAGE_PROFILE QString("ManagerProfile")
#define VISIT_MANAGE_USER QString("ManagerUser")
#define VISIT_MANAGE_EMAIL_CONFIG QString("EmailConfig")
#define VISIT_MANAGE_MAIN_COMPANY QString("MainCompany")
#define VISIT_MANAGE_BRAND QString("ManageBrand")
#define VISIT_MANAGE_VEHIC_NAME QString("ManageVeicname")
#define VISIT_MANAGE_REASON QString("ManageReason")
#define VISIT_MANAGE_COMMON_AREA QString("ManageCommonArea")
#define VISIT_MANAGE_RESERVE QString("ManageReserve")
#define VISIT_REPORT_RESERVE QString("ReportReserve")


class visitPlugin : public QRadPluginInterface
{
    Q_OBJECT;
    Q_INTERFACES( QRadPluginInterface );
    Q_PLUGIN_METADATA(IID "visitplugin" FILE "visit.json")

public:
     visitPlugin();
    ~visitPlugin();

    void onLoad(QRadPluginContainer*);

protected:
    Managervisit *m_manager;
    ManagerVisitante *m_managerVisitante;
    EditDweller *m_editDweller;
    ManagerDweller *m_managerDweller;
    Editvisit *m_edit;
    Managerpreaut *m_managerPreAut;
    Managerprofile *m_managerProfile;
    Manageruser *m_managerUser;
    Editemailconf *m_emailConfig;
    Editmaincompany *m_maincompany;


private slots:
    void Process( const QString& action );
    void setParam(QString, QVariant);
    QVariant getParam(QString);

private:
    QRadPluginContainer *m_container;
    int m_dwellerid;
};

#endif // visit_H
