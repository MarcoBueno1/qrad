#ifndef City_H
#define City_H
 
#include "orm.h"

DECLARE_ORM(City)

DECLARE_CLASS(City)
     ORM_DECLARE_ATTRIBUTE(City, int, id)
     ORM_DECLARE_ATTRIBUTE(City, QString, Name)
     ORM_MATCH_TABLE(City, "city")
ORM_END_MAP()
#endif 
