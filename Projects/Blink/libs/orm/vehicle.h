#ifndef vehicle_H
#define vehicle_H
 
#include "orm.h"

DECLARE_ORM(vehicle)

DECLARE_CLASS(vehicle)
    ORM_DECLARE_ATTRIBUTE(vehicle, int, id)
    ORM_DECLARE_ATTRIBUTE(vehicle, int, owner)
    ORM_DECLARE_ATTRIBUTE(vehicle, QString, board)
    ORM_DECLARE_ATTRIBUTE(vehicle, int, brand)
    ORM_DECLARE_ATTRIBUTE(vehicle, int, veicname)
    ORM_DECLARE_ATTRIBUTE(vehicle, bool, Removed)
    ORM_MATCH_TABLE(vehicle, "vehicle")
ORM_END_MAP()

/*
DECLARE_MODEL(vehicle)

class vehicle : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(int, owner);
     MODEL_DECLARE_ATTRIBUTE(QString, board);
     MODEL_DECLARE_ATTRIBUTE(int, brand);
     MODEL_DECLARE_ATTRIBUTE(int, veicname);
     MODEL_DECLARE_ATTRIBUTE(bool, Removed);

     MODEL_MATCH_TABLE(vehicle, "vehicle")

private:
    static vehicleList* m_allList;
public:
    static vehicleList* findAll(void);
    static vehicle* findByid(int id);
    static vehicle* findByid(int id, QString database);
};
 */
#endif 
