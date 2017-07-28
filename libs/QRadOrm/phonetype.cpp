#include "phonetype.h"

ORM_BEGIN_MAP(PhoneType)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Type, "type");
     ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()
