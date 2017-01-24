#include "veicname.h"

veicnameList* veicname::m_allList = NULL;

MODEL_BEGIN_MAP(veicname)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(type, "type");
     MODEL_MAP_FIELD(tp, "tp");
     MODEL_MAP_FIELD(Removed, "removed");

MODEL_END_MAP()

QList<veicname*>* veicname::findAll()
{
    MODEL_INIT_LIST(veicname, m_allList);

    QString query = QString("select * from veicname");

    if (!veicname::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

veicname* veicname::findByid(int id)
{
    veicname *veicnameM = new veicname();

    QString query = QString("select * from veicname where id = %3").arg(id);

    if (!veicnameM->fillModelFromQuery(query))
    {
        delete veicnameM;
        return NULL;
    }

    return veicnameM;
}
veicname* veicname::findByid(int id, QString database)
{
    return veicname::findByPrimaryKey(id, database);
}
