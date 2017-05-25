#include "accounttyperegister.h"
#include "ui_accounttyperegister.h"
#include "dsmsgmessages.h"

#define SQL_SELECT_ACCOUNTTYPE  "select * from dsm_fin_accounttype where id = (%1)"
#define SQL_INSERT_ACCOUNTTYPE  "insert into dsm_fin_accounttype (description, type, removed) values (?, ?, false)"
#define SQL_UPDATE_ACCOUNTTYPE  "update dsm_fin_accounttype set (description, type) = (?, ?)  where id = (?)"

AccountTypeRegister::AccountTypeRegister(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountTypeRegister)
{
    m_ui->setupUi(this);

    m_accountTypeId = 0;
    m_accountTypeName.clear();
    m_ui->lineEditAccountType->setFocus();

    connect(m_ui->btnSave, SIGNAL(pressed()), this, SLOT(SaveAccountType()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(reject()));

}

AccountTypeRegister::~AccountTypeRegister()
{
    delete m_ui;
}

void AccountTypeRegister::changeEvent(QEvent *e)
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

void AccountTypeRegister::SendAccountTypeId(int accountTypeId)
{
    QSqlQueryModel  *select = new QSqlQueryModel;
    select->setQuery(QString(SQL_SELECT_ACCOUNTTYPE).arg(accountTypeId));

    m_ui->lineEditAccountType->setText(select->record(0).value("description").toString());
    m_ui->comboBoxType->setCurrentIndex(select->record(0).value("type").toInt());

    m_accountTypeId = accountTypeId;
    delete select;
}

void AccountTypeRegister::SaveAccountType(void)
{
    QSqlQuery   *updateAccountType = new QSqlQuery;

    if (!m_ui->lineEditAccountType->text().isEmpty())
    {
        m_accountTypeName = m_ui->lineEditAccountType->text();

        if (m_accountTypeId == 0)
        {
            updateAccountType->prepare(SQL_INSERT_ACCOUNTTYPE);
        }
        else
        {
            updateAccountType->prepare(SQL_UPDATE_ACCOUNTTYPE);
        }

        /** description */
        updateAccountType->addBindValue(m_ui->lineEditAccountType->text().toUpper());

        /** type */
        updateAccountType->addBindValue(m_ui->comboBoxType->currentIndex());

        if (m_accountTypeId != 0)
        {
            /** id */
            updateAccountType->addBindValue(m_accountTypeId);
        }
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
    }
    delete updateAccountType;
}

QString AccountTypeRegister::GetAccountTypeName(void)
{
    return m_accountTypeName;
}
