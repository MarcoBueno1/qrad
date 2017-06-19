#include "showpayments.h"
#include "ui_showpayments.h"
#include "ui_showpayments.h"
#include "parsepayments.h"
#include <QMessageBox>
#include "ticket.h"
#include "accounttoreceivemodel.h"
#include "accounttoreceivehistorymodel.h"
#include "qradshared.h"
#include "qradconfig.h"
#include "qraddebug.h"


#define SQL_PAYED "select seunumero as \"SeuNumero\",nomepagador as \"Pagador\", datavencto as \"Vencto\", valor as \"Valor R$\", datapagto as \"Pago Em\", valorpago as \"Pago R$\" from mem_table"

ShowPayments::ShowPayments(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowPayments)
{
    ui->setupUi(this);
    m_model = NULL;

    connect(ui->pushButtonOk, SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()),this,SLOT(reject()));



}

ShowPayments::~ShowPayments()
{
    if( m_model )
        delete m_model;
    delete ui;
}

void ShowPayments::Exec(QString Path)
{
    ParsePayments *Payments = new ParsePayments();

    if( !Payments->BuildFrom(Path))
    {
        QMessageBox::warning(this, "Oops!", QString("Não foi encontrado nenhum pagamento no arquivo %1").arg(Path));
        delete Payments;
        return ;
    }
    QSqlDatabase db = QSqlDatabase::database("memory_sqlql");

    m_model = new QSqlQueryModel();

    m_model->setQuery(SQL_PAYED,db);
    if( m_model->rowCount())
    {
        QMessageBox::warning(this, "Oops!", QString("Não foi encontrado nenhum pagamento no arquivo %1, tabela vazia").arg(Path));
        delete Payments;
        return ;
    }
    ui->tableView->setModel(m_model);
    if( QDialog::Accepted == exec())
    {
        /// Ok Proced payment..
        PersistPayments();
    }
}
void ShowPayments::PersistPayments()
{
   bool bError = false;

    for( int i=0; i < m_model->rowCount(); i++ )
    {
        QString SeuNumero = m_model->index(i, 0).sibling(i,0).data().toString();
        QString Vencto    = m_model->index(i, 2).sibling(i,2).data().toString();
        QString Valor     = m_model->index(i, 3).sibling(i,3).data().toString();
        QString PagoEm    = m_model->index(i, 4).sibling(i,4).data().toString();
        QString ValorPago = m_model->index(i, 5).sibling(i,5).data().toString();


        Valor.replace(",", ".");
        ValorPago.replace(",", ".");
        QDate dtVencto = QDate::fromString(Vencto, FMT_DATE);

        ticketList *tktlst = ticket::findBy(QString("select * from ticket where seunumero =%1 and valor=%2 and vencto='%3'").arg(SeuNumero).arg(Valor).arg(dtVencto.toString(FMT_DATE_DB)));
        ticket *tkt = tktlst?tktlst->at(0):0;
        if( !tkt )
        {
            bError = true ;
            debug_message("%s\n",QString("Não encontrado: SeuNumero=%1, Valor=%2, vencto=%3").arg(SeuNumero).arg(Valor).arg(dtVencto.toString(FMT_DATE)).toLatin1().data());
        }
        else
        {
            QDate dtVencto = QDate::fromString(PagoEm, FMT_DATE);
            tkt->setPagoEm(dtVencto);
            tkt->setValorPago(ValorPago.toDouble());
            tkt->setStatus(stPaid);
            if(!tkt->Save())
            {
                bError = true ;
                debug_message("%s\n",QString("Não possivel salvar informações de %1").arg(tkt->getObs()).toLatin1().data());
            }
            else
            {
                AccountToReceiveModel *account = AccountToReceiveModel::findById(tkt->getAccountId(),true);
                if( account )
                {
                    account->setPaid(true);
                    account->setValuePaid(tkt->getValorPago());
                    account->setPaidDate(tkt->getPagoEm());
                    if(account->Save())
                    {
                        AccountToReceiveHistoryModel *his =  new AccountToReceiveHistoryModel;
                        his->setAccountToReceiveId(account->getId());
                        his->setDate(QDate::currentDate());
                        his->setUserId(QRadConfig::GetCurrentUserId());
                        his->setTypeOperation(AccountOpPaid);
                        his->Save();
                        delete his;

                    }
                    else
                    {
                        bError = true ;
                        debug_message("%s\n",QString("Não possivel salvar pagamento(accounttoreceive) de %1").arg(account->getId()).toLatin1().data());

                    }
                    delete account;
                }
            }
        }
    }
    if(bError)
    {
        QMessageBox::warning(this, "Oops!", "Ocorreram erros durante o processamento dos pagamentos!");
    }
}
