#include "supplier.h"


ORM_BEGIN_MAP(supplier)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(CNPJ, "cnpj");
     ORM_MAP_FIELD(Nome, "nome");
     ORM_MAP_FIELD(Fantasia, "fantasia");
     ORM_MAP_FIELD(Data, "data_cadastro");
     ORM_MAP_FIELD(Hora, "hora_cadastro");
ORM_END_MAP()

