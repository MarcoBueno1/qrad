#include "paymentmodel.h"

ORM_BEGIN_MAP(Payment)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(DebtId, "debtid");
    ORM_MAP_FIELD(Value, "value");
    ORM_MAP_FIELD(FineValue, "fine");
    ORM_MAP_FIELD(InterestValue, "interest");
    ORM_MAP_FIELD(Date, "date");
    ORM_MAP_FIELD(Time, "time");
    ORM_MAP_FIELD(UserId, "userid");
    ORM_MAP_FIELD(Obs, "obs");
    ORM_MAP_FIELD(SaleDateBegin, "saledatebegin");
    ORM_MAP_FIELD(SaleTimeBegin, "saletimebegin");
    ORM_MAP_FIELD(SaleDateEnd, "saledateend");
    ORM_MAP_FIELD(SaleTimeEnd, "saletimeend");

ORM_END_MAP()

