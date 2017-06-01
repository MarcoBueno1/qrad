#include "buildtkt.h"
#include "tktspecie.h"
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QEventLoop>
#include <QCoreApplication>

#define TKT_PREFIX "BOLETO."

#define TKT_PURGE "LimparLista"
#define TKT_CONFIG "ConfigurarDados"
 #define TKT_CONFIG_CEDENTE "[Cedente]\n"\
"Nome=%1\n"\
"CNPJCPF=%2\n"\
"Logradouro=%3\n"\
"Numero=%4\n"\
"Bairro=%5\n"\
"Cidade=%6\n"\
"CEP=%7\n"\
"Complemento=%8\n"\
"UF=%9\n"\
"RespEmis=%10\n"\
"TipoPessoa=%11\n"\
"CodigoCedente=%12\n"\
"LayoutBol=%13\n"\
"CaracTitulo=%14\n"

 #define TKT_CONFIG_ACCOUNT "[Conta]\n"\
"Conta=%1\n"\
"DigitoConta=%2\n"\
"Agencia=%3\n"\
"DigitoAgencia=%4\n"


 #define TKT_CONFIG_BANK "[Banco]\n"\
"Numero=%1\n"\
"CNAB=%2\n"


#define TKT_ADD_TICKET "IncluirTitulos\n"
 #define TKT_CONFIG_TICKET "[Titulo%1]\n"\
"NumeroDocumento=%2\n"\
"NossoNumero=%3\n"\
"Carteira=%4\n"\
"ValorDocumento=%5\n"\
"Sacado.NomeSacado=%6\n"\
"Sacado.CNPJCPF=%7\n"\
"Sacado.Logradouro=%8\n"\
"Sacado.Numero=%9\n"\
"Sacado.Bairro=%10\n"\
"Sacado.Complemento=%11\n"\
"Sacado.Cidade=%12\n"\
"Sacado.UF=%13\n"\
"Sacado.CEP=%14\n"\
"Mensagem=%16\n"\
"ValorMoraJuros=%17\n"\
"Vencimento=%18\n"\
"Especie=%19\n"


#define TKT_BUILD_SHIPPING "GerarRemessa"


#define TKT_PRINT "Imprimir"
#define TKT_PRINT_PDF "GerarPDF"

#define SEND_PATH "/media/sf_Dvl/acbr/ent/"
#define SEND_NAME "ent.txt"

#define RECEIVE_PATH "/media/sf_Dvl/acbr/sai/"
#define RECEIVE_NAME "ent-resp.txt"


#ifdef Q_OS_WIN
#include <windows.h> // for Sleep
#endif
void BuildTkt::qSleep(int ms)
{
#if 0
    QTime dieTime= QTime::currentTime().addSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
#endif


#ifdef Q_OS_WIN
    Sleep(uint(ms));
#else
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);
#endif

}


void BuildTkt::DirModified(QString dir )
{
   m_bDirModified = true;
}

BuildTkt::BuildTkt()
{
    m_watcher.addPath(RECEIVE_PATH);
    QObject::connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(DirModified(QString)));
}

BuildTkt::~BuildTkt()
{
    for( int i = 0; i < m_tickets.count(); i++)
    {
        qDebug() << "deletando: " <<m_tickets.at(i);
        delete m_tickets.at(i);
    }
    m_tickets.clear();
}


bool BuildTkt::Send(QString cmd)
{
    int dwTimeout = 0;
    QString filemane  = QString("%1%2").arg(SEND_PATH).arg(SEND_NAME);

    m_bDirModified = false;

    QFile *file = new QFile(filemane);
    if( !file->open(QIODevice::WriteOnly))
    {
        delete file;
        return false;
    }
    bool result = file->write(cmd.toLatin1())?true:false;
    file->flush();
    file->close();
    qDebug() << "Escrito:" << cmd.toLatin1();
    delete file;

    if( result ) // verifica retorno
    {
        while( !m_bDirModified && (dwTimeout < 20))
        {
             qSleep(100);
             dwTimeout++;
        }
        /*
        if( dwTimeout == 50 )
        {
            qDebug() << "timeout, retornando erro.:";
            m_lastError = "Sem resposta para o comando de envio!";
            return false;
        }
        */

//        qDebug() << "Depois da espera:";
        qSleep(100);
        //qSleep(1);
        QFile *receivefile = new QFile(QString("%1%2").arg(RECEIVE_PATH).arg(RECEIVE_NAME));
//        qDebug() << "Depois new:";
        if( !receivefile->open(QIODevice::ReadOnly))
        {
//            qDebug() << "Vai deletar receivefile:";
            delete receivefile;
            m_lastError = "Erro na abertura do arquivo de resposta!";
            return false;
        }
        result = true;
//         qDebug() << "Vai ler dados:";
        QString answer = receivefile->readAll();
//         qDebug() << "leu:";
        if( !answer.startsWith("OK:"))
        {
            result = false;
//             qDebug() << "setando lasterror:" << answer;
            m_lastError = answer;
        }
//         qDebug() << "fehando o arquivo";
        receivefile->close();
//         qDebug() << "Vai deletar receivefile 2:";
        delete receivefile;
//         qDebug() << "Vai deletar arquivo receivefile:";
        QFile::remove(QString("%1%2").arg(RECEIVE_PATH).arg(RECEIVE_NAME));
        qDebug() << "recebido:" << answer;

    }
    return result;
}

bool BuildTkt::Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount)
{
   QString cmdPurge =  QString("%1%2").arg(TKT_PREFIX).arg(TKT_PURGE);


   if(!pCompany || !pTktConfig || !pBank || !pAccount )
   {
       return false;
   }

   m_TktCount = 0;
   m_ShippNumber =0;

   if( !Send(cmdPurge))
   {
        qDebug() << "Saindo da init com false :";
      
       return false;
   }

   m_pCompany = pCompany;
   m_pTktConfig = pTktConfig;
   m_pBank = pBank;
   m_pAccount = pAccount;


   QString paramCedente = QString(TKT_CONFIG_CEDENTE)
           .arg(m_pCompany->getname())
           .arg(m_pCompany->getcnpj())
           .arg(m_pCompany->getAddrees())
           .arg(m_pCompany->getHouseNumber())
           .arg(m_pCompany->getNeighborhood())
           .arg(m_pCompany->getCity()->getName())
           .arg(m_pCompany->getCEP())
           .arg(m_pCompany->getComplement())
           .arg(m_pCompany->getState()->getSign())
           .arg(m_pTktConfig->getRespEmis()-1)
           .arg(m_pTktConfig->getTipoPessoa()-1)
           .arg(m_pTktConfig->getCodigoCedente())
           .arg(m_pTktConfig->getLayoutBol()-1)
           .arg(m_pTktConfig->getTipoCobranca()-1);

   QString paramConta = QString(TKT_CONFIG_ACCOUNT)
           .arg(m_pAccount->getConta())
           .arg(m_pAccount->getDigitoAgencia())
           .arg(m_pAccount->getAgencia())
           .arg(m_pAccount->getDigitoAgencia());

   QString paramBank = QString(TKT_CONFIG_BANK)
            .arg(m_pBank->getCode())
            .arg(m_pTktConfig->getcnab()-1);

   QString cmdConfig =  QString("%1%2(%3%4%5)")
                        .arg(TKT_PREFIX)
                        .arg(TKT_CONFIG)
                        .arg(paramCedente)
                        .arg(paramConta)
                        .arg(paramBank);

   return Send(cmdConfig);
}

bool BuildTkt::Print(bool bPrinter, QString strPath)
{
    QString cmdPrint =  QString("%1%2").arg(TKT_PREFIX).arg(bPrinter?TKT_PRINT:TKT_PRINT_PDF);

    return Send(cmdPrint);
}


bool BuildTkt::AppendTicket(Dweller *pDweller, QString strValue, QDate dtVencto)
{
    m_TktCount++;

    Ticket *tkt =  new Ticket(pDweller,strValue,dtVencto);

    m_tickets.append(tkt);

    return true;


}

bool BuildTkt::AddTickets()
{
    QString paramCfgTkt;

    for( int i = 0; i < m_tickets.count(); i++ )
    {
       Dweller *pDweller = m_tickets.at(i)->getDweller();
       QString strValue  = m_tickets.at(i)->getValue();
       QDate   dtVencto  = m_tickets.at(i)->getDate();
       int dwSpecie  =  m_pTktConfig->getespecie();
       QString strSpecie;

       if(dwSpecie)
       {
           TktSpecie *spec = TktSpecie::findByPrimaryKey(dwSpecie);
           if(spec)
           {
               strSpecie = spec->getDescription();
               delete spec;
           }
       }

       QString aux;
       Tower *pTower = Tower::findByPrimaryKey(pDweller->gettower());
       Ap    *pAp    =  Ap::findByPrimaryKey(pDweller->getap());
       if( pTower && pAp)
           aux = QString("%1%2").arg(pDweller->gettower()).arg(pAp->getNumber());

       paramCfgTkt += QString(TKT_CONFIG_TICKET)
                          .arg(i+1)
                          .arg(m_pTktConfig->getNossoNumero())
            .arg(aux)
            .arg(m_pTktConfig->getCarteira())
            .arg(strValue.replace(".",","))
            .arg(pDweller->getName())
            .arg(pDweller->getCPF())
            .arg(m_pCompany->getAddrees())
            .arg(m_pCompany->getHouseNumber())
            .arg(m_pCompany->getNeighborhood())
            .arg(QString("%1-%2").arg(pTower?pTower->getName():" ").arg(pAp?pAp->getNumber():" "))
            .arg(m_pCompany->getCity()->getName())
            .arg(m_pCompany->getState()->getSign())
            .arg(m_pCompany->getCEP())
            .arg(m_pTktConfig->getMensagem())
            .arg(m_pTktConfig->getJuros().replace(".",","))
            .arg(dtVencto.toString("dd/MM/yyyy"))
            .arg(strSpecie);

       if( pTower )
           delete pTower;
       if( pAp)
           delete pAp;

    }

    QString cmdPrint =  QString("%1%2(\"%3\")").arg(TKT_PREFIX).arg(TKT_ADD_TICKET).arg(paramCfgTkt);

    return Send(cmdPrint);

}


bool BuildTkt::BuildShipping(QString strDir, QString FileName )
{
    m_ShippNumber++;

    QString cmdShipping =  QString("%1%2").arg(TKT_PREFIX).arg(TKT_BUILD_SHIPPING);

    QString params = QString("\"%1\",%2,%3").arg(strDir).arg(m_ShippNumber).arg(FileName);

    QString cmdSnd = QString("%1(%2)").arg(cmdShipping).arg(params);

    return Send(cmdSnd);


}


Ticket::Ticket(Dweller *dweller, QString value, QDate date)
{
    m_dweller = dweller;
    m_value =  value;
    m_date = date;
}

Dweller *Ticket::getDweller()
{
    return m_dweller;
}
QString Ticket::getValue()
{
    return m_value;
}

QDate   Ticket::getDate()
{
    return m_date;
}
