#include "accounttoreceivemodel.h"

ORM_BEGIN_MAP(AccountToReceiveModel)
    ORM_MAP_PRIMARYKEY(Id, "id");
    ORM_MAP_FIELD(IssueDate, "issuedate");
    ORM_MAP_FIELD(VencDate, "vencdate");
    ORM_MAP_FIELD(PaidDate, "paiddate");
    ORM_MAP_FIELD(Obs, "obs");
    ORM_MAP_FIELD(Value, "value");
    ORM_MAP_FIELD(ValuePaid, "valuepaid");
    ORM_MAP_FIELD(AccountTypeId, "accounttypeid");
    ORM_MAP_FIELD(Description, "description");
    ORM_MAP_FIELD(ClientId, "clientid");
    ORM_MAP_FIELD(Paid, "paid");
    ORM_MAP_FIELD(BankId, "bankid");
    ORM_MAP_FIELD(Removed, "removed");
    ORM_MAP_FIELD(DocNumber, "docnumber");
    ORM_MAP_FIELD(DebtId, "debtid");
ORM_END_MAP()
