#include "paymentwaymodel.h"

ORM_BEGIN_MAP(PaymentWay)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(Description, "description");
    ORM_MAP_FIELD(CreditSystemId, "creditsystemid");
    ORM_MAP_FIELD(PaybackEnabled, "paybackenabled");
    ORM_MAP_FIELD(Visible, "visible");
ORM_END_MAP()

