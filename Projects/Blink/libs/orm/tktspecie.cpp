#include "tktspecie.h"


ORM_BEGIN_MAP(TktSpecie)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Description, "Description");
     ORM_MAP_FIELD(tp, "tp");
ORM_END_MAP()

