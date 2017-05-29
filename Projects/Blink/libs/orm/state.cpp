#include "state.h"

ORM_BEGIN_MAP(State)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Name, "name");
     ORM_MAP_FIELD(Sign, "sign");
ORM_END_MAP()

