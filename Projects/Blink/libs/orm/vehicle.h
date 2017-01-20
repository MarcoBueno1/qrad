#ifndef vehicle_H
#define vehicle_H
 
#include "model.h"

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
 
#endif 
