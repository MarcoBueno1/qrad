#include "ormreport.h"

ORM_BEGIN_MAP(report)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(Name, "name");
    ORM_MAP_FIELD(Path, "path");
    ORM_MAP_FIELD(Menu, "menu");
    ORM_MAP_FIELD(DateMode, "datemode");
    ORM_MAP_FIELD(Sql, "sql");
    ORM_MAP_FIELD(SqlCombo, "sqlcombo");
    ORM_MAP_FIELD(Permission, "permission");
    ORM_MAP_FIELD(Visible, "visible");
ORM_END_MAP()
