#include "accounttoreceivehistorymodel.h"

ORM_BEGIN_MAP(AccountToReceiveHistoryModel)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(AccountToReceiveId, "accounttoreceiveid");
    ORM_MAP_FIELD(TypeOperation, "typeoperation");
    ORM_MAP_FIELD(UserId, "userid");
    ORM_MAP_FIELD(Date, "date");
    ORM_MAP_FIELD(Time, "time");

ORM_END_MAP()

