#include "vehicle.h"

ORM_BEGIN_MAP(vehicle)
    ORM_MAP_PRIMARYKEY(id, "id");
    ORM_MAP_FIELD(owner, "owner");
    ORM_MAP_FIELD(board, "board");
    ORM_MAP_FIELD(brand, "brand");
    ORM_MAP_FIELD(veicname, "veicname");
    ORM_MAP_FIELD(Removed, "removed");
ORM_END_MAP()

