#ifndef Tower_H
#define Tower_H
 
#include "model.h"

DECLARE_MODEL(Tower)

class Tower : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, Name);

     MODEL_MATCH_TABLE(Tower, "tower")

private:
    static TowerList* m_allList;
public:
    static TowerList* findAll(void);
    static Tower* findByid(int id);
    static Tower* findByid(int id, QString database);
};
 
#endif 
