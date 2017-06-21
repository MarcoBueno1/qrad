#ifndef TICKETCONTROLLER_H
#define TICKETCONTROLLER_H

#include "ticket.h"

#include "buildtkt.h"
#include "dweller.h"
#include "extratx.h"



class TicketController
{
public:
    TicketController();
    ~TicketController();

    bool BuildTicketCond(int id=0 );
    bool BuildTicketExtra( extratx *pTx );
    void OpenRemDir();
    void OpenPDF();
    void SendEmail();
    bool doShipp(QString dir="", QString filename="",BBO_TYPE type=tpTxCond, BBOL_STATUS status=stCreated);
    bool doPrint(BBO_TYPE type, BBOL_STATUS status, ticket *ptkt=0);
private:
    BuildTkt *g_tkt;

    bool InitAcbr();
    bool doPrepare(BBO_TYPE type, BBOL_STATUS status);

    void Send(Dweller *pDweller);
    bool BuildTicket( DwellerList *dlist,
                      QDate date,
                      int type,
                      QString Obs,
                      int ExtraTxId=0 );
};

#endif // TICKETCONTROLLER_H
