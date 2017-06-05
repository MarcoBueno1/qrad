#ifndef ticket_H
#define ticket_H
 
#include "orm.h"


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

DECLARE_ORM(ticket)

DECLARE_CLASS(ticket)
     ORM_DECLARE_ATTRIBUTE(ticket, int, id);
     ORM_DECLARE_ATTRIBUTE(ticket, int, NossoNumero);
     ORM_DECLARE_ATTRIBUTE(ticket, int, SeuNumero);
     ORM_DECLARE_ATTRIBUTE(ticket, int, clientid);
     ORM_DECLARE_ATTRIBUTE(ticket, QDate, Vencto);
     ORM_DECLARE_ATTRIBUTE(ticket, double, Valor);
     ORM_DECLARE_ATTRIBUTE(ticket, QDate, PagoEm);
     ORM_DECLARE_ATTRIBUTE(ticket, double, ValorPago);
     ORM_DECLARE_ATTRIBUTE(ticket, int, idticket);
     ORM_DECLARE_ATTRIBUTE(ticket, int, Status);
     ORM_DECLARE_ATTRIBUTE(ticket, int, Type);
     ORM_DECLARE_ATTRIBUTE(ticket, QString, Obs);
     ORM_DECLARE_ATTRIBUTE(ticket, bool, Removed);
     ORM_MATCH_TABLE(ticket, "ticket")
ORM_END_MAP()
#endif 
