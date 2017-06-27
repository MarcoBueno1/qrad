#include "itauparser.h"

#include <QFile>
#include <QStringList>
//#include "itauticket.h"
#include "bankticket.h"

ItauParser::ItauParser()
{

}

ItauParser::~ItauParser()
{

}


BankTicket * ItauParser::ParseLine( QString Line )
{
//109          000012281          007060-02          HOSPITAL SANTA JULIA LTDA                         18/05/2017        4320        COBRANCA          L     18/05/2017     1.550,00   01              -3,36       1.546,64
    int index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString Cart = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString NossoNumero = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();


    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString SeuNumero = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString Pagador;
    do
    {
      Pagador += Line.mid(0,index);
      Line.remove(0,index);
      Line = Line.trimmed();
      if(Line.at(0).isNumber())
          break;
      Pagador += " ";
    }while(1);

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString Data = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString Agencia = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString Cobranca = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString Pago = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString DataPago = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString Valor = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString NumeroDescartar = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

    index = Line.indexOf(" ");
    if( -1 == index)
        return NULL;

    QString JurosMulta = Line.mid(0,index);
    Line.remove(0,index);
    Line = Line.trimmed();

//    index = Line.indexOf(" ");
//    if( -1 == index)
//        return NULL;

    QString ValorPago = Line;//mid(0,index);
//    Line.remove(0,index);
//    Line = Line.trimmed();
    if( Cart.isEmpty() || NossoNumero.isEmpty() || SeuNumero.isEmpty() ||
            Pagador.isEmpty() || Data.isEmpty() || Agencia.isEmpty() ||
            Cobranca.isEmpty() || Pago.isEmpty() || DataPago.isEmpty() ||
            Valor.isEmpty() || NumeroDescartar.isEmpty() || JurosMulta.isEmpty()
            || ValorPago.isEmpty())
        return NULL;

    QDate  dtVencto = QDate::fromString(Data,"dd/MM/yyyy");
    QDate  dtPagto = QDate::fromString(DataPago,"dd/MM/yyyy");

    return new BankTicket(Cart,NossoNumero,SeuNumero,Pagador,dtVencto,Pago.startsWith("L")?true:false,dtPagto,Agencia,Valor,ValorPago,Pago.startsWith("L")?tpLiquidated:tpOther);

}


bool ItauParser::Parse(QList<BankTicket*> *tikets, QString Path)
{
    QFile *file = new QFile(Path);

    if( !file->open(QIODevice::ReadOnly))
    {
        delete file ;
        return false;
    }

    QString contents =  file->readAll();

    QStringList lines = contents.split("\n");
    for( int i = 0; i < lines.count();i ++)
    {
        QString line = lines.at(i);
        if( line.trimmed().at(0).isNumber() ) // ok we found a valid line
        {
            BankTicket *pTicket =  ParseLine(line.trimmed());

            if(pTicket)
            {
                tikets->append(pTicket);
            }
        }
    }

    return !lines.isEmpty();
}
