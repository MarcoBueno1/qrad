#include "cep.h"

ORM_BEGIN_MAP(Cep)
     ORM_MAP_PRIMARYKEY(Id, "id");
     ORM_MAP_FIELD(Number, "number");
     ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()

