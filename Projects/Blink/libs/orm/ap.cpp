#include "ap.h"

ApList* Ap::m_allList = NULL;

MODEL_BEGIN_MAP(Ap)
     MODEL_MAP_PRIMARYKEY(id, "id");
	 
     MODEL_MAP_FIELD(Number, "number");
     MODEL_MAP_FIELD(Tower, "tower");
     MODEL_MAP_FIELD(m2, "m2");

	 MODEL_END_MAP()

QList<Ap*>* Ap::findAll()
{
    MODEL_INIT_LIST(Ap, m_allList);

    QString query = QString("select * from ap");

    if (!Ap::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Ap* Ap::findByid(int id)
{
    Ap *ApM = new Ap();

    QString query = QString("select * from ap where id = %3").arg(id);

    if (!ApM->fillModelFromQuery(query))
    {
        delete ApM;
        return NULL;
    }

    return ApM;
}
Ap* Ap::findByid(int id, QString database)
{
    return Ap::findByPrimaryKey(id, database);
}
