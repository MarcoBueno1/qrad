#include "accounttypemanager.h"
#include "ui_accounttypemanager.h"

#define SQL_SELECT_ACCOUNTTYPE  "select id, description, type from dsm_fin_accounttype where removed = false order by description"
#define SQL_DELETE_ACCOUNTTYPE  "update dsm_fin_accounttype set removed = true where id = (%1)"

AccountTypeManager::AccountTypeManager(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountTypeManager)
{
    m_ui->setupUi(this);

    m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    m_selectAccountType = new QSqlQueryModel;

    InitialConfig();

    connect(m_ui->lineEditSearch, SIGNAL(textChanged(QString)), this, SLOT(SearchAccountType(QString)));
    connect(m_ui->btnNew, SIGNAL(pressed()), this, SLOT(NewAccountType()));
    connect(m_ui->btnEdit, SIGNAL(pressed()), this, SLOT(EditAccountType()));
    connect(m_ui->btnDel, SIGNAL(pressed()), this, SLOT(DeleteAccountType()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(close()));
    connect(m_ui->tableViewAccountType,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(m_ui->tableViewAccountType->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(fillTheFields(QModelIndex)));

}

AccountTypeManager::~AccountTypeManager()
{
    delete m_selectAccountType;
    delete m_ui;
}

void AccountTypeManager::changeEvent(QEvent *e)
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

void AccountTypeManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    InitialConfig();
}

void AccountTypeManager::GetAccountType(void)
{
    m_selectAccountType->setQuery(SQL_SELECT_ACCOUNTTYPE);

    m_ui->tableViewAccountType->setModel(m_selectAccountType);
    m_ui->tableViewAccountType->show();
    m_ui->tableViewAccountType->selectRow(0);
}

void AccountTypeManager::InitialConfig(QString groupName)
{
    GetAccountType();
    QModelIndex currentIndex = m_ui->tableViewAccountType->currentIndex();

    ConfigHeaderTable();
    m_ui->tableViewAccountType->setCurrentIndex(m_selectAccountType->index(0,1));

    if (!groupName.isEmpty())
        m_ui->tableViewAccountType->keyboardSearch(groupName);

    else
        m_ui->tableViewAccountType->setCurrentIndex(m_selectAccountType->index(currentIndex.row(),1));

    m_ui->lineEditSearch->setFocus();
    ShowCurrentInformations();
}

void AccountTypeManager::ConfigHeaderTable(void)
{
    m_ui->tableViewAccountType->hideColumn(0);

    m_selectAccountType->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Nome da Conta"));
    m_selectAccountType->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Tipo"));

    m_ui->tableViewAccountType->setItemDelegateForColumn(1, new ColumnObs );
    m_ui->tableViewAccountType->setItemDelegateForColumn(2, new ColumnAccountType);

    m_ui->tableViewAccountType->setColumnWidth(1, 330);
    m_ui->tableViewAccountType->horizontalHeader()->setStretchLastSection(true);
}

void AccountTypeManager::fillTheFields(QModelIndex currentIndex)
{
    m_accountTypeId = currentIndex.sibling(currentIndex.row(),0).data().toInt();
}

void AccountTypeManager::keyPressEvent(QKeyEvent *event)
{
    int index;
    switch(event->key())
    {
        case Qt::Key_PageUp:
        case Qt::Key_Up:
        {
            if (m_ui->tableViewAccountType->currentIndex().row() > 0)
            {
                QCoreApplication::sendEvent(m_ui->tableViewAccountType, event);
                index = m_ui->tableViewAccountType->currentIndex().row();
                m_ui->lineEditSearch->setText(m_selectAccountType->record(index).value("description").toString());
                m_ui->lineEditSearch->selectAll();
                m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
                ShowCurrentInformations();
            }
            break;
        }

        case Qt::Key_PageDown:
        case Qt::Key_Down:
        {
            if ( (m_ui->tableViewAccountType->currentIndex().row() + 1) < m_selectAccountType->rowCount())
            {
                QCoreApplication::sendEvent(m_ui->tableViewAccountType, event);
                index = m_ui->tableViewAccountType->currentIndex().row();
                m_ui->lineEditSearch->setText(m_selectAccountType->record(index).value("description").toString());
                m_ui->lineEditSearch->selectAll();
                m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
                ShowCurrentInformations();
            }
            break;
        }
        case Qt::Key_Escape:
        {
            this->close();
            break;
        }

        default:
        QWidget::keyPressEvent( event );
        break;
    }
}

void AccountTypeManager::SearchAccountType(QString text)
{
    int index;

    if ( text.isEmpty() )
        return;

    m_ui->tableViewAccountType->keyboardSearch(text);

    index = m_ui->tableViewAccountType->currentIndex().row();

    if (m_selectAccountType->record(index).value("description").toString().startsWith(m_ui->lineEditSearch->text(),Qt::CaseInsensitive))
    {
        m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
    else
    {
        m_ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
    }
}

void AccountTypeManager::NewAccountType(void)
{
    AccountTypeRegister *accountTypeRegister = new AccountTypeRegister(this);
    dsmConfig::centralizarWidget(accountTypeRegister);

    accountTypeRegister->exec();
    InitialConfig(accountTypeRegister->GetAccountTypeName());

    delete accountTypeRegister;
}

void AccountTypeManager::EditAccountType(void)
{
    if (m_accountTypeId > 6)
    {
        AccountTypeRegister *accountTypeRegister = new AccountTypeRegister(this);
        dsmConfig::centralizarWidget(accountTypeRegister);

        accountTypeRegister->SendAccountTypeId(m_accountTypeId);

        accountTypeRegister->exec();
        InitialConfig(accountTypeRegister->GetAccountTypeName());

        delete accountTypeRegister;
    }
    else
    {
        QMessageBox::information(this, MSG_INFO_TITLE , MSG_INFO_CANNOT_EDIT_ACCOUNTTYPE);
    }
}

void AccountTypeManager::DeleteAccountType(void)
{
    if (m_accountTypeId > 6)
    {
        QModelIndex index = m_ui->tableViewAccountType->currentIndex();

        if (QMessageBox::question(this, MSG_QUESTION_TITLE, MSG_QUESTION_DELETE_ACCOUNTTYPE, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            QSqlQuery   *deleteProfile = new QSqlQuery;
            if (!deleteProfile->exec(QString(SQL_DELETE_ACCOUNTTYPE).arg(m_accountTypeId)))
            {
                QMessageBox::critical(this, MSG_ERROR_DATABASE_TITLE, MSG_ERROR_DATABASE);
            }
            else
            {
                QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_DELETE_OK);
                InitialConfig();
            }
            delete deleteProfile;
        }

        if( index.row() == 0 )
            m_ui->tableViewAccountType->setCurrentIndex(m_selectAccountType->index(index.row(),1));
        else
        {
            m_ui->tableViewAccountType->setCurrentIndex(m_selectAccountType->index(index.row()-1,1));
            m_ui->lineEditSearch->setText(m_selectAccountType->record(index.row()-1).value("description").toString());
        }
    }
    else
    {
        QMessageBox::information(this, MSG_INFO_TITLE , MSG_INFO_CANNOT_DELETE_ACCOUNTTYPE);
    }
}

void AccountTypeManager::TableClicked(QModelIndex currentIndex)
{
    m_ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),1).data().toString());
    m_ui->lineEditSearch->selectAll();
    ShowCurrentInformations();
}

void AccountTypeManager::ShowCurrentInformations(void)
{
    QString strTemp;

    if( m_selectAccountType->rowCount()  )
    {
     strTemp.sprintf("Contas ( %d de %d )", m_ui->tableViewAccountType->currentIndex().row() + 1, m_selectAccountType->rowCount());
     m_ui->groupBox_2->setTitle(strTemp);
    }
}
