#include "common_area.h"


ORM_BEGIN_MAP(common_area)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(name, "name");
     ORM_MAP_FIELD(description, "description");
     ORM_MAP_FIELD(interval, "interval");
     ORM_MAP_FIELD(tp, "tp");
     ORM_MAP_FIELD(removed, "removed");
ORM_END_MAP()

