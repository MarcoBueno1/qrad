#include "vehicle.h"

vehicleList* vehicle::m_allList = NULL;

MODEL_BEGIN_MAP(vehicle)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(owner, "owner");
     MODEL_MAP_FIELD(board, "board");
     MODEL_MAP_FIELD(brand, "brand");
     MODEL_MAP_FIELD(veicname, "veicname");
     MODEL_MAP_FIELD(Removed, "removed");
MODEL_END_MAP()

QList<vehicle*>* vehicle::findAll()
{
    MODEL_INIT_LIST(vehicle, m_allList);

    QString query = QString("select * from vehicle");

    if (!vehicle::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

vehicle* vehicle::findByid(int id)
{
    vehicle *vehicleM = new vehicle();

    QString query = QString("select * from vehicle where id = %3").arg(id);

    if (!vehicleM->fillModelFromQuery(query))
    {
        delete vehicleM;
        return NULL;
    }

    return vehicleM;
}
vehicle* vehicle::findByid(int id, QString database)
{
    return vehicle::findByPrimaryKey(id, database);
}
