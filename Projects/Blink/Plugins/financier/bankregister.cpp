#include "bankregister.h"
#include "ui_bankregister.h"
#include "dsmsgmessages.h"
#define SQL_SELECT_BANK  "select * from fin_bank where id = (%1)"
#define SQL_INSERT_BANK  "insert into fin_bank (code, description, removed) values (?, ?, false)"
#define SQL_UPDATE_BANK  "update fin_bank set (code, description) = (?, ?)  where id = (?)"

BankRegister::BankRegister(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::BankRegister)
{
    m_ui->setupUi(this);

    m_bankId = 0;
    m_bankName.clear();
    m_ui->lineEditCode->setFocus();

    connect(m_ui->btnSave, SIGNAL(pressed()), this, SLOT(SaveBank()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(reject()));

}

BankRegister::~BankRegister()
{
    delete m_ui;
}

void BankRegister::changeEvent(QEvent *e)
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

void BankRegister::SendBankId(int bankId)
{
    QSqlQueryModel  *select = new QSqlQueryModel;
    select->setQuery(QString(SQL_SELECT_BANK).arg(bankId));

    m_ui->lineEditCode->setText(select->record(0).value("code").toString());
    m_ui->lineEditDescription->setText(select->record(0).value("description").toString());

    m_bankId = bankId;
    delete select;
}

void BankRegister::SaveBank(void)
{
    QSqlQuery   *updateBank = new QSqlQuery;

    if ( (!m_ui->lineEditCode->text().isEmpty()) &&
         (!m_ui->lineEditDescription->text().isEmpty()) )
    {

        BankModelList *bankModels = BankModel::findByCode(m_ui->lineEditCode->text().toUpper().trimmed());
        if ( (bankModels != NULL) && (bankModels->at(0)->getId() != m_bankId) )
        {
            QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_BANK_ALREADY_EXIST);
        }
        else
        {
            m_bankName = m_ui->lineEditDescription->text();

            if (m_bankId == 0)
            {
                updateBank->prepare(SQL_INSERT_BANK);
            }
            else
            {
                updateBank->prepare(SQL_UPDATE_BANK);
            }

            /** type */
            updateBank->addBindValue(m_ui->lineEditCode->text().toUpper().trimmed());

            /** description */
            updateBank->addBindValue(m_ui->lineEditDescription->text().toUpper());


            if (m_bankId != 0)
            {
                /** id */
                updateBank->addBindValue(m_bankId);
            }
            if(!updateBank->exec())
            {
                qDebug() << updateBank->lastError().text();
                QMessageBox::critical(this, MSG_ERROR_DATABASE_TITLE, MSG_ERROR_DATABASE);
                this->reject();
            }
            else
            {
                QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_RECORD_OK);
                this->accept();
            }
        }
    }
    delete updateBank;
}

QString BankRegister::GetBankName(void)
{
    return m_bankName;
}
