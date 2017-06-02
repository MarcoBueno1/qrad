#ifndef ticket_H
#define ticket_H
 
#include "orm.h"

DECLARE_ORM(ticket)

DECLARE_CLASS(ticket)
     ORM_DECLARE_ATTRIBUTE(ticket, int, id);
     ORM_DECLARE_ATTRIBUTE(ticket, QString, NossoNumero);
     ORM_DECLARE_ATTRIBUTE(ticket, QString, SeuNumero);
     ORM_DECLARE_ATTRIBUTE(ticket, int, clientid);
     ORM_DECLARE_ATTRIBUTE(ticket, QDate, Vencto);
     ORM_DECLARE_ATTRIBUTE(ticket, double, Valor);
     ORM_DECLARE_ATTRIBUTE(ticket, QDate, PagoEm);
     ORM_DECLARE_ATTRIBUTE(ticket, double, ValorPago);
     ORM_DECLARE_ATTRIBUTE(ticket, int, idticket);
     ORM_MATCH_TABLE(ticket, "ticket")
ORM_END_MAP()
#endif 
