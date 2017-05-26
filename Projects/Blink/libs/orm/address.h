#ifndef address_H
#define address_H
 
#include "model.h"

typedef enum
{
   tpDweller,
   tpVisit,
   tpSupplier
}ADDRESS_TYPE, PADDRESS_TYPE;

DECLARE_MODEL(Address)

class Address : public Model
{
     MODEL_DECLARE_ATTRIBUTE(int, id);
     MODEL_DECLARE_ATTRIBUTE(int, street);
     MODEL_DECLARE_ATTRIBUTE(QString, number);
     MODEL_DECLARE_ATTRIBUTE(int, cep);
     MODEL_DECLARE_ATTRIBUTE(int, Neighborhood);
     MODEL_DECLARE_ATTRIBUTE(int, city);
     MODEL_DECLARE_ATTRIBUTE(int, state);
     MODEL_DECLARE_ATTRIBUTE(int, Owner);
     MODEL_DECLARE_ATTRIBUTE(int, OwnerType);
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
