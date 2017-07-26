#ifndef NEIGHBORHOOD_H
#define NEIGHBORHOOD_H

#include "orm.h"

DECLARE_ORM(Neighborhood)

DECLARE_CLASS(Neighborhood)
     ORM_DECLARE_ATTRIBUTE(Neighborhood, int, id)
     ORM_DECLARE_ATTRIBUTE(Neighborhood, QString, Name)
     ORM_DECLARE_ATTRIBUTE(Neighborhood, bool, Removed)
     ORM_MATCH_TABLE(Neighborhood, "neighborhood")
ORM_END_MAP()
#endif // NEIGHBORHOOD_H
