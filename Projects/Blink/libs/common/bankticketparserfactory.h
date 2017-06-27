#ifndef _BANKTICKETPARSERFACTORY_H
#define _BANKTICKETPARSERFACTORY_H

#include "bankticketinterface.h"
#include <QString>

class BankTicketParserFactory
{
public:
    BankTicketParserFactory();
    ~BankTicketParserFactory();

    static IBankTicketParser *GetParser(QString filename);

};


#endif
