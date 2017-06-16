#include "acbr.h"


ORM_BEGIN_MAP(Acbr)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(SendPath, "sendpath");
     ORM_MAP_FIELD(ReceivePath, "receivepath");
     ORM_MAP_FIELD(SendName, "sendname");
     ORM_MAP_FIELD(ReceiveName, "receivename");
ORM_END_MAP()

