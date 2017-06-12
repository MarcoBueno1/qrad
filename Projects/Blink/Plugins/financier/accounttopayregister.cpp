#include "accounttopayregister.h"
#include "ui_accounttopayregister.h"
#include "qradmoney.h"
#include "dsmsgmessages.h"

AccountToPayRegister::AccountToPayRegister(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountToPayRegister)
{
    m_ui->setupUi(this);

    m_expMoney          = new QRegExpValidator(QRegExp(REGEXP_MONEY), 0);
    m_modelAccountType  = new QSqlQueryModel;
    m_modelSupplier     = new QSqlQueryModel;
    m_modelBank         = new QSqlQueryModel;

    m_ui->lineEditValue->setValidator(m_expMoney);

    m_accountToPayId = 0;
    m_accountToPayDescription.clear();
    m_ui->lineEditDesciption->setFocus();

    m_ui->dateEditVencDate->setDate(QDate::currentDate());

    GetAccountTypeValues();
    GetSupplierValues();
    GetBankValues();
    m_ui->comboBoxAccountType->setCurrentIndex(0);

    connect(m_ui->btnSave, SIGNAL(pressed()), this, SLOT(SaveAccountToPay()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(reject()));
    connect(m_ui->textEditObs, SIGNAL(tabPressed()), this, SLOT(changeFocus()) );

}

AccountToPayRegister::~AccountToPayRegister()
{
    delete m_ui;
    delete m_expMoney;
    delete m_modelAccountType;
    delete m_modelSupplier;
    delete m_modelBank;
}

void AccountToPayRegister::changeEvent(QEvent *e)
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

void AccountToPayRegister::GetAccountTypeValues(void)
{
    m_modelAccountType->setQuery(QString(SQL_SELECT_ACCOUNTTYPE_COMBO).arg(AccountTypeToPay).arg(AccountTypeBoth));
    m_ui->comboBoxAccountType->setModel(m_modelAccountType);
    m_ui->comboBoxAccountType->setModelColumn(1);
}

void AccountToPayRegister::GetSupplierValues(void)
{
    m_modelSupplier->setQuery(SQL_SELECT_SUPPLIER_COMBO);
    m_ui->comboBoxSupplier->setModel(m_modelSupplier);
    m_ui->comboBoxSupplier->setModelColumn(1);
}

void AccountToPayRegister::GetBankValues(void)
{
    m_modelBank->setQuery(SQL_SELECT_BANK_COMBO);
    m_ui->comboBoxBank->setModel(m_modelBank);
    m_ui->comboBoxBank->setModelColumn(1);
}

void AccountToPayRegister::SendAccountToPayId(int accountToPayId)
{
    AccountToPayModel *accountToPayModel = AccountToPayModel::findByPrimaryKey(accountToPayId);

    m_ui->comboBoxAccountType->setModelColumn(0);

    m_ui->lineEditDesciption->setText(accountToPayModel->getDescription());
    m_ui->dateEditVencDate->setDate(accountToPayModel->getVencDate());
    m_ui->comboBoxAccountType->setCurrentIndex(m_ui->comboBoxAccountType->findText(QString("%1").arg(accountToPayModel->getAccountTypeId())));
    m_ui->lineEditDocNumber->setText(accountToPayModel->getDocNumber());
    m_ui->lineEditValue->setText(QRadMoney::MoneyHumanForm2(accountToPayModel->getValue()));

    if (accountToPayModel->getSupplierId() == 0)
    {
        m_ui->groupBoxSupplier->setChecked(false);
    }
    else
    {
        m_ui->groupBoxSupplier->setChecked(true);
        m_ui->comboBoxSupplier->setModelColumn(0);
        m_ui->comboBoxSupplier->setCurrentIndex(m_ui->comboBoxSupplier->findText(QString("%1").arg(accountToPayModel->getSupplierId())));
        m_ui->comboBoxSupplier->setModelColumn(1);
    }

    if (accountToPayModel->getBankId() == 0)
    {
        m_ui->groupBoxBank->setChecked(false);
    }
    else
    {
        m_ui->groupBoxBank->setChecked(true);
        m_ui->comboBoxBank->setModelColumn(0);
        m_ui->comboBoxBank->setCurrentIndex(m_ui->comboBoxBank->findText(QString("%1").arg(accountToPayModel->getBankId())));
        m_ui->comboBoxBank->setModelColumn(1);
    }

    m_ui->textEditObs->setPlainText(accountToPayModel->getObs());

    m_ui->comboBoxAccountType->setModelColumn(1);

    m_ui->groupBoxInfo->setEnabled(!accountToPayModel->getPaid());
    m_ui->groupBoxSupplier->setEnabled(!accountToPayModel->getPaid());
    m_ui->groupBoxBank->setEnabled(!accountToPayModel->getPaid());
    m_ui->groupBoxObs->setEnabled(!accountToPayModel->getPaid());
    m_ui->btnSave->setEnabled(!accountToPayModel->getPaid());

    m_accountToPayId = accountToPayId;
    delete accountToPayModel;
}

void AccountToPayRegister::SaveAccountToPay(void)
{
    int     max = (m_ui->spinBoxRepeat->value() == 0)?1:m_ui->spinBoxRepeat->value();
    bool    flag = true;
    QString description;
    QDate   vencDate;

    for (int index = 1; index <= max; index++)
    {
        AccountToPayModel   *accountToPayModel = new AccountToPayModel;

        m_accountToPayDescription = m_ui->lineEditDesciption->text();
        m_ui->comboBoxAccountType->setModelColumn(0);

        if (m_ui->groupBoxRepeat->isChecked() && max > 1)
        {
            description = QString("%1 %2/%3").arg(m_ui->lineEditDesciption->text().toUpper()).arg(index).arg(max);
        }
        else
        {
            description = (m_ui->lineEditDesciption->text().isEmpty())?" ":m_ui->lineEditDesciption->text().toUpper();
        }

        accountToPayModel->setDescription(description);
        accountToPayModel->setIssueDate(QDate::currentDate());

        if (m_ui->groupBoxRepeat->isChecked() && index > 1)
        {
            if (m_ui->comboBoxRepeat->currentIndex() == 0)
            {
                vencDate = m_ui->dateEditVencDate->date().addDays((index - 1));
            }
            else if (m_ui->comboBoxRepeat->currentIndex() == 1)
            {
                vencDate = m_ui->dateEditVencDate->date().addDays((index - 1) * 7);
            }
            else if (m_ui->comboBoxRepeat->currentIndex() == 2)
            {
                vencDate = m_ui->dateEditVencDate->date().addMonths(index - 1);
            }
            else
            {
                vencDate = m_ui->dateEditVencDate->date().addYears(index - 1);
            }
        }
        else
        {
            vencDate = m_ui->dateEditVencDate->date();
        }

        accountToPayModel->setVencDate(vencDate);
        accountToPayModel->setAccountTypeId(m_ui->comboBoxAccountType->currentText().toInt());
        accountToPayModel->setDocNumber(m_ui->lineEditDocNumber->text().toUpper());
        accountToPayModel->setValue(QRadMoney::MoneyComputerForm2(m_ui->lineEditValue->text()));

        if (m_ui->groupBoxSupplier->isChecked())
        {
            m_ui->comboBoxSupplier->setModelColumn(0);
            accountToPayModel->setSupplierId(m_ui->comboBoxSupplier->currentText().toInt());
            m_ui->comboBoxSupplier->setModelColumn(1);
        }
        else
        {
            accountToPayModel->setSupplierId(0);
        }

        accountToPayModel->setPaid(false);

        if (m_ui->groupBoxBank->isChecked())
        {
            m_ui->comboBoxBank->setModelColumn(0);
            accountToPayModel->setBankId(m_ui->comboBoxBank->currentText().toInt());
            m_ui->comboBoxBank->setModelColumn(1);
        }
        else
        {
            accountToPayModel->setBankId(0);
        }

        accountToPayModel->setObs(m_ui->textEditObs->toPlainText().toUpper());

        if (m_accountToPayId != 0)
        {
            accountToPayModel->setId(m_accountToPayId);
        }
        if(!accountToPayModel->Save())
        {
            flag = false;
            break;
        }
        m_ui->comboBoxAccountType->setModelColumn(1);
        delete accountToPayModel;
    }

    if (flag)
    {
        QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_RECORD_OK);
        this->accept();
    }
    else
    {
        QMessageBox::critical(this, MSG_ERROR_DATABASE_TITLE, MSG_ERROR_SAVE_ACCOUNT);
        this->reject();
    }
}

QString AccountToPayRegister::GetAccountToPayDescription(void)
{
    return m_accountToPayDescription;
}

void AccountToPayRegister::changeFocus()
{
    m_ui->btnSave->setFocus();
}
