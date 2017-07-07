#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "qradround.h"
#include <QCoreApplication>
#include <QStringList>

#define debug_message(fmt, args...) \
    if (QCoreApplication::instance()->arguments().contains("--debug")) \
    { fprintf(stdout, fmt, ##args); fflush(stdout); }


QRadRound::QRadRound()
{

}

double QRadRound::PowerRound(double Value)
{
    double dRet;
    char pBuff[200];

    sprintf( pBuff, "%.2lf", Value);

    dRet = (double) atof( pBuff );
    if( (dRet - 0.004) > Value)
        dRet -= 0.01;
    return dRet;
}

double QRadRound::PowerRound2(double Value)
{
    double dRet;
    char pBuff[200];

    sprintf( pBuff, "%.3lf", Value);

    dRet = (double) atof( pBuff );
    if( (dRet - 0.004) > Value)
        dRet -= 0.01;
    return dRet;
}

double QRadRound::PowerRoundAmount(double Value)
{
    char pBuff[200];

    sprintf( pBuff, "%.3lf", Value);

    return (double) atof( pBuff );
}

double QRadRound::PowerRoundAmountTruncate(double Value)
{
    char pBuff[200];

    sprintf( pBuff, "%.5lf", Value);
    pBuff[strlen(pBuff)-2] = 0;

    return (double) atof( pBuff );
}

bool QRadRound::IsEqual(double Value1, double Value2)
{
    char szValue1[100];
    char szValue2[100];

    sprintf( szValue1, "%.2lf", Value1);
    sprintf( szValue2, "%.2lf", Value2);

    if( 0 == strcmp( szValue1, szValue2))
        return true;

    debug_message( "\n%s != %s\n",szValue1, szValue2);
    return false;
}
