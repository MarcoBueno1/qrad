#include "emailconf.h"

emailconfList* emailconf::m_allList = NULL;

MODEL_BEGIN_MAP(emailconf)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(username, "username");
     MODEL_MAP_FIELD(password, "password");
     MODEL_MAP_FIELD(server, "server");
     MODEL_MAP_FIELD(port, "port");
MODEL_END_MAP()

QList<emailconf*>* emailconf::findAll()
{
    MODEL_INIT_LIST(emailconf, m_allList);

    QString query = QString("select * from emailconf");

    if (!emailconf::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

emailconf* emailconf::findByid(int id)
{
    emailconf *emailconfM = new emailconf();

    QString query = QString("select * from emailconf where id = %3").arg(id);

    if (!emailconfM->fillModelFromQuery(query))
    {
        delete emailconfM;
        return NULL;
    }

    return emailconfM;
}
emailconf* emailconf::findByid(int id, QString database)
{
    return emailconf::findByPrimaryKey(id, database);
}
