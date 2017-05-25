#ifndef AccountToPayHistoryModel_H
#define AccountToPayHistoryModel_H

#include "orm.h"
#include "accounttopaymodel.h"
#include "userV2.h"


DECLARE_ORM(AccountToPayHistoryModel)

DECLARE_CLASS(AccountToPayHistoryModel)
    ORM_DECLARE_ATTRIBUTE(AccountToPayHistoryModel, int, Id)
    ORM_DECLARE_FOREINGKEY(AccountToPayHistoryModel,int, AccountToPayId, AccountToPayModel, AccountToPayModel);
    ORM_DECLARE_ATTRIBUTE(AccountToPayHistoryModel,int, TypeOperation)
    ORM_DECLARE_FOREINGKEY(AccountToPayHistoryModel, int, UserId, User, User)
    ORM_DECLARE_ATTRIBUTE(AccountToPayHistoryModel, QDate, Date)
    ORM_DECLARE_ATTRIBUTE(AccountToPayHistoryModel, QTime, Time)

    ORM_MATCH_TABLE(AccountToPayHistoryModel, "fin_accounttopay_history")
ORM_END_MAP()
 
#endif 
