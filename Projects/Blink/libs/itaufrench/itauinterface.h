#ifndef _ITAUINTERFACE_H
#define _ITAUINTERFACE_H

#include <QString>
#include <QList>
#include "itauticket.h"


class ItauInterface
{
public:
    ItauInterface();
    ~ItauInterface();

    static bool Load(QList<itauticket*> *tikets,QString Path);
};



#endif
