#ifndef machine_H
#define machine_H

#include "orm.h"

DECLARE_ORM(machine)

DECLARE_CLASS(machine)
    ORM_DECLARE_ATTRIBUTE(machine, int, id)
    ORM_DECLARE_ATTRIBUTE(machine, QString, Machine)
    ORM_MATCH_TABLE(machine, "machine")
ORM_END_MAP()


/*
 
#include "model.h"

DECLARE_MODEL(machine)

class machine : public Model
{

     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, machine);

     MODEL_MATCH_TABLE(machine, "machine")

private:
    static machineList* m_allList;
public:
    static machineList* findAll(void);
    static machine* findByid(int id);
    static machine* findByid(int id, QString database);
    static machine* findByMachine(QString Machine);
};

*/
 
#endif 
