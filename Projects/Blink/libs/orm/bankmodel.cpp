#include "bankmodel.h"

ORM_BEGIN_MAP(BankModel)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(Description, "description");
    ORM_MAP_FIELD(Code, "code");
ORM_END_MAP()

