#include "login.h"

loginList* login::m_allList = NULL;

MODEL_BEGIN_MAP(login)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(user, "userid");
     MODEL_MAP_FIELD(date, "date");
     MODEL_MAP_FIELD(time, "time");
MODEL_END_MAP()

QList<login*>* login::findAll()
{
    MODEL_INIT_LIST(login, m_allList);

    QString query = QString("select * from login");

    if (!login::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

login* login::findByid(int id)
{
    login *loginM = new login();

    QString query = QString("select * from login where id = %3").arg(id);

    if (!loginM->fillModelFromQuery(query))
    {
        delete loginM;
        return NULL;
    }

    return loginM;
}
login* login::findByid(int id, QString database)
{
    return login::findByPrimaryKey(id, database);
}
