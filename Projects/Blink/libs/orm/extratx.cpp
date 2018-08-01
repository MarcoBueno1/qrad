#include "extratx.h"

ORM_BEGIN_MAP(extratx)
     ORM_MAP_PRIMARYKEY(Id, "id")
     ORM_MAP_FIELD(Value, "valor")
     ORM_MAP_FIELD(Times, "times")
     ORM_MAP_FIELD(Motivo, "motivo")
     ORM_MAP_FIELD(Data, "data")
     ORM_MAP_FIELD(All, "todos")
     ORM_MAP_FIELD(Dweller, "dweller")
     ORM_MAP_FIELD(Obs, "obs")
     ORM_MAP_FIELD(PercentAdv, "percentadv")
     ORM_MAP_FIELD(ValueAdv, "valueadv")
ORM_END_MAP()

