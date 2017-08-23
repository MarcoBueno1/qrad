#include "visit.h"

//alter table visit add column visittype integer default 0 ;

visitList* visit::m_allList = NULL;

MODEL_BEGIN_MAP(visit)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(Data, "data_entrada");
     MODEL_MAP_FIELD(Hora, "hora_entrada");
     MODEL_MAP_FIELD(Visitante, "visitante");
     MODEL_MAP_FIELD(Reason, "reason");
     MODEL_MAP_FIELD(Saida, "data_saida");
     MODEL_MAP_FIELD(saida_hora, "saida_hora");
     MODEL_MAP_FIELD(autorizador, "autorizador");
     MODEL_MAP_FIELD(Notified, "notified");
     MODEL_MAP_FIELD(Type, "visittype");
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
visitList * visit::findNotNotified()
{
    MODEL_INIT_LIST(visit, m_allList);

    QString query = QString("select * from visit v inner join dweller d on d.id = v.autorizador and d.notifbyemail = true and v.notified = false and d.removed = false and d.moveout < '2012-01-01' order by v.id desc limit 100");

    if (!visit::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}
