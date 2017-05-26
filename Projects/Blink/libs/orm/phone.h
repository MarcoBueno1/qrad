#ifndef Phone_H
#define Phone_H

#include "orm.h"

DECLARE_ORM(Phone)

DECLARE_CLASS(Phone)
     ORM_DECLARE_ATTRIBUTE(Phone, int, id)
     ORM_DECLARE_ATTRIBUTE(Phone, QString, Number)
     ORM_DECLARE_ATTRIBUTE(Phone, int, Operator)
     ORM_DECLARE_ATTRIBUTE(Phone, int, Type)
     ORM_DECLARE_ATTRIBUTE(Phone, bool, WatsApp)
     ORM_DECLARE_ATTRIBUTE(Phone, int, Owner)
     ORM_DECLARE_ATTRIBUTE(Phone, int, OwnerType)
     ORM_DECLARE_ATTRIBUTE(Phone, bool, Removed)

    ORM_MATCH_TABLE(Phone, "phone")
ORM_END_MAP()
 
#endif 
