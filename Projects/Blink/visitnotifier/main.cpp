#include <QCoreApplication>
#include "visit.h"
#include "Visitante.h"
#include "pgsqlasync.h"
//#include "reason.h"
#include "smtp.h"
#include "qradshared.h"
#include "dweller.h"
#include <QDebug>

QString g_user     = "garden.club.residence";
QString g_password = "1234marc";
QString g_host     = "gmail.com";
Smtp *g_mail = NULL ;

//garden.club.residence@gmail.com
//1234marc

/*
smtp.gmail.com

Requer SSL: Sim

Requer TLS: Sim (se disponível)

Requer autenticação: Sim

Porta para SSL: 465

Porta para TLS/STARTTLS: 587
*/
void configureDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL7");
///    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qraddb");
    db.setHostName("localhost");
    db.setPassword("dsmpassword");
    db.setUserName("dsm");

    if(!db.open())
    {
        qDebug() <<"Impossivel conectar no banco";
    }
    else
    {
        qDebug() << "Conexão Ok";
    }

}

QString GetAuthorizer(visit *pCurrent)
{
    QString Name;
    Dweller *d =  Dweller::findByid(pCurrent->getautorizador());
    if( d )
    {
        Name = d->getName();
        delete d;
    }
    return Name;
}

QString GetReason(visit *pCurrent)
{
   Q_UNUSED(pCurrent);
  return QString("Visita tecnica");
}

bool GetImage(visit *pCurrent)
{
    bool bResult = false;
    Visitante *v = Visitante::findByid(pCurrent->getVisitante());
    if( v )
    {
        if(v->getLoId())
        {
           QFile remove("Foto.jpg");
           remove.remove();

           bResult = v->getImage().save("Foto.jpg");

        }
       delete v;
    }
   return bResult;
}

QString GetVistName(visit *pCurrent)
{
    QString Name;
    Visitante *v = Visitante::findByid(pCurrent->getVisitante());
    if( v )
    {
        Name = v->getNome();
        delete v;
    }
    return Name;
}

QString GetTo(visit *pCurrent)
{
    QString Name;
    Dweller *d =  Dweller::findByid(pCurrent->getautorizador());
    if( d )
    {
        Name = d->getemail();
        delete d;
    }
    return Name;
}


bool ProcessEmail(visit *pCurrent)
{
  QString to  = GetTo(pCurrent);
  if( to.isEmpty())
  {
      /// nothing to do ...
     qDebug() << "Sem e-mail, nada a fazer..!";
      return true;
  }

  if( g_mail == NULL )
  {
      g_mail = new Smtp(g_user,g_password,g_host);
  }



  QString from     = "garden.club.residence@gmail.com";

  QString visitant= GetVistName(pCurrent);
  QString subject  = QString("%1 a caminho de seu apartamento...").arg(visitant);
  QString body  = QString("Prezado Morador,\n\t O Sr(a) %1 entrou no condominio em %2 %3 e dirige-se a seu apartamento, motivo da visita:\n\n\t%4\n\n\tAutorizado por: %5\n\n\tAtenciosamente\n")
          .arg(visitant)
          .arg(pCurrent->getData().toString(FMT_DATE))
          .arg(pCurrent->getHora().toString(FMT_TIME))
          .arg(GetReason(pCurrent))
          .arg(GetAuthorizer(pCurrent));

  QStringList list;
  if( GetImage(pCurrent))
         list.append("Foto.jpg");



  g_mail->sendMail(from,to,subject,body,list);

  qDebug() << "e-mail enviado.. ";

  return true;

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    configureDatabase();

    do
    {
        PGSQLAsync::WaitChange("visit",10000,"localhost","qraddb", "dsm", "dsmpassword");
        visitList *visits = visit::findNotNotified();
        for( int i =0; visits && ( i < visits->count()); i++)
        {
            qDebug() << "item a ser processado .. ";
            visit *pCurrent = visits->at(i);
            if( ProcessEmail(pCurrent) )
                pCurrent->updateNotified(true);
        }
        //qDebug() << "WaitForChange.... ";


    }while(1);

    return a.exec();
}
