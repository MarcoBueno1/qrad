#include "qradmoney.h"
#include <QDebug>
#include "math.h"
//#include "qradconfig.h"
#include "qradround.h"

QRadMoney::QRadMoney()
{
}

QString QRadMoney::MoneyHumanForm(QString money)
{
    int position;
    int i;
    bool    isNegative = false;

    if (money.contains("-"))
    {
        money.replace("-","");
        isNegative = true;
    }

    if (!money.contains("."))
        money.append(".00");

    money.replace(".",",");
    position = money.indexOf(",") + 1;

    if( 2 != (money.length() - position) )
        money.append("0");

    position = money.indexOf(",") + 1;

    i = position;
    while(i > 4)
    {
        i -= 4;
        money.insert(i,".");
        i++;
    }

    if (isNegative)
    {
        money.insert(0, "R$ -");
    }
    else
    {
        money.insert(0,"R$ ");
    }

    return money;
}

QString QRadMoney::MoneyHumanForm(float money)
{
    QString strMoney;

    strMoney.sprintf("%0.02f",money);

    if (strMoney == "-0.00")
    {
        strMoney = "0.00";
    }
    return MoneyHumanForm(strMoney);

}

QString QRadMoney::MoneyHumanForm2(QString money)
{
    int position;
    int i;
    bool    isNegative = false;

    if (money.contains("-"))
    {
        money.replace("-","");
        isNegative = true;
    }

    if (!money.contains("."))
        money.append(".00");

    money.replace(".",",");
    position = money.indexOf(",") + 1;

    if( 2 != (money.length() - position) )
        money.append("0");

    position = money.indexOf(",") + 1;

    i = position;
    while(i > 4)
    {
        i -= 4;
        money.insert(i,".");
        i++;
    }

    if (isNegative)
    {
        money.insert(0, "-");
    }
    return money;
}

QString QRadMoney::MoneyHumanForm3(float value)
{
    int position;
    int i;
    QString money;

    money.sprintf("%0.02f",value);

    if (money == "-0.00")
    {
        money = "0.00";
    }

    if (!money.contains("."))
        money.append(".00");

    money.replace(".",",");
    position = money.indexOf(",") + 1;

    if( 2 != (money.length() - position) )
        money.append("0");

    position = money.indexOf(",") + 1;

    i = position;
    while(i > 4)
    {
        i -= 4;
        money.insert(i,".");
        i++;
    }

    return money;
}

QString QRadMoney::MoneyHumanForm4(QString money)
{
    int position;
    int i;
    bool    isNegative = false;

    if (money.contains("-"))
    {
        money.replace("-","");
        isNegative = true;
    }

    if (money.size() == 1)
    {
        money = "0,0" + money;
    }
    else if (money.size() == 2)
    {
        money = "0," + money;
    }
    else
    {
        money = money.left(money.size()-2) + "," + money.mid(money.size()-2);
    }

    position = money.indexOf(",") + 1;

    i = position;
    while(i > 4)
    {
        i -= 4;
        money.insert(i,".");
        i++;
    }

    if (isNegative)
    {
        money.insert(0, "-");
    }
    return money;
}

QString QRadMoney::MoneyHumanForm5(float value)
{
    int position;
    int i;
    QString money;

    money.sprintf("%0.04f",value);

    if (money == "-0.00")
    {
        money = "0.00";
    }

    if (!money.contains("."))
        money.append(".00");

    money.replace(".",",");
    position = money.indexOf(",") + 1;

    if( 4 != (money.length() - position) )
        money.append("0");

    position = money.indexOf(",") + 1;

    i = position;
    while(i > 4)
    {
        i -= 4;
        money.insert(i,".");
        i++;
    }

    return money;
}


QString QRadMoney::MoneyHumanForm2(double money)
{
    return MoneyHumanForm2(QString().sprintf("%0.02f", money));
}

QString QRadMoney::MoneyComputerForm(QString money)
{
    money.remove("R$ ");
    money.replace(".","");
    money.replace(",",".");
    return money;
}

float QRadMoney::MoneyComputerForm2(QString money)
{
    return MoneyComputerForm(money).toFloat();
}

float QRadMoney::Round(float money)
{
    return QRadRound::PowerRound(money);
}

float QRadMoney::Round2(float money)
{
    money *= 10000;
    money = qRound(money);

    return money /= 10000;
}

double QRadMoney::Round(double money, int decimals)
{
    if (decimals <= 0)
        return (double)((int)money);

    int dec = 10;

    for (int i = 0; i < (decimals-1); i++)
        dec *= 10;

    int iValue = money * dec;
    int lastDecimal = iValue;
    int tmp = iValue;

    int count = 0;

    while (tmp > 1)
    {
        if (!count)
            count = 1;
        else
            count *= 10;

        tmp = tmp/10;
    }

    for (int i = count; i >= 10; i-= 10)
    {
        lastDecimal %= i;
    }

    if (lastDecimal >= 5)
        iValue += 10 - lastDecimal;
    else
        iValue -= lastDecimal;

    return iValue/((double)dec);
}

void QRadMoney::FormatValue(float value, int *intValue, int *fracValue)
{
    *intValue = (int)value;
    *fracValue = (int)(QRadMoney::Round((QRadMoney::Round(value) - *intValue)*100));

    if ((*fracValue) > 99)
    {
        *fracValue = 0;
        *intValue += 1;
    }
}

int QRadMoney::StrToInt(QString money)
{
    money.remove("R$ ");
    money.replace(".","");
    money.replace(",","");

    return money.toInt();
}

double QRadMoney::GetInterestValue(double capital, double taxPerDay, int period)
{
    if( taxPerDay < 0.00 || period == 0)
        return 0.0;

//    if (QRadConfig::IsSimpleInterest())
//    {
//        return (capital * taxPerDay * period);
//    }
    return capital * pow((1 + taxPerDay), period);
}
