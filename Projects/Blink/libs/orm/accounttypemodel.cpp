#include "accounttypemodel.h"

ORM_BEGIN_MAP(AccountTypeModel)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(Description, "description");
    ORM_MAP_FIELD(Type, "type");

ORM_END_MAP()

