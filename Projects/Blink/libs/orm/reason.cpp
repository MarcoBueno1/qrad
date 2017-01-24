#include "reason.h"

reasonList* reason::m_allList = NULL;

MODEL_BEGIN_MAP(reason)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(description, "description");
     MODEL_MAP_FIELD(tp, "tp");
     MODEL_MAP_FIELD(removed, "removed");
MODEL_END_MAP()

QList<reason*>* reason::findAll()
{
    MODEL_INIT_LIST(reason, m_allList);

    QString query = QString("select * from reason");

    if (!reason::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

reason* reason::findByid(int id)
{
    reason *reasonM = new reason();

    QString query = QString("select * from reason where id = %3").arg(id);

    if (!reasonM->fillModelFromQuery(query))
    {
        delete reasonM;
        return NULL;
    }

    return reasonM;
}
reason* reason::findByid(int id, QString database)
{
    return reason::findByPrimaryKey(id, database);
}
