#include "paidaccount.h"
#include "ui_paidaccount.h"

#include "dsmsgmessages.h"
#include "qradmoney.h"
#include "qradconfig.h"


#define SQL_UPDATE_PAIDACCOUNT      "update fin_accounttopay set (paiddate, paid, valuepaid) = (?, ?, ?)  where id = (?)"
#define SQL_UPDATE_RECEIVEACCOUNT   "update fin_accounttoreceive set (paiddate, paid, valuepaid) = (?, ?, ?)  where id = (?)"

PaidAccount::PaidAccount(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PaidAccount)
{
    m_ui->setupUi(this);

    m_expMoney = new QRegExpValidator(QRegExp(REGEXP_MONEY), 0);

    m_ui->lineEditPaidValue->setValidator(m_expMoney);

    m_accountId = 0;
    m_ui->dateEditPaidDate->setDate(QDate::currentDate());
    m_ui->dateEditPaidDate->setFocus();

    connect(m_ui->btnSave, SIGNAL(pressed()), this, SLOT(SavePaidAccount()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(reject()));
}

PaidAccount::~PaidAccount()
{
    delete m_ui;
    delete m_expMoney;
}

void PaidAccount::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PaidAccount::SendPaidAccountId(int accountId, DSM_AccountType_t accountType)
{
    m_accountId = accountId;
    m_accountType = accountType;

    if (m_accountType == AccountTypeToPay)
    {
        AccountToPayModel *accountToPayModel = AccountToPayModel::findByPrimaryKey(m_accountId);
        m_ui->lineEditPaidValue->setText(QRadMoney::MoneyHumanForm2(accountToPayModel->getValue()));
        delete accountToPayModel;
    }
    else
    {
        AccountToReceiveModel *accountToReceiveModel = AccountToReceiveModel::findByPrimaryKey(m_accountId);
        m_ui->lineEditPaidValue->setText(QRadMoney::MoneyHumanForm2(accountToReceiveModel->getValue()));
        delete accountToReceiveModel;
    }
}

void PaidAccount::SavePaidAccount(void)
{
    if (QRadMoney::MoneyComputerForm2(m_ui->lineEditPaidValue->text()) == 0)
    {
        QMessageBox::information(this, MSG_INFO_TITLE, MSG_QUESTION_ACCOUNT_ZERO);
        QRadConfig::GoTo(m_ui->lineEditPaidValue);
    }
    else
    {
        QSqlQuery   *updateAccountType = new QSqlQuery;

        if (m_accountType == AccountTypeToPay)
        {
            updateAccountType->prepare(SQL_UPDATE_PAIDACCOUNT);
        }
        else
        {
            updateAccountType->prepare(SQL_UPDATE_RECEIVEACCOUNT);
        }

        /** paiddate */
        updateAccountType->addBindValue(m_ui->dateEditPaidDate->text());

        /** paid */
        updateAccountType->addBindValue(true);

        /** valuepaid */
        updateAccountType->addBindValue(QRadMoney::MoneyComputerForm2(m_ui->lineEditPaidValue->text()));

        /** id */
        updateAccountType->addBindValue(m_accountId);

        if(!updateAccountType->exec())
        {
            QMessageBox::critical(this, MSG_ERROR_DATABASE_TITLE, MSG_ERROR_DATABASE);
            this->reject();
        }
        else
        {
            QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_RECORD_OK);
            this->accept();
        }
        delete updateAccountType;
    }
}

double PaidAccount::getValuePaid()
{
    return QRadMoney::MoneyComputerForm2(m_ui->lineEditPaidValue->text());
}
