#ifndef AccountToReceiveModel_H
#define AccountToReceiveModel_H

#include "orm.h"
#include "accounttypemodel.h"
#include "debtmodel.h"

DECLARE_ORM(AccountToReceiveModel)

DECLARE_CLASS(AccountToReceiveModel)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,int, Id)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,QDate, IssueDate)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,QDate, VencDate)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,QDate, PaidDate)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,QString, Obs)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,double, Value)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,double, ValuePaid)
    ORM_DECLARE_FOREINGKEY(AccountToReceiveModel,int, AccountTypeId, AccountTypeModel, AccountTypeModel);
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,QString, Description)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,int, ClientId)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,bool, Paid)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,int, BankId)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,bool, Removed)
    ORM_DECLARE_ATTRIBUTE(AccountToReceiveModel,QString, DocNumber)
    ORM_DECLARE_FOREINGKEY(AccountToReceiveModel,int, DebtId, Debt, Debt)
   
    ORM_MATCH_TABLE(AccountToReceiveModel, "fin_accounttoreceivemodel")
ORM_END_MAP()

#endif 
