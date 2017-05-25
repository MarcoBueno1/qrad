#ifndef AccountToReceiveHistoryModel_H
#define AccountToReceiveHistoryModel_H

#include "orm.h"
#include "accounttoreceivemodel.h"
#include "userV2.h"


DECLARE_ORM(AccountToReceiveHistoryModel)

DECLARE_CLASS(AccountToReceiveHistoryModel)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveHistoryModel, int, Id)
    ORM_DECLARE_FOREINGKEY(AccountToReceiveHistoryModel,int, AccountToReceiveId, AccountToReceiveModel, AccountToReceiveModel);
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveHistoryModel,int, TypeOperation)
    ORM_DECLARE_FOREINGKEY(AccountToReceiveHistoryModel, int, UserId, User, User)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveHistoryModel, QDate, Date)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveHistoryModel, QTime, Time)

    ORM_MATCH_TABLE(AccountToReceiveHistoryModel, "fin_accounttoreceive_history")
ORM_END_MAP()
 
#endif 
