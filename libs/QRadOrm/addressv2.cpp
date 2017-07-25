#include "addressv2.h"

ORM_BEGIN_MAP(AddressV2)
    ORM_MAP_PRIMARYKEY(id, "id");
    ORM_MAP_FIELD(street, "street");
    ORM_MAP_FIELD(number, "number");
    ORM_MAP_FIELD(cep, "cep");
    ORM_MAP_FIELD(Neighborhood, "Neighborhood");
    ORM_MAP_FIELD(city, "city");
    ORM_MAP_FIELD(state, "state");
    ORM_MAP_FIELD(Owner, "owner");
    ORM_MAP_FIELD(OwnerType, "ownertype");
    ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()
