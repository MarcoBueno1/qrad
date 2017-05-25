#ifndef Payment_H
#define Payment_H

#include "orm.h"
#include "userV2.h"
#include "debtmodel.h"

DECLARE_ORM(Payment)
DECLARE_CLASS(Payment)
    ORM_DECLARE_ATTRIBUTE(Payment,int, Id);
    ORM_DECLARE_FOREINGKEY(Payment,int, DebtId, Debt, Debt);
    ORM_DECLARE_ATTRIBUTE(Payment,double,  Value);
    ORM_DECLARE_ATTRIBUTE(Payment, double,  FineValue);
    ORM_DECLARE_ATTRIBUTE(Payment, double,  InterestValue);
    ORM_DECLARE_ATTRIBUTE(Payment,QDate, Date);
    ORM_DECLARE_ATTRIBUTE(Payment,QTime, Time);
    ORM_DECLARE_FOREINGKEY(Payment, int, UserId, User, User);
    ORM_DECLARE_ATTRIBUTE(Payment, QString, Obs);
    ORM_DECLARE_ATTRIBUTE(Payment, QDate, SaleDateBegin);
    ORM_DECLARE_ATTRIBUTE(Payment, QTime, SaleTimeBegin);
    ORM_DECLARE_ATTRIBUTE(Payment, QDate, SaleDateEnd);
    ORM_DECLARE_ATTRIBUTE(Payment, QTime, SaleTimeEnd);

    ORM_MATCH_TABLE(Payment, "payment")
ORM_END_MAP()
 
#endif 
