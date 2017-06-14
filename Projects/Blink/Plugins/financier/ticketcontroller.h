#ifndef TICKETCONTROLLER_H
#define TICKETCONTROLLER_H

#include "ticket.h"

#include "buildtkt.h"
#include "dweller.h"



class TicketController
{
public:
    TicketController();
    ~TicketController();

    bool BuildTicketCond(int id=0 );
    void OpenRemDir();
    void OpenPDF();
    void SendEmail();
private:
    BuildTkt *g_tkt;

    bool InitAcbr();
    bool doShipp(QString dir, QString filename,BBO_TYPE type, BBO_TYPE status);
    bool doPrepare(BBO_TYPE type, BBO_TYPE status);
    bool doPrint(BBO_TYPE type, BBO_TYPE status, int id=0);

    void Send(Dweller *pDweller);
};

#endif // TICKETCONTROLLER_H
