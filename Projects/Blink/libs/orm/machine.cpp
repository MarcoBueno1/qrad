#include "machine.h"

machineList* machine::m_allList = NULL;

MODEL_BEGIN_MAP(machine)
     MODEL_MAP_PRIMARYKEY(id, "id");
     MODEL_MAP_FIELD(machine, "machine");
MODEL_END_MAP()

QList<machine*>* machine::findAll()
{
    MODEL_INIT_LIST(machine, m_allList);

    QString query = QString("select * from machine");

    if (!machine::fillModelList(m_allList, query))
        return NULL;

    return m_allList;
}

machine* machine::findByid(int id)
{
    machine *machineM = new machine();

    QString query = QString("select * from machine where id = %3").arg(id);

    if (!machineM->fillModelFromQuery(query))
    {
        delete machineM;
        return NULL;
    }

    return machineM;
}
machine* machine::findByMachine(QString Machine)
{
    machine *machineM = new machine();

    QString query = QString("select * from machine where machine = '%3'").arg(Machine);

    if (!machineM->fillModelFromQuery(query))
    {
        delete machineM;
        return NULL;
    }

    return machineM;
}
machine* machine::findByid(int id, QString database)
{
    return machine::findByPrimaryKey(id, database);
}
