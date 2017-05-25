#include "accounttopayhistorymodel.h"

ORM_BEGIN_MAP(AccountToPayHistoryModel)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(AccountToPayId, "accounttopayid");
    ORM_MAP_FIELD(TypeOperation, "typeoperation");
    ORM_MAP_FIELD(UserId, "userid");
    ORM_MAP_FIELD(Date, "date");
    ORM_MAP_FIELD(Time, "time");

ORM_END_MAP()

