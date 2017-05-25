#ifndef DSMMONEY_H
#define DSMMONEY_H

#include <QString>

class DSMMoney
{
public:
    DSMMoney();
    static QString  MoneyHumanForm(QString);
    static QString  MoneyHumanForm(float money);
    static QString  MoneyHumanForm2(QString);
    static QString  MoneyHumanForm2(double money);
    static QString  MoneyHumanForm3(float value);
    static QString  MoneyHumanForm4(QString money);
    static QString  MoneyHumanForm5(float value);
    static QString  MoneyComputerForm(QString);
    static float    MoneyComputerForm2(QString);
    static float    Round(float money);
    static float    Round2(float money);
    static double   Round(double money, int decimals);
    static void     FormatValue(float value, int *intValue, int *fracValue);
    static int      StrToInt(QString money);
    static double   GetInterestValue(double capital, double taxPerDay, int period);
};

#endif // DSMMONEY_H
