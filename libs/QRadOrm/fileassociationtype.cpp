#include "fileassociationtype.h"

ORM_BEGIN_MAP(FileAssociationType)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(Description, "description");
     ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()
