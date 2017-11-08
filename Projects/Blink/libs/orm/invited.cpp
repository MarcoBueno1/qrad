#include "invited.h"


ORM_BEGIN_MAP(Invited)
     ORM_MAP_PRIMARYKEY(Id, "id");
     ORM_MAP_FIELD(Reserve, "reserve");
     ORM_MAP_FIELD(Name, "name");
     ORM_MAP_FIELD(Documment, "documment");
     ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()

