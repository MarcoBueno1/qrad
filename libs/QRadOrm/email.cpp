#include "email.h"


ORM_BEGIN_MAP(email)
     ORM_MAP_PRIMARYKEY(Id, "id");
     ORM_MAP_FIELD(User, "vuser");
     ORM_MAP_FIELD(Password, "password");
     ORM_MAP_FIELD(Host, "host");
     ORM_MAP_FIELD(Port, "port");
ORM_END_MAP()

