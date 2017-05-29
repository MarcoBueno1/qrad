#include "buildtkt.h"
#include <QFile>

#define TKT_PREFIX "BOLETO."

#define TKT_PURGE "LimparLista"
#define TKT_CONFIG "ConfigurarDados"
 #define TKT_CONFIG_CEDENTE "   [Cedente]"\
"Nome=%1"\
"CNPJCPF=%2"\
"Logradouro=%3"\
"Numero=%4"\
"Bairro=%5"\
"Cidade=%6"\
"CEP=%7"\
"Complemento=%8"\
"UF=%9"\
"RespEmis=%10"\
"TipoPessoa=%11"\
"CodigoCedente=%12"\
"LayoutBol=%13"\
"CaracTitulo=%14"

 #define TKT_CONFIG_ACCOUNT "   [Conta]"\
"Conta=%1"\
"DigitoConta=%2"\
"Agencia=%3"\
"DigitoAgencia=%4"


 #define TKT_CONFIG_BANK "   [Banco]"\
"Numero=%1"\
"CNAB=%2"


#define TKT_ADD_TICKET "IncluirTitulos"
 #define TKT_CONFIG_TICKET "[Titulo%1]"\
"NumeroDocumento=%2"\
"NossoNumero=%3"\
"Carteira=%4"\
"ValorDocumento=%5"\
"Sacado.NomeSacado=%6"\
"Sacado.CNPJCPF=%7"\
"Sacado.Logradouro=%8"\
"Sacado.Numero=%9"\
"Sacado.Bairro=%10"\
"Sacado.Complemento=%11"\
"Sacado.Cidade=%12"\
"Sacado.UF=%13"\
"Sacado.CEP=%14"\
"Mensagem=%16"\
"ValorMoraJuros=%17"\
"Vencimento=%18"



#define TKT_PRINT "Imprimir"
#define TKT_PRINT_PDF "GerarPDF"

#define FILE_PATH "C:\\ACBRMonitor\\"
#define FILE_NAME "Send.txt"

bool BuildTkt::Send(QString cmd)
{
    QString filemane  = QString("%1%2").arg(FILE_PATH).arg(FILE_NAME);

    QFile *file = new QFile(filemane);
    if( !file->open(QIODevice::WriteOnly))
    {
        delete file;
        return false;
    }
    bool result = file->write(cmd.toLatin1())?true:false;
    file->flush();
    file->close();
    delete file;
    return result;
}

bool BuildTkt::Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount)
{
   QString cmdPurge =  QString("%1%2").arg(TKT_PREFIX).arg(TKT_PURGE);

   m_TktCount = 0;

   if( !Send(cmdPurge))
       return false;

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
                        .arg(paramBank)
                        .arg(paramBank);

   return Send(cmdConfig);
}

bool BuildTkt::Print(bool bPrinter, QString strPath)
{
    QString cmdPrint =  QString("%1%2").arg(TKT_PREFIX).arg(bPrinter?TKT_PRINT:TKT_PRINT_PDF);

    return Send(cmdPrint);
}


bool BuildTkt::AddTicket(Dweller *pDweller, QString strValue, QDate dtVencto)
{
    m_TktCount++;

    QString paramCfgTkt = QString(TKT_CONFIG_TICKET)
                          .arg(m_TktCount)
                          .arg(m_pTktConfig->getNossoNumero())
            .arg(QString("%1%2").arg(pDweller->getTower()->getid()).arg(pDweller->getAp()->getNumber()))
            .arg(m_pTktConfig->getCarteira())
            .arg(strValue.replace(".",","))
            .arg(pDweller->getName())
            .arg(pDweller->getCPF())
            .arg(m_pCompany->getAddrees())
            .arg(m_pCompany->getHouseNumber())
            .arg(m_pCompany->getNeighborhood())
            .arg(QString("%1-%2").arg(pDweller->getTower()->getName()).arg(pDweller->getAp()->getNumber()))
            .arg(m_pCompany->getCity()->getName())
            .arg(m_pCompany->getState()->getSign())
            .arg(m_pCompany->getCEP())
            .arg(m_pTktConfig->getMensagem())
            .arg(m_pTktConfig->getJuros().replace(".",","))
            .arg(dtVencto.toString("dd/MM/yyyy"));

    QString cmdPrint =  QString("%1%2(%3)").arg(TKT_PREFIX).arg(TKT_ADD_TICKET).arg(paramCfgTkt);

    return Send(cmdPrint);

}
