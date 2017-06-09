#include "shipper.h"


ORM_BEGIN_MAP(shipper)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Data, "data");
     ORM_MAP_FIELD(Hora, "hora");
     ORM_MAP_FIELD(User, "vuser");
ORM_END_MAP()

