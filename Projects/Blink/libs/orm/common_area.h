#ifndef common_area_H
#define common_area_H
 
#include "orm.h"

typedef enum interv_type
{
    midhour,
    hour
}QRAD_INTERV_TYPE, *PQRAD_INTERV_TYPE;

DECLARE_ORM(common_area)

DECLARE_CLASS(common_area)
     ORM_DECLARE_ATTRIBUTE(common_area, int, id)
     ORM_DECLARE_ATTRIBUTE(common_area, QString, name)
     ORM_DECLARE_ATTRIBUTE(common_area, QString, description)
     ORM_DECLARE_ATTRIBUTE(common_area, int, interval)
     ORM_DECLARE_ATTRIBUTE(common_area, int, tp)
     ORM_DECLARE_ATTRIBUTE(common_area, bool, removed)
     ORM_MATCH_TABLE(common_area, "common_area")
ORM_END_MAP()
#endif 
