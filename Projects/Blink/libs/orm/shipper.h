#ifndef shipper_H
#define shipper_H
 
#include "orm.h"

DECLARE_ORM(shipper)

DECLARE_CLASS(shipper)
     ORM_DECLARE_ATTRIBUTE(shipper, int, id)
     ORM_DECLARE_ATTRIBUTE(shipper, QDate, Data)
     ORM_DECLARE_ATTRIBUTE(shipper, QTime, Hora)
     ORM_DECLARE_ATTRIBUTE(shipper, int, User)
     ORM_DECLARE_ATTRIBUTE(shipper, int, Amount)
     ORM_MATCH_TABLE(shipper, "shipper")
ORM_END_MAP()
#endif 
