#ifndef common_area_H
#define common_area_H
 
#include "orm.h"

DECLARE_ORM(common_area)

DECLARE_CLASS(common_area)
     ORM_DECLARE_ATTRIBUTE(common_area, int, id);
     ORM_DECLARE_ATTRIBUTE(common_area, QString, name);
     ORM_DECLARE_ATTRIBUTE(common_area, QString, description);
     ORM_MATCH_TABLE(common_area, "common_area")
ORM_END_MAP()
#endif 
