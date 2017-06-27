#ifndef _ITAUINTERFACE_H
#define _ITAUINTERFACE_H

#include <QString>
#include <QList>
#include "bankticketinterface.h"
//#include "itauticket.h"


class ItauInterface : public IBankTicketParser
{
public:
    ItauInterface(QString Path="");
    ~ItauInterface();

    bool Parse(QList<BankTicket*> *tikets,QString Path);
    bool Parse(QList<BankTicket*> *tikets);
private:
    QString m_filenme;
};



#endif
