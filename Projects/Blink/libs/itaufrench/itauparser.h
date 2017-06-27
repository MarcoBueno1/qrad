#ifndef ITAUPARSER_H
#define ITAUPARSER_H

#include "bankticket.h"
#include <QList>

class ItauParser
{
public:
    ItauParser();
    ~ItauParser();

    static bool Parse(QList<BankTicket*> *tikets, QString Path);
private:
    static BankTicket *ParseLine( QString Line );
};

#endif // ITAUPARSER_H
