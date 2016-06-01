#ifndef DOCVALIDATE_H
#define DOCVALIDATE_H

#include <QString>

class CDoc
{
public:
    CDoc();

protected:
    static int mod11(char *szValue, char tipo);

};



class CCPF : public CDoc
{
public:
    CCPF();
    static bool IsValid( QString strCPF );
};


class CCNPJ : public CDoc
{
public:
    CCNPJ();
    static bool IsValid( QString strCNPJ );
};


#endif // DSMDOCS_H
