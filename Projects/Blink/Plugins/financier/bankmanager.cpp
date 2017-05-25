#include "bankmanager.h"
#include "ui_bankmanager.h"

#define SQL_SELECT_BANK  "select id, code, description from dsm_fin_bank where removed = false order by description"
#define SQL_DELETE_BANK  "update dsm_fin_bank set removed = true where id = (%1)"

BankManager::BankManager(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::BankManager)
{
    m_ui->setupUi(this);

    m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    m_selectBank = new QSqlQueryModel;

    InitialConfig();

    connect(m_ui->lineEditSearch, SIGNAL(textChanged(QString)), this, SLOT(SearchBank(QString)));
    connect(m_ui->btnNew, SIGNAL(pressed()), this, SLOT(NewBank()));
    connect(m_ui->btnEdit, SIGNAL(pressed()), this, SLOT(EditBank()));
    connect(m_ui->btnDel, SIGNAL(pressed()), this, SLOT(DeleteBank()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(close()));
    connect(m_ui->tableViewBank,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(m_ui->tableViewBank->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(fillTheFields(QModelIndex)));

}

BankManager::~BankManager()
{
    delete m_selectBank;
    delete m_ui;
}

void BankManager::changeEvent(QEvent *e)
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

void BankManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    InitialConfig();
}

void BankManager::GetBank(void)
{
    m_selectBank->setQuery(SQL_SELECT_BANK);

    m_ui->tableViewBank->setModel(m_selectBank);
    m_ui->tableViewBank->show();
    m_ui->tableViewBank->selectRow(0);
}

void BankManager::InitialConfig(QString banckName)
{
    GetBank();
    QModelIndex currentIndex = m_ui->tableViewBank->currentIndex();

    ConfigHeaderTable();
    m_ui->tableViewBank->setCurrentIndex(m_selectBank->index(0,2));

    if (!banckName.isEmpty())
        m_ui->tableViewBank->keyboardSearch(banckName);

    else
        m_ui->tableViewBank->setCurrentIndex(m_selectBank->index(currentIndex.row(),2));

    m_ui->lineEditSearch->setFocus();
    ShowCurrentInformations();
}

void BankManager::ConfigHeaderTable(void)
{
    m_ui->tableViewBank->hideColumn(0);

    m_selectBank->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Código"));
    m_selectBank->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Descrição"));

    m_ui->tableViewBank->setItemDelegateForColumn(1, new ColumnCenter);
    m_ui->tableViewBank->setItemDelegateForColumn(2, new ColumnObs);

    m_ui->tableViewBank->setColumnWidth(1, 80);
    m_ui->tableViewBank->setColumnWidth(2, 300);
    m_ui->tableViewBank->horizontalHeader()->setStretchLastSection(true);
}

void BankManager::fillTheFields(QModelIndex currentIndex)
{
    m_bankId = currentIndex.sibling(currentIndex.row(),0).data().toInt();
}

void BankManager::keyPressEvent(QKeyEvent *event)
{
    int index;
    switch(event->key())
    {
        case Qt::Key_PageUp:
        case Qt::Key_Up:
        {
            if (m_ui->tableViewBank->currentIndex().row() > 0)
            {
                QCoreApplication::sendEvent(m_ui->tableViewBank, event);
                index = m_ui->tableViewBank->currentIndex().row();
                m_ui->lineEditSearch->setText(m_selectBank->record(index).value("description").toString());
                m_ui->lineEditSearch->selectAll();
                m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
                ShowCurrentInformations();
            }
            break;
        }

        case Qt::Key_PageDown:
        case Qt::Key_Down:
        {
            if ( (m_ui->tableViewBank->currentIndex().row() + 1) < m_selectBank->rowCount())
            {
                QCoreApplication::sendEvent(m_ui->tableViewBank, event);
                index = m_ui->tableViewBank->currentIndex().row();
                m_ui->lineEditSearch->setText(m_selectBank->record(index).value("description").toString());
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

void BankManager::SearchBank(QString text)
{
    int index;

    if ( text.isEmpty() )
        return;

    m_ui->tableViewBank->keyboardSearch(text);

    index = m_ui->tableViewBank->currentIndex().row();

    if (m_selectBank->record(index).value("description").toString().startsWith(m_ui->lineEditSearch->text(),Qt::CaseInsensitive))
    {
        m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
    else
    {
        m_ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
    }
}

void BankManager::NewBank(void)
{
    BankRegister *bankRegister = new BankRegister(this);
    QRadConfig::centralizarWidget(bankRegister);

    bankRegister->exec();
    InitialConfig(bankRegister->GetBankName());

    delete bankRegister;
}

void BankManager::EditBank(void)
{
    BankRegister *bankRegister = new BankRegister(this);
    QRadConfig::centralizarWidget(bankRegister);

    bankRegister->SendBankId(m_bankId);

    bankRegister->exec();
    InitialConfig(bankRegister->GetBankName());

    delete bankRegister;
}

void BankManager::DeleteBank(void)
{
    QModelIndex index = m_ui->tableViewBank->currentIndex();

    if (QMessageBox::question(this, MSG_QUESTION_TITLE, MSG_QUESTION_DELETE_BANK, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
    {
        QSqlQuery   *deleteProfile = new QSqlQuery;
        if (!deleteProfile->exec(QString(SQL_DELETE_BANK).arg(m_bankId)))
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
    {
        m_ui->tableViewBank->setCurrentIndex(m_selectBank->index(index.row(),2));
    }
    else
    {
        m_ui->tableViewBank->setCurrentIndex(m_selectBank->index(index.row()-1,2));
        m_ui->lineEditSearch->setText(m_selectBank->record(index.row()-1).value("description").toString());
    }
}

void BankManager::TableClicked(QModelIndex currentIndex)
{
    m_ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),2).data().toString());
    m_ui->lineEditSearch->selectAll();
    ShowCurrentInformations();
}

void BankManager::ShowCurrentInformations(void)
{
    QString strTemp;

    if( m_selectBank->rowCount()  )
    {
     strTemp.sprintf("Contas ( %d de %d )", m_ui->tableViewBank->currentIndex().row() + 1, m_selectBank->rowCount());
     m_ui->groupBox_2->setTitle(strTemp);
    }
}
