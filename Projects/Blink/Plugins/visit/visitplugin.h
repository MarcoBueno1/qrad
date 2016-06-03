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



private slots:
    void Process( const QString& action );
    void setParam(QString, QVariant);
    QVariant getParam(QString);

private:
    QRadPluginContainer *m_container;
};

#endif // visit_H
