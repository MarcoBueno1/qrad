#include "preaut.h"

preautList* preaut::m_allList = NULL;

MODEL_BEGIN_MAP(preaut)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(sunday, "sunday");
     MODEL_MAP_FIELD(monday, "monday");
     MODEL_MAP_FIELD(tuesday, "tuesday");
     MODEL_MAP_FIELD(wednesday, "wednesday");
     MODEL_MAP_FIELD(thursday, "thursday");
     MODEL_MAP_FIELD(friday, "friday");
     MODEL_MAP_FIELD(saturday, "saturday");
     MODEL_MAP_FIELD(visit, "visit");
     MODEL_MAP_FIELD(horaini, "horaini");
     MODEL_MAP_FIELD(horafim, "horafim");
     MODEL_MAP_FIELD(validate, "validate");
     MODEL_MAP_FIELD(authorizer, "authorizer");
     MODEL_MAP_FIELD(obs, "obs");
     MODEL_MAP_FIELD(autsince, "autsince");
     MODEL_MAP_FIELD(reason, "reason");
     MODEL_MAP_FIELD(destination, "destination");
     MODEL_MAP_FIELD(tp, "tp");
     MODEL_MAP_FIELD(NoAnnounce, "noannounce");
     MODEL_MAP_FIELD(removed, "removed");
MODEL_END_MAP()

QList<preaut*>* preaut::findAll()
{
    MODEL_INIT_LIST(preaut, m_allList);

    QString query = QString("select * from preaut");

    if (!preaut::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

preaut* preaut::findByid(int id)
{
    preaut *preautM = new preaut();

    QString query = QString("select * from preaut where id = %3").arg(id);

    if (!preautM->fillModelFromQuery(query))
    {
        delete preautM;
        return NULL;
    }

    return preautM;
}
preaut* preaut::findByid(int id, QString database)
{
    return preaut::findByPrimaryKey(id, database);
}

preaut* preaut::findValidByVisitor( int visitor )
{
  QString query = QString( "select * from preaut where visit = %1 "\
                            "  and (current_time between  horaini and horafim) and "\
                            "        (select case when (select extract(DOW from current_date)) = 0 then sunday "\
                            "                     when (select extract(DOW from current_date)) = 1 then monday "\
                            "                     when (select extract(DOW from current_date)) = 2 then tuesday "\
                            "                     when (select extract(DOW from current_date)) = 3 then wednesday "\
                            "                     when (select extract(DOW from current_date)) = 4 then thursday "\
                            "                     when (select extract(DOW from current_date)) = 5 then friday "\
                            "                     when (select extract(DOW from current_date)) = 6 then saturday "\
                            "                    end ) = true and (removed <> true) and  (validate > current_date) limit 1").arg(visitor);

    preaut *preautM = new preaut();
    if (!preautM->fillModelFromQuery(query))
    {
        delete preautM;
        return NULL;
    }

    return preautM;
}
