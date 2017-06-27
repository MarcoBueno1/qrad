#ifndef _BANKTICKETINTERFACE_H
#define _BANKTICKETINTERFACE_H

#include "bankticket.h"

class IBankTicketParser
{
public:
    virtual ~IBankTicketParser() {}

    virtual bool Parse(QList<BankTicket*> *tickets,QString Path) = 0;
    virtual bool Parse(QList<BankTicket*> *tickets) = 0;
};

#endif
