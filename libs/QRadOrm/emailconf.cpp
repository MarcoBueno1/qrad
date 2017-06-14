#include "emailconf.h"


ORM_BEGIN_MAP(emailconf)
     ORM_MAP_PRIMARYKEY(Id, "id");
     ORM_MAP_FIELD(UserName, "username");
     ORM_MAP_FIELD(Password, "password");
     ORM_MAP_FIELD(Server, "server");
     ORM_MAP_FIELD(Port, "port");
ORM_END_MAP()

