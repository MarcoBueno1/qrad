#include "buildtkt.h"
#include "tktspecie.h"
#include <QFile>
#include <QDebug>
#include <QThread>
#include <QEventLoop>
#include "acbr.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include "qradround.h"
#include "qradshared.h"
#include "qraddebug.h"
#include <QFileInfo>
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
"PercentualMulta=%18\n"\
"Vencimento=%19\n"\
"DataMoraJuros=%20\n"\
"Especie=%21\n"\
"DataDesconto=%22\n"\
"ValorDesconto=%23\n"



#define TKT_BUILD_SHIPPING "GerarRemessa"


#define TKT_PRINT "Imprimir"
#define TKT_PRINT_PDF "GerarPDF"

#define SEND_PATH "/media/sf_Dvl/acbr/ent/"
#define SEND_NAME "ent.txt"

#define RECEIVE_PATH "/media/sf_Dvl/acbr/sai/"
#define RECEIVE_NAME "ent-resp.txt"

#define TKT_EXTRACT_RETURN "BOLETO.LerRetorno(\"%1\",\"%2\" )"

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

BuildTkt::BuildTkt(QString Path)
{

    m_SendPath = SEND_PATH;
    m_ReceivePath = RECEIVE_PATH;
    m_SendFile = SEND_NAME;
    m_ReceiveFile = RECEIVE_NAME;

//    m_watcher.addPath(m_ReceivePath);
    m_parsepath= Path;
//    QObject::connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(DirModified(QString)));
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
    _tracein_

   debug_message( "Comando: %s\n", cmd.toLatin1().data());

    QString filemane  = QString("%1%2").arg(m_SendPath).arg(m_SendFile);

    m_bDirModified = false;

    QFile *file = new QFile(filemane);
    if( !file->open(QIODevice::WriteOnly))
    {
        delete file;
        _traceout_
        return false;
    }
    bool result = file->write(cmd.toLatin1())?true:false;
    file->flush();
    file->close();
    qDebug() << "Escrito:" << cmd.toLatin1();
    delete file;

    if( result ) // verifica retorno
    {
        while( m_dwTimeout > 0)
        {
             qSleep(1);
             QFileInfo check_file(QString("%1%2").arg(m_ReceivePath).arg(m_ReceiveFile));
             // check if file exists and if yes: Is it really a file and no directory?
             if (check_file.exists() && check_file.isFile() && (check_file.size() > 0))
             {
                 break;
             }
             m_dwTimeout--;
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
        QFile *receivefile = new QFile(QString("%1%2").arg(m_ReceivePath).arg(m_ReceiveFile));
//        qDebug() << "Depois new:";
        if( !receivefile->open(QIODevice::ReadOnly))
        {
//            qDebug() << "Vai deletar receivefile:";
            delete receivefile;
            m_lastError = "Erro na abertura do arquivo de resposta!";
            debug_message( "Sem Resposta....\n");
            _traceout_
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
        QFile::remove(QString("%1%2").arg(m_ReceivePath).arg(m_ReceiveFile));
        qDebug() << "recebido:" << answer;

    }
    _traceout_
    return result;
}

void BuildTkt::CleanUpList()
{
   _tracein_
   for( int i =0; i < m_tickets.count(); i++ )
   {
       delete m_tickets.at(i);
   }
   m_tickets.clear();
   _traceout_
}


bool BuildTkt::Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount)
{
   QString cmdPurge =  QString("%1%2").arg(TKT_PREFIX).arg(TKT_PURGE);

   CleanUpList();

   if(!pCompany || !pTktConfig || !pBank || !pAccount )
   {
       return false;
   }

#ifdef WIN32
   Acbr *pAcbr = Acbr::findByid(1, true);
   if( pAcbr )
   {
       m_SendPath = pAcbr->getSendPath();
       m_ReceivePath = pAcbr->getReceivePath();
       m_SendFile = pAcbr->getSendName();
       m_ReceiveFile = pAcbr->getReceiveName();
       delete pAcbr;

       m_watcher.removePath(m_ReceivePath);
       QObject::disconnect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(DirModified(QString)));
       m_watcher.addPath(m_ReceivePath);
       QObject::connect(&m_watcher, SIGNAL(directoryChanged(QString)), this, SLOT(DirModified(QString)));

   }
#endif

   m_TktCount = 0;
   m_ShippNumber =0;

   m_dwTimeout = 2000;
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

   m_dwTimeout = 2000;
   return Send(cmdConfig);
}

bool BuildTkt::Print(bool bPrinter, QString strPath)
{
    QString cmdPrint =  QString("%1%2").arg(TKT_PREFIX).arg(bPrinter?TKT_PRINT:TKT_PRINT_PDF);

    m_dwTimeout = 60000;
    bool Ret = Send(cmdPrint);
    if( Ret )
    {
       QFile::rename("C:\\ACBrMonitorPLUS\\boleto.pdf",strPath);
    }

    return Ret;
}


bool BuildTkt::AppendTicket(Dweller *pDweller,
                            QString strValue,
                            QDate dtVencto,
                            QString NossoNumero,
                            QString SeuNumero,
                            QString Mensagem,
                            QString Discount)
{
    m_TktCount++;

    Ticket *tkt =  new Ticket(pDweller,strValue,dtVencto, NossoNumero,SeuNumero,Mensagem, Discount);

    m_tickets.append(tkt);

    return true;


}


QString diacriticLetters_;
QStringList noDiacriticLetters_;

QString removeAccents(QString s) {
    if (diacriticLetters_.isEmpty()) {
        diacriticLetters_ = QString::fromUtf8("ŠŒŽšœžŸ¥µÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝßàáâãäåæçèéêëìíîïðñòóôõöøùúûüýÿ");
        noDiacriticLetters_ << "S"<<"OE"<<"Z"<<"s"<<"oe"<<"z"<<"Y"<<"Y"<<"u"<<"A"<<"A"<<"A"<<"A"<<"A"<<"A"<<"AE"<<"C"<<"E"<<"E"<<"E"<<"E"<<"I"<<"I"<<"I"<<"I"<<"D"<<"N"<<"O"<<"O"<<"O"<<"O"<<"O"<<"O"<<"U"<<"U"<<"U"<<"U"<<"Y"<<"s"<<"a"<<"a"<<"a"<<"a"<<"a"<<"a"<<"ae"<<"c"<<"e"<<"e"<<"e"<<"e"<<"i"<<"i"<<"i"<<"i"<<"o"<<"n"<<"o"<<"o"<<"o"<<"o"<<"o"<<"o"<<"u"<<"u"<<"u"<<"u"<<"y"<<"y";
    }

    QString output = "";
    for (int i = 0; i < s.length(); i++) {
        QChar c = s[i];
        int dIndex = diacriticLetters_.indexOf(c);
        if (dIndex < 0) {
            output.append(c);
        } else {
            QString replacement = noDiacriticLetters_[dIndex];
            output.append(replacement);
        }
    }

    return output;
}


QString BuildTkt::MountYourNumvber(QString SeuNumero, QString ap, QString tow )
{
    if(SeuNumero.isEmpty())
        return SeuNumero;

    QString ret = QString("%1 %2").arg(ap).arg(tow.mid(0,1).toUpper());

    if( ret.trimmed().isEmpty())
        return SeuNumero;

    return ret;
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
       QString NossoNumero = QString("%1").arg(m_tickets.at(i)->getNossoNumero());
       QString SeuNumero = QString("%1").arg(m_tickets.at(i)->getSeuNumero());
       QString ValorDiscount = m_tickets.at(i)->getDiscount();

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

       if(NossoNumero.isEmpty())
           NossoNumero = m_pTktConfig->getNossoNumero();


       QString MountedYourNumber = MountYourNumvber(SeuNumero, pAp?pAp->getNumber():"",pTower?pTower->getName():"" );
       if( MountedYourNumber.isEmpty())
           MountedYourNumber = aux;

       QString msg = m_tickets.at(i)->getMensagem();
       if( msg.isEmpty())
       {
           msg = m_pTktConfig->getMensagem();
           msg = QString("%1%2/%3").arg(msg).arg(dtVencto.longMonthName(dtVencto.month()).toUpper()).arg(dtVencto.year());
       }
       double valor       = strValue.replace(",", ".").toDouble();
       debug_message("valor=%02.02f\n", valor);
       double percentJuros = m_pTktConfig->getJuros().replace(",",".").toDouble();
       debug_message("percentJuros=%02.02f\n", percentJuros);

       QString JurosDia = QString("%1").arg(QRadRound::PowerRound(QRadRound::PowerRound(valor/dtVencto.daysInMonth()/100)*percentJuros));
       debug_message("JurosDia Passo1=%02.02f\n", QRadRound::PowerRound(valor/dtVencto.daysInMonth()/100));

       debug_message("JurosDia=%s\n", JurosDia.toLatin1().data());
       JurosDia.replace(".", ",");
//       QString ValorDiscount = QString("%1").arg(QRadRound::PowerRound(QRadRound::PowerRound(valor/100)*(m_pTktConfig->getDiscount())));
//       ValorDiscount.replace(".", ",");

       paramCfgTkt += QString(TKT_CONFIG_TICKET)
                          .arg(i+1)
                          .arg(MountedYourNumber)
            .arg(NossoNumero)
            .arg(m_pTktConfig->getCarteira())
            .arg(strValue.replace(".",","))
//            .arg(strValue)
            .arg(removeAccents(pDweller->getName()))
            .arg(pDweller->getCPF())
            .arg(m_pCompany->getAddrees())
            .arg(m_pCompany->getHouseNumber())
            .arg(m_pCompany->getNeighborhood())
            .arg(QString("%1-%2").arg(pTower?pTower->getName():" ").arg(pAp?pAp->getNumber():" "))
            .arg(m_pCompany->getCity()->getName())
            .arg(m_pCompany->getState()->getSign())
            .arg(m_pCompany->getCEP())
            .arg(msg)
            .arg(JurosDia)
            .arg(m_pTktConfig->getMulta().replace(".",","))
            .arg(dtVencto.toString("dd/MM/yyyy"))
            .arg(dtVencto.toString("dd/MM/yyyy"))
            .arg(strSpecie)
            .arg(dtVencto.toString("dd/MM/yyyy"))
            .arg(ValorDiscount);

       if( pTower )
           delete pTower;
       if( pAp)
           delete pAp;

    }

    QString cmdPrint =  QString("%1%2(\"%3\")").arg(TKT_PREFIX).arg(TKT_ADD_TICKET).arg(paramCfgTkt);

    m_dwTimeout = 2000;
    return Send(cmdPrint);

}


bool BuildTkt::BuildShipping(QString strDir, QString FileName )
{
    m_ShippNumber++;

    QString cmdShipping =  QString("%1%2").arg(TKT_PREFIX).arg(TKT_BUILD_SHIPPING);

    QString params = QString("\"%1\",%2,%3").arg(strDir).arg(m_ShippNumber).arg(FileName);

    QString cmdSnd = QString("%1(%2)").arg(cmdShipping).arg(params);

    m_dwTimeout = 40000;
    return Send(cmdSnd);
}


/*
[Titulo1]
Sacado.Nome=
Sacado.CNPJCPF=
Vencimento=29/06/2012
DataDocumento=30/12/1899
NumeroDocumento=NF 12345
DataProcessamento=06/06/2013
NossoNumero=12345
Carteira=123
ValorDocumento=300
DataOcorrencia=27/06/2012
DataCredito=28/06/2012
DataBaixa=29/06/2012
ValorDespesaCobranca=3,26
ValorAbatimento=0
ValorDesconto=0
ValorMoraJuros=0
ValorIOF=0
ValorOutrasDespesas=0
ValorOutrosCreditos=0
ValorRecebido=300
SeuNumero=NF 00548   12345
CodTipoOcorrencia=toRetornoLiquidado
DescricaoTipoOcorrencia=06-Liquidação
MotivoRejeicao1=04-Compensação Eletrônica.

*/
#define TKT_LIQUIDATED "toRetornoLiquidado"
#define TKT_REGISTERED "toRetornoRegistroConfirmado"

bool BuildTkt::ExtractReturn(QList<BankTicket *> *tickets, QString strDir, QString FileName )
{

    QString cmd =  QString(TKT_EXTRACT_RETURN).arg(strDir).arg(FileName);
    m_dwTimeout = 1000;
    if( !Send(cmd))
    {
        return false;
    }

#ifdef _WIN32
    QSettings settings(QString("%1Retorno.Ini").arg(strDir),
                       QSettings::IniFormat);
#else
    QSettings settings(QString("/media/sf_Dvl/Retorno.Ini"),
                       QSettings::IniFormat);
#endif


    QString Nome         ;
    QString cpf          ;
    QString vencto       ;
    QString NossoNumero  ;
    QString SeuNumero    ;
    QString VlrRecebido  ;
    QString VlrDocumento ;
    QString TipoOperacao ;
    QString dtPagto;
    QRAD_BANKTKT_TYPEOP tpOp = tpOther;

    for( int i= 0; ; i++)
    {
      Nome         = settings.value(QString("Titulo%1/Sacado.Nome").arg(i+1)).toString();
      cpf          = settings.value(QString("Titulo%1/Sacado.CNPJCPF").arg(i+1)).toString();
      vencto       = settings.value(QString("Titulo%1/Vencimento").arg(i+1)).toString();
      NossoNumero  = settings.value(QString("Titulo%1/NossoNumero").arg(i+1)).toString();
      SeuNumero    = settings.value(QString("Titulo%1/NumeroDocumento").arg(i+1)).toString();
      VlrRecebido  = settings.value(QString("Titulo%1/ValorRecebido").arg(i+1)).toStringList().join(",");
      VlrDocumento = settings.value(QString("Titulo%1/ValorDocumento").arg(i+1)).toStringList().join(",");
      TipoOperacao = settings.value(QString("Titulo%1/CodTipoOcorrencia").arg(i+1)).toString();
      dtPagto      = settings.value(QString("Titulo%1/DataCredito").arg(i+1)).toString();

      if( Nome.isEmpty() && cpf.isEmpty() && vencto.isEmpty() && NossoNumero.isEmpty() && SeuNumero.isEmpty() && VlrRecebido.isEmpty() && VlrDocumento.isEmpty() )
              break;
      else
      {
//#ifdef __DEBUG__

          qDebug() << "Titulo......:" << i+1;
          qDebug() << "Nome........:" << Nome;
          qDebug() << "cpf.........:" <<cpf;
          qDebug() << "vencto......:"  << vencto;
          qDebug() << "NossoNumero.:" <<NossoNumero;
          qDebug() << "SeuNumero...:"   << SeuNumero;
          qDebug() << "VlrRecebido.:" <<VlrRecebido;
          qDebug() << "VlrDocumento:" <<VlrDocumento;
          qDebug() << "TipoOperacao:" <<TipoOperacao;
//#endif

          if(tickets)
          {
              if( TKT_LIQUIDATED == TipoOperacao )
                  tpOp = tpLiquidated;
              else if(TKT_REGISTERED == TipoOperacao )
                  tpOp = tpRegistered;
              else
                  tpOp = tpOther;


              BankTicket *pNew = new BankTicket("",
                                                NossoNumero,
                                                SeuNumero,
                                                "",
                                                QDate::fromString(vencto,FMT_DATE),
                                                tpOp==tpLiquidated,
                                                QDate::fromString(dtPagto, FMT_DATE),
                                                "",
                                                VlrDocumento,
                                                VlrRecebido,
                                                tpOp);
              tickets->append(pNew);
          }
       }



    }

    return true;
}

bool BuildTkt::Parse(QList<BankTicket*> *tikets,QString Path)
{
    QString strDir;
    QString FileName;
//#ifdef _WIN32
    strDir = Path.mid(0,Path.lastIndexOf("\\")+1);
    FileName = Path.mid(Path.lastIndexOf("\\")+1);
//#elseif
//    strDir = Path.mid(0,Path.lastIndexOf("/")+1);
//    FileName = Path.mid(Path.lastIndexOf("/")+1);
//#endif

    qDebug() << strDir;
    qDebug() << FileName;
    return ExtractReturn(tikets, strDir, FileName );
}

bool BuildTkt::Parse(QList<BankTicket*> *tikets)
{
    return Parse(tikets,m_parsepath);
}



Ticket::Ticket(Dweller *dweller,
               QString value,
               QDate date,
               QString NossoNumero,
               QString SeuNumero,
               QString Mensagem,
               QString Discount)
{
    m_dweller = dweller;
    m_value =  value;
    m_date = date;
    m_NossoNumero = NossoNumero;
    m_SeuNumero = SeuNumero;
    m_Mensagem = Mensagem;
    m_Discount = Discount;
}

Dweller *Ticket::getDweller()
{
    return m_dweller;
}
QString Ticket::getValue()
{
    return m_value;
}
QString Ticket::getNossoNumero()
{
    return m_NossoNumero;
}
QString Ticket::getSeuNumero()
{
    return m_SeuNumero;
}

QString Ticket::getMensagem()
{
    return m_Mensagem;
}

QDate   Ticket::getDate()
{
    return m_date;
}
QString Ticket::getDiscount()
{
    return m_Discount;
}

