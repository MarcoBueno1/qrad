#include "city.h"

ORM_BEGIN_MAP(City)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Name, "name");
ORM_END_MAP()

