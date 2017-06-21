#include "extratx.h"

ORM_BEGIN_MAP(extratx)
     ORM_MAP_PRIMARYKEY(Id, "id")
     ORM_MAP_FIELD(Value, "value")
     ORM_MAP_FIELD(Times, "times")
     ORM_MAP_FIELD(Motivo, "motivo")
     ORM_MAP_FIELD(Data, "data")
     ORM_MAP_FIELD(All, "all")
     ORM_MAP_FIELD(Dweller, "dweller")
ORM_END_MAP()

