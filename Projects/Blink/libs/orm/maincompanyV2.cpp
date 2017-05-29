#include "maincompanyV2.h"


ORM_BEGIN_MAP(MainCompany)
    ORM_MAP_PRIMARYKEY(id, "id");
    ORM_MAP_FIELD(name, "name");
    ORM_MAP_FIELD(fantasyname, "fantasyname");
    ORM_MAP_FIELD(cnpj, "cnpj");
    ORM_MAP_FIELD(ie, "ie");
    ORM_MAP_FIELD(phone, "phone");
    ORM_MAP_FIELD(email, "email");
    ORM_MAP_FIELD(city, "city");
    ORM_MAP_FIELD(state, "state");
    ORM_MAP_FIELD(CEP, "cep")
    ORM_MAP_FIELD(Addrees, "address")
    ORM_MAP_FIELD(Neighborhood, "neighborhood")
    ORM_MAP_FIELD(HouseNumber, "housenumber")
    ORM_MAP_FIELD(Complement, "complement")
    ORM_MAP_FIELD(Max, "max");
    ORM_MAP_FIELD(License, "licence");
    ORM_MAP_FIELD(lastLicense, "lastlicense");
ORM_END_MAP()

