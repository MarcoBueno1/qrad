#include "qrademail.h"
#include "emailconf.h"
#include "qradprogresswindow.h"

QRadEmail* QRadEmail::m_instance = NULL;

QRadEmail* QRadEmail::getInstance()
{
    if (NULL == m_instance)
        m_instance = new QRadEmail();

    return m_instance;
}


QRadEmail::QRadEmail()
{
    g_user     = "garden.club.residence@gmail.com";
    g_password = "1234marc";
    g_host     = "smtp.gmail.com";
    g_port     = "465";

    emailconf *email = emailconf::findById(1,true);
    if(email)
    {
        g_user     = email->getUserName();
        g_password = email->getPassword();
        g_host     = email->getServer();
        g_port     = email->getPort();
        delete email;
    }
}

QRadEmail::~QRadEmail()
{

}



bool QRadEmail::Send( QString Name,
                      QString to,
                      QString Subject,
                      QString Body,
                      QStringList files )
{
    QList<QString> listTo;
    QStringList listcoo;

    QRAD_SHOW_PRPGRESS(QString("Enviando e-mail para %1(%2) ...").arg(Name).arg(to));

    if(to.contains(";"))
        listTo.append(to.split(";"));
    else
        listTo.append(to);

    M2Smtp *newEmail  = new M2Smtp( g_host,
                                    g_user,
                                    g_password,
                                    g_port,
                                    g_user,
                                    listTo ,
                                    true,listcoo,Subject,Body,files, false );

    bool bRet = false;
    if(newEmail->send())
        bRet = true;

    m_emails.append(newEmail);

    QRAD_HIDE_PRPGRESS();
    return bRet;


/*
    Dweller *pDweller = Dweller::findByid(pTkt->getclientid());
    if( !pDweller)
        return false ;

    QString to = pDweller->getemail();
    if( to.isEmpty())
    {
        delete pDweller;
        return false;
    }
    emailconf *pEmail = emailconf::findById(1,true);

    QString from = pEmail->getUserName();
    QString body  = QString("Prezado Morador,<br/><br/>        Segue em anexo boleto para pagamento.<br/><br/><br/><br/>Atenciosamente\n");
    QString subject  = QString("Taxa Condominial");

    QStringList list;

    QString filename = "boleto.pdf";
    pTkt->getFile(filename, pTkt->getLoId());

    list.append(filename);

    qDebug() << "Linha:" << __LINE__;

    QList<QString> listTo;
    QStringList listcoo;
    listTo.append(to);



    M2Smtp *newEmail  = new M2Smtp(g_host,
                                   g_user,
                                   g_password,
                                   g_port,
                                   g_user,
                                   listTo ,
                                   true,listcoo,subject,body,list, false);

    bool bRet = false;
    if(newEmail->send())
        bRet = true;

    m_emails.append(newEmail);
    return bRet;
*/

}
