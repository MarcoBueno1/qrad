#include "userV2.h"

ORM_BEGIN_MAP(User)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(name, "name");
     ORM_MAP_FIELD(email, "email");
     ORM_MAP_FIELD(contractsince, "contractsince");
     ORM_MAP_FIELD(password, "password");
     ORM_MAP_FIELD(profile, "profile");
     ORM_MAP_FIELD(LoId, "loid");
     ORM_MAP_FIELD(removed, "removed");

ORM_END_MAP()

