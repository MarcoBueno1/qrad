#include "filedescription.h"

ORM_BEGIN_MAP(FileDescription)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Description, "description");
     ORM_MAP_FIELD(Tp, "tp" );
     ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()

