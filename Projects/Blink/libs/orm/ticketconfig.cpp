#include "ticketconfig.h"


ORM_BEGIN_MAP(ticketconfig)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(RespEmis, "respemis");
     ORM_MAP_FIELD(TipoPessoa, "tipopessoa");
     ORM_MAP_FIELD(CodigoCedente, "codigocedente");
     ORM_MAP_FIELD(LayoutBol, "layoutbol");
     ORM_MAP_FIELD(TipoCobranca, "tipocobranca");
     ORM_MAP_FIELD(cnab, "cnab");
     ORM_MAP_FIELD(especie, "especie");
ORM_END_MAP()

