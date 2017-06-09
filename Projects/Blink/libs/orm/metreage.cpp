#include "metreage.h"


ORM_BEGIN_MAP(metreage)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Descricao, "description");
     ORM_MAP_FIELD(m2, "m2");
     ORM_MAP_FIELD(MontlyValue, "montlyvalue");
ORM_END_MAP()

