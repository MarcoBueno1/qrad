#include "phone.h"

PhoneList* Phone::m_allList = NULL;

MODEL_BEGIN_MAP(Phone)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(dwellerid, "dwellerid" );
     MODEL_MAP_FIELD(Number, "number");
     MODEL_MAP_FIELD(Operator, "operator");
     MODEL_MAP_FIELD(Type, "type");
     MODEL_MAP_FIELD(WatsApp, "watsapp" );
     MODEL_MAP_FIELD(Removed, "removed");

     MODEL_END_MAP()

QList<Phone*>* Phone::findAll()
{
    MODEL_INIT_LIST(Phone, m_allList);

    QString query = QString("select * from phone");

    if (!Phone::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

Phone* Phone::findByid(int id)
{
    Phone *PhoneM = new Phone();

    QString query = QString("select * from phone where id = %3").arg(id);

    if (!PhoneM->fillModelFromQuery(query))
    {
        delete PhoneM;
        return NULL;
    }

    return PhoneM;
}
Phone* Phone::findByid(int id, QString database)
{
    return Phone::findByPrimaryKey(id, database);
}
