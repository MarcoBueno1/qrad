#ifndef Phone_H
#define Phone_H
 
#include "model.h"

DECLARE_MODEL(Phone)

class Phone : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(int, dwellerid );
     MODEL_DECLARE_ATTRIBUTE(QString, Number);
     MODEL_DECLARE_ATTRIBUTE(int, Operator);
     MODEL_DECLARE_ATTRIBUTE(int, Type);
     MODEL_DECLARE_ATTRIBUTE(bool, WatsApp);
     MODEL_DECLARE_ATTRIBUTE(bool, Removed);

     MODEL_MATCH_TABLE(Phone, "phone")

private:
    static PhoneList* m_allList;
public:
    static PhoneList* findAll(void);
    static Phone* findByid(int id);
    static Phone* findByid(int id, QString database);
};
 
#endif 
