#ifndef configmodel_H
#define configmodel_H
 
#include "orm.h"

/*
typedef enum enu_status_tkt
{
  stCreated,
  stBuiltShipp,
  stPaid 
}BBOL_STATUS, *PBBOL_STATUS;

typedef enum enu_type_tkt
{
  tpTxCond,
  tpTxExtr,
  tpOutr,
}BBO_TYPE, *PBBOL_TYPE;
*/

DECLARE_ORM(config)

DECLARE_CLASS(config)
     ORM_DECLARE_ATTRIBUTE(config, int, id)
     ORM_DECLARE_ATTRIBUTE(config, double, M2Value)
     ORM_DECLARE_ATTRIBUTE(config, double, PunctualDiscount)
     ORM_DECLARE_ATTRIBUTE(config, QDate, DefaultPayDate)
     ORM_MATCH_TABLE(config, "config")
ORM_END_MAP()
#endif 
