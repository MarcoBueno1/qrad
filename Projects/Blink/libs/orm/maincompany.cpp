#include "maincompany.h"

maincompanyList* maincompany::m_allList = NULL;

MODEL_BEGIN_MAP(maincompany)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(name, "name");
     MODEL_MAP_FIELD(fantasyname, "fantasyname");
     MODEL_MAP_FIELD(cnpj, "cnpj");
     MODEL_MAP_FIELD(ie, "ie");
     MODEL_MAP_FIELD(phone, "phone");
     MODEL_MAP_FIELD(email, "email");
     MODEL_MAP_FIELD(license, "license");
     MODEL_MAP_FIELD(city, "city");
     MODEL_MAP_FIELD(state, "state");
MODEL_END_MAP()

QList<maincompany*>* maincompany::findAll()
{
    MODEL_INIT_LIST(maincompany, m_allList);

    QString query = QString("select * from maincompany");

    if (!maincompany::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

maincompany* maincompany::findByid(int id)
{
    maincompany *maincompanyM = new maincompany();

    QString query = QString("select * from maincompany where id = %3").arg(id);

    if (!maincompanyM->fillModelFromQuery(query))
    {
        delete maincompanyM;
        return NULL;
    }

    return maincompanyM;
}
maincompany* maincompany::findByid(int id, QString database)
{
    return maincompany::findByPrimaryKey(id, database);
}
