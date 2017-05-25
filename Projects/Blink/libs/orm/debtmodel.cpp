#include "debtmodel.h"

ORM_BEGIN_MAP(Debt)
    ORM_MAP_PRIMARYKEY(Id, "id")
    ORM_MAP_FIELD(SaleId, "saleid")
    ORM_MAP_FIELD(PaymentWayId, "paymentwayid")
    ORM_MAP_FIELD(CreditId, "creditid")
    ORM_MAP_FIELD(PaymentDate, "paymentdate")
    ORM_MAP_FIELD(Value, "value")
    ORM_MAP_FIELD(Paid, "paid")
    ORM_MAP_FIELD(Portion, "portion");
    ORM_MAP_FIELD(NumPortions, "numportions");
    ORM_MAP_FIELD(Status, "status");
    ORM_MAP_FIELD(Description, "description");
    ORM_MAP_FIELD(PromotionId, "promotionid");
    ORM_MAP_FIELD(Payback, "payback");
    ORM_MAP_FIELD(BoletoEmitted, "boletoemitted");
    ORM_MAP_FIELD(HandTaken, "handtaken" );
ORM_END_MAP()

