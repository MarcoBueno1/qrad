#ifndef PARSEPAYMENTS_H
#define PARSEPAYMENTS_H

#include <QSqlDatabase>

class ParsePayments
{
public:
    ParsePayments();
    ~ParsePayments();

    bool BuildFrom(QString Path);

private:
    QSqlDatabase ConfigTempDatabase();
};

#endif // PARSEPAYMENTS_H
