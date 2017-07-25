#ifndef ADDRESSV2_H
#define ADDRESSV2_H

#include "orm.h"

DECLARE_ORM(AddressV2)

DECLARE_CLASS(AddressV2)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, id)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, street)
     ORM_DECLARE_ATTRIBUTE(AddressV2,QString, number)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, cep)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, Neighborhood)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, city)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, state)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, Owner)
     ORM_DECLARE_ATTRIBUTE(AddressV2,int, OwnerType)
     ORM_DECLARE_ATTRIBUTE(AddressV2,bool, Removed)
     ORM_MATCH_TABLE(AddressV2, "address")
ORM_END_MAP()

#endif // ADDRESSV2_H
