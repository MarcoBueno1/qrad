#ifndef Address_H
#define Address_H
 
#include "model.h"

DECLARE_MODEL(Address)

class Address : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(int, Street);
     MODEL_DECLARE_ATTRIBUTE(int, Number);
     MODEL_DECLARE_ATTRIBUTE(int, cep);
     MODEL_DECLARE_ATTRIBUTE(int, Neighborhood);
     MODEL_DECLARE_ATTRIBUTE(bool, Removed);

     MODEL_MATCH_TABLE(Address, "address")

private:
    static AddressList* m_allList;
public:
    static AddressList* findAll(void);
    static Address* findByid(int id);
    static Address* findByid(int id, QString database);
};
 
#endif 
