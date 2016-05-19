#include "visit.h"

visitList* visit::m_allList = NULL;

MODEL_BEGIN_MAP(visit)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(Data, "data_entrada");
     MODEL_MAP_FIELD(Hora, "hora_entrada");
     MODEL_MAP_FIELD(Visitante, "visitante");
     MODEL_MAP_FIELD(Reason, "reason");
     MODEL_MAP_FIELD(Saida, "data_saida");
     MODEL_MAP_FIELD(saida_hora, "saida_hora");
MODEL_END_MAP()

QList<visit*>* visit::findAll()
{
    MODEL_INIT_LIST(visit, m_allList);

    QString query = QString("select * from visit");

    if (!visit::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

visit* visit::findByid(int id)
{
    visit *visitM = new visit();

    QString query = QString("select * from visit where id = %3").arg(id);

    if (!visitM->fillModelFromQuery(query))
    {
        delete visitM;
        return NULL;
    }

    return visitM;
}
visit* visit::findByid(int id, QString database)
{
    return visit::findByPrimaryKey(id, database);
}
