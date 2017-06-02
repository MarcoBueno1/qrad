#ifndef ITAUPARSER_H
#define ITAUPARSER_H

#include "itauticket.h"
#include <QList>

class ItauParser
{
public:
    ItauParser();
    ~ItauParser();

    static bool Parse(QList<itauticket*> *tikets, QString Path);
private:
    static itauticket *ParseLine( QString Line );
};

#endif // ITAUPARSER_H
