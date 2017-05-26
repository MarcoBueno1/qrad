#include "phone.h"

ORM_BEGIN_MAP(Phone)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Number, "number");
     ORM_MAP_FIELD(Operator, "operator");
     ORM_MAP_FIELD(Type, "type");
     ORM_MAP_FIELD(WatsApp, "watsapp" );
     ORM_MAP_FIELD(Owner, "owner");
     ORM_MAP_FIELD(OwnerType, "ownertype");
     ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()

