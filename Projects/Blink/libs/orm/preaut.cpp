#include "preaut.h"

PreautList* Preaut::m_allList = NULL;

MODEL_BEGIN_MAP(Preaut)
     MODEL_MAP_PRIMARYKEY(id, "id"); 
     MODEL_MAP_FIELD(Visit, "visit");
     MODEL_MAP_FIELD(Reason, "reason");
     MODEL_MAP_FIELD(AutSince, "autsince");
     MODEL_MAP_FIELD(Validate, "validate");
     MODEL_MAP_FIELD(EnterTime, "entertime");
     MODEL_MAP_FIELD(Sunday, "sunday");
     MODEL_MAP_FIELD(Tuesday, "tuesday");
     MODEL_MAP_FIELD(Wednesday, "wednesday");
     MODEL_MAP_FIELD(Thursday, "thursday");
     MODEL_MAP_FIELD(Friday, "friday");
     MODEL_MAP_FIELD(Saturday, "aturday");
	 
	 MODEL_END_MAP()

QList<Preaut*>* Preaut::findAll()
{
    MODEL_INIT_LIST(Preaut, m_allList);

    QString query = QString("select * from preaut");

    if (!Preaut::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Preaut* Preaut::findByid(int id)
{
    Preaut *PreautM = new Preaut();

    QString query = QString("select * from preaut where id = %3").arg(id);

    if (!PreautM->fillModelFromQuery(query))
    {
        delete PreautM;
        return NULL;
    }

    return PreautM;
}
Preaut* Preaut::findByid(int id, QString database)
{
    return Preaut::findByPrimaryKey(id, database);
}
