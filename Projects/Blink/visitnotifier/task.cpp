#include <QPixmap>
#include "visit.h"
#include "Visitante.h"
#include "pgsqlasync.h"
//#include "reason.h"
#include "smtp.h"
#include "qradshared.h"
#include "dweller.h"
#include <QDebug>
#include "m2smtp.h"
#include<QApplication>
#include "task.h"
#include "reason.h"

QString g_user     = "garden.club.residence@gmail.com";
//QString g_user     = "lnxdevelopment@gmail.com";
QString g_password = "1234marc";
QString g_host     = "smtp.gmail.com";
QString g_port     = "465";


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
   QString string = QString("Visita tecnica");
   reason *reas = reason::findByid(pCurrent->getReason());
   if(reas)
   {
      string = reas->getdescription();
      delete reas;
   }

  return string;
}

QString GetImage(visit *pCurrent)
{
    QString path;
    Visitante *v = Visitante::findByid(pCurrent->getVisitante());
    if( v )
    {
        if(v->getLoId())
        {
//           QFile remove("Foto.jpg");
  //         remove.remove();

           path = v->getImage("");

        }
       delete v;
    }
   return path;
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

//  if( g_mail == NULL )
//  {
//      g_mail = new Smtp(g_user,g_password,g_host);
//  }



  QString from     = "garden.club.residence@gmail.com";

  QString visitant= GetVistName(pCurrent);
  QString subject  = QString("%1 a caminho de seu apartamento...").arg(visitant);
  QString body  = QString("Prezado Morador,<br/><br/>        O Sr(a) %1 entrou no condominio em %2 as %3hrs e dirige-se a seu apartamento, motivo da visita:<br/><br/><br/><br/>          %4<br/><br/><br/><br/>        Autorizado por: %5<br/><br/><br/><br/>        Atenciosamente\n")
          .arg(visitant)
          .arg(pCurrent->getData().toString(FMT_DATE))
          .arg(pCurrent->getHora().toString(FMT_TIME))
          .arg(GetReason(pCurrent))
          .arg(GetAuthorizer(pCurrent));

  qDebug() << "Linha:" << __LINE__;
  QStringList list;

  QString photo = GetImage(pCurrent);
  if( !photo.isEmpty())
         list.append(photo);

  qDebug() << "Linha:" << __LINE__;

  QList<QString> listTo;
  QStringList listcoo;
  listTo.append(to);

  qDebug() << "to:" << to;
  qDebug() << "Linha:" << __LINE__;

  M2Smtp *newEmail  = new M2Smtp(g_host,
                                 g_user,
                                 g_password,
                                 g_port,
                                 g_user,
                                 listTo ,
                                 true,listcoo,subject,body,list, false);
//  connect (newEmail, SIGNAL(status(int)), this, SLOT(updateProgress(int)));
//  connect (newEmail, SIGNAL(completed(bool,int)), this, SLOT(onSendCompleted(bool,int)));

//  progress->setValue(0);

  //progress->show();
  qDebug() << "Linha:" << __LINE__;
  bool bRet = false;
  if(newEmail->send())
      bRet = true;
  qDebug() << "Linha:" << __LINE__;


  //g_mail->sendMail(from,to,subject,body,list);

  qDebug() << "e-mail enviado.. ";

  return bRet;

}



Task::Task(QObject *parent) : QObject(parent)
{

}


Task::~Task()
{

}


void Task::run()
{
    // Do processing here

    do
    {
        PGSQLAsync::WaitChange("visit",10000,"localhost","qraddb", "dsm", "dsmpassword");
        visitList *visits = visit::findNotNotified();
        for( int i =0; visits && ( i < visits->count()); i++)
        {
            qDebug() << "item a ser processado .. ";
            printf("item a ser processado .. \n");
            visit *pCurrent = visits->at(i);
            if( ProcessEmail(pCurrent) )
                pCurrent->updateNotified(true);
        }
        qDebug() << "WaitForChange.... ";


    }while(1);

    emit finished();
}
