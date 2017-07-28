#ifndef ticketmodel_H
#define ticketmodel_H
 
#include "orm.h"


typedef enum enu_status_tkt
{
  stCreated,
  stBuiltShipp,
  stRegistered,
  stPaid,
  stModified,
  stDateModified,
  stValueModified,
  stDateValueModified

}BBOL_STATUS, *PBBOL_STATUS;


typedef enum enu_sendstatus_tkt
{
  stDisabled = -1,
  stPending,
  stSent,
  stErrSent // future use
}BBOL_SEND_STATUS, *PBBOL_SEND_STATUS;


typedef enum enu_type_tkt
{
  tpTxCond, //0
  tpTxExtr, //1
  tpOutr,   //2
  tpAll,    //3
}BBO_TYPE, *PBBOL_TYPE;

DECLARE_ORM(ticket)

DECLARE_CLASS(ticket)
     ORM_DECLARE_ATTRIBUTE(ticket, int, id)
     ORM_DECLARE_ATTRIBUTE(ticket, int, NossoNumero)
     ORM_DECLARE_ATTRIBUTE(ticket, int, SeuNumero)
     ORM_DECLARE_ATTRIBUTE(ticket, int, clientid)
     ORM_DECLARE_ATTRIBUTE(ticket, QDate, Vencto)
     ORM_DECLARE_ATTRIBUTE(ticket, double, Valor)
     ORM_DECLARE_ATTRIBUTE(ticket, QDate, PagoEm)
     ORM_DECLARE_ATTRIBUTE(ticket, double, ValorPago)
     ORM_DECLARE_ATTRIBUTE(ticket, int, idticket)
     ORM_DECLARE_ATTRIBUTE(ticket, int, Status)
     ORM_DECLARE_ATTRIBUTE(ticket, int, Type)
     ORM_DECLARE_ATTRIBUTE(ticket, QString, Obs)
     ORM_DECLARE_ATTRIBUTE(ticket, int, User)
     ORM_DECLARE_ATTRIBUTE(ticket, int, AccountId)
     ORM_DECLARE_ATTRIBUTE(ticket, int, SendStatus)
     ORM_DECLARE_ATTRIBUTE(ticket, int, LoId)
     ORM_DECLARE_ATTRIBUTE(ticket, bool, Removed)
     ORM_DECLARE_ATTRIBUTE(ticket, int, ExtraTxId)
     ORM_DECLARE_ATTRIBUTE(ticket, double, Discount)
     ORM_DECLARE_ATTRIBUTE(ticket, QDate, IssueDate)
     ORM_MATCH_TABLE(ticket, "ticket")
public:
    int saveFile(QString path);
    void getFile(QString path, int Loid);
    bool UpdateToPaid(QDate date, double value);

ORM_END_MAP()
#endif 
