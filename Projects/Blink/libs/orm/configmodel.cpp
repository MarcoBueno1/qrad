#include "configmodel.h"


ORM_BEGIN_MAP(config)
     ORM_MAP_PRIMARYKEY(id, "id");
     ORM_MAP_FIELD(M2Value, "m2value");
     ORM_MAP_FIELD(PunctualDiscount, "punctualdiscount");
     ORM_MAP_FIELD(DefaultPayDate, "defaultpaydate");
ORM_END_MAP()

