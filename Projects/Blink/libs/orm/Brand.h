#ifndef Brand_H
#define Brand_H
 
#include "model.h"

DECLARE_MODEL(Brand)

class Brand : public Model
{


     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(QString, Name);
     MODEL_DECLARE_ATTRIBUTE(int, tp);
     MODEL_DECLARE_ATTRIBUTE(bool, Removed);


     MODEL_MATCH_TABLE(Brand, "brand")

private:
    static BrandList* m_allList;
public:
    static BrandList* findAll(void);
    static Brand* findByid(int id);
    static Brand* findByid(int id, QString database);
};
 
#endif 
