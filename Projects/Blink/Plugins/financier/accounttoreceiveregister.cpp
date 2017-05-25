#include "accounttoreceiveregister.h"
#include "ui_accounttoreceiveregister.h"

#include "dsmmoney.h"
#include "dsmsgmessages.h"

AccountToReceiveRegister::AccountToReceiveRegister(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountToReceiveRegister)
{
    m_ui->setupUi(this);

    m_expMoney          = new QRegExpValidator(QRegExp(REGEXP_MONEY), 0);
    m_modelAccountType  = new QSqlQueryModel;
    m_modelClient     = new QSqlQueryModel;

    m_ui->lineEditValue->setValidator(m_expMoney);

    m_accountToReceiveId = 0;
    m_accountToReceiveDescription.clear();
    m_ui->lineEditDesciption->setFocus();

    m_ui->dateEditVencDate->setDate(QDate::currentDate());

    GetAccountTypeValues();
    GetClientValues();
    m_ui->comboBoxAccountType->setCurrentIndex(0);

    connect(m_ui->btnSave, SIGNAL(pressed()), this, SLOT(SaveAccountToReceive()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(reject()));
    connect(m_ui->textEditObs, SIGNAL(tabPressed()), this, SLOT(changeFocus()) );
}

AccountToReceiveRegister::~AccountToReceiveRegister()
{
    delete m_ui;
    delete m_expMoney;
    delete m_modelAccountType;
    delete m_modelClient;
}

void AccountToReceiveRegister::changeEvent(QEvent *e)
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

void AccountToReceiveRegister::GetAccountTypeValues(void)
{
    m_modelAccountType->setQuery(QString(SQL_SELECT_ACCOUNTTYPE_COMBO).arg(AccountTypeToReceive).arg(AccountTypeBoth));
    m_ui->comboBoxAccountType->setModel(m_modelAccountType);
    m_ui->comboBoxAccountType->setModelColumn(1);
}

void AccountToReceiveRegister::GetClientValues(void)
{
    m_modelClient->setQuery(SQL_SELECT_CLIENT_COMBO);
    m_ui->comboBoxClient->setModel(m_modelClient);
    m_ui->comboBoxClient->setModelColumn(1);
}

void AccountToReceiveRegister::SendAccountToReceiveId(int accountToReceiveId)
{
    AccountToReceiveModel *accountToReceiveModel = AccountToReceiveModel::findByPrimaryKey(accountToReceiveId);

    m_ui->comboBoxAccountType->setModelColumn(0);

    m_ui->lineEditDesciption->setText(accountToReceiveModel->getDescription());
    m_ui->dateEditVencDate->setDate(accountToReceiveModel->getVencDate());
    m_ui->comboBoxAccountType->setCurrentIndex(m_ui->comboBoxAccountType->findText(QString("%1").arg(accountToReceiveModel->getAccountTypeId())));
    m_ui->lineEditValue->setText(DSMMoney::MoneyHumanForm2(accountToReceiveModel->getValue()));

    if (accountToReceiveModel->getClientId() == 0)
    {
        m_ui->groupBoxClient->setChecked(false);
    }
    else
    {
        m_ui->groupBoxClient->setChecked(true);
        m_ui->comboBoxClient->setModelColumn(0);
        m_ui->comboBoxClient->setCurrentIndex(m_ui->comboBoxClient->findText(QString("%1").arg(accountToReceiveModel->getClientId())));
        m_ui->comboBoxClient->setModelColumn(1);
    }

    m_ui->textEditObs->setPlainText(accountToReceiveModel->getObs());

    m_ui->comboBoxAccountType->setModelColumn(1);

    m_ui->groupBoxInfo->setEnabled(!accountToReceiveModel->getPaid());
    m_ui->groupBoxClient->setEnabled(!accountToReceiveModel->getPaid());
    m_ui->groupBoxObs->setEnabled(!accountToReceiveModel->getPaid());
    m_ui->btnSave->setEnabled(!accountToReceiveModel->getPaid());

    m_accountToReceiveId = accountToReceiveId;
    delete accountToReceiveModel;
}

void AccountToReceiveRegister::SaveAccountToReceive(void)
{
    if (!m_ui->lineEditDesciption->text().isEmpty())
    {
        AccountToReceiveModel *accountToReceiveModel = new AccountToReceiveModel;
        m_accountToReceiveDescription = m_ui->lineEditDesciption->text();
        m_ui->comboBoxAccountType->setModelColumn(0);

        accountToReceiveModel->setDescription(m_ui->lineEditDesciption->text().toUpper());
        accountToReceiveModel->setIssueDate(QDate::currentDate());
        accountToReceiveModel->setVencDate(m_ui->dateEditVencDate->date());
        accountToReceiveModel->setAccountTypeId(m_ui->comboBoxAccountType->currentText().toInt());
        accountToReceiveModel->setValue(DSMMoney::MoneyComputerForm2(m_ui->lineEditValue->text()));

        if (m_ui->groupBoxClient->isChecked())
        {
            m_ui->comboBoxClient->setModelColumn(0);
            accountToReceiveModel->setClientId(m_ui->comboBoxClient->currentText().toInt());
            m_ui->comboBoxClient->setModelColumn(1);
        }
        else
        {
            accountToReceiveModel->setClientId(0);
        }

        accountToReceiveModel->setPaid(false);
        accountToReceiveModel->setObs(m_ui->textEditObs->toPlainText().toUpper());

        if (m_accountToReceiveId != 0)
        {
            accountToReceiveModel->setId(m_accountToReceiveId);
        }
        if(!accountToReceiveModel->Save())
        {
            QMessageBox::critical(this, MSG_ERROR_DATABASE_TITLE, MSG_ERROR_SAVE_ACCOUNT);
            this->reject();
        }
        else
        {
            QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_RECORD_OK);
            this->accept();
        }
        m_ui->comboBoxAccountType->setModelColumn(1);
        delete accountToReceiveModel;
    }
}

QString AccountToReceiveRegister::GetAccountToReceiveDescription(void)
{
    return m_accountToReceiveDescription;
}

void AccountToReceiveRegister::changeFocus()
{
    m_ui->btnSave->setFocus();
}
