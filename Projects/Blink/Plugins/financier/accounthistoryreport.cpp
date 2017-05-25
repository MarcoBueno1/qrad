#include "accounthistoryreport.h"
#ifdef _OLD_REPORT_
#include "dsmreport.h"
#endif
#include "column2delegate.h"


#define SQL_SELECT_ACCOUNTTOPAY     "select ah.id, a.id, ah.userid, a.description, 'A PAGAR' as accounttype, ah.typeoperation, u.name, ah.date, ah.time  from dsm_fin_accounttopay_history ah inner join dsm_fin_accounttopay a on ah.accounttopayid = a.id inner join dsm_user u on ah.userid = u.id "
#define SQL_SELECT_ACCOUNTTORECEIVE "select ah.id, a.id, ah.userid, a.description, 'A RECEBER' as accounttype, ah.typeoperation, u.name, ah.date, ah.time  from dsm_fin_accounttoreceive_history ah inner join dsm_fin_accounttoreceive a on ah.accounttoreceiveid = a.id inner join dsm_user u on ah.userid = u.id "
#define SQL_SELECT_ACCOUNTS         "select * from( %1 ) as teste %2 %3 %4 order by date desc, time desc, description"

AccountHistoryReport::AccountHistoryReport(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountHistoryReport)
{
    m_ui->setupUi(this);

    m_accountModel = new QSqlQueryModel;
    m_userModel    = new QSqlQueryModel;

    GetUserValues();

    m_ui->dateEditSince->setDate( QDate::currentDate().addMonths(-1) );
    m_ui->dateEditUntil->setDate( QDate::currentDate() );

    InitialConfig();

    connect(m_ui->groupBoxPeriod, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->dateEditSince, SIGNAL(dateChanged(QDate)), this, SLOT(InitialConfig()));
    connect(m_ui->dateEditUntil, SIGNAL(dateChanged(QDate)), this, SLOT(InitialConfig()));
    connect(m_ui->checkBoxToPay, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->checkBoxToReceive, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->checkBoxCreate, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->checkBoxEdit, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->checkBoxRemove, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->checkBoxPaid, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->checkBoxRestore, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->groupBoxUser, SIGNAL(clicked()), this, SLOT(InitialConfig()));
    connect(m_ui->comboBoxUser, SIGNAL(currentIndexChanged(int)), this, SLOT(InitialConfig()));
}

AccountHistoryReport::~AccountHistoryReport()
{
    delete m_accountModel;
    delete m_userModel;
    delete m_ui;
}

void AccountHistoryReport::changeEvent(QEvent *e)
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

void AccountHistoryReport::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    m_ui->dateEditSince->setFocus();
    InitialConfig();
}

void AccountHistoryReport::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_F1:
        {
            QModelIndex modelIndex;
            modelIndex = m_ui->tableViewAccountHistory->currentIndex();

            AccountHistoryReportAux *accountHistoryReportAux = new AccountHistoryReportAux(this);
            accountHistoryReportAux->SendAccountId(modelIndex.sibling(modelIndex.row(),1).data().toInt(),
                                                   (modelIndex.sibling(modelIndex.row(),4).data().toString() == "A PAGAR")?AccountTypeToPay:AccountTypeToReceive);
            dsmConfig::centralizarWidget(accountHistoryReportAux);
            accountHistoryReportAux->exec();
            delete accountHistoryReportAux;
        }
        break;
        case Qt::Key_Escape:
        {
            this->close();
        }
        break;
        default:
        {
            QWidget::keyPressEvent(e);
        }
    }//switch
}

void AccountHistoryReport::InitialConfig(int row)
{
    QString strAux = "",
            strDate = "",
            strOp = "",
            strUser = "";

    if (m_ui->checkBoxToPay->isChecked() && m_ui->checkBoxToReceive->isChecked())
    {
        strAux = QString::fromUtf8("%1 union %2").arg(SQL_SELECT_ACCOUNTTOPAY).arg(SQL_SELECT_ACCOUNTTORECEIVE);
    }
    else if (m_ui->checkBoxToPay->isChecked())
    {
        strAux = QString::fromUtf8("%1").arg(SQL_SELECT_ACCOUNTTOPAY);
    }
    else if (m_ui->checkBoxToReceive->isChecked())
    {
        strAux = QString::fromUtf8("%1").arg(SQL_SELECT_ACCOUNTTORECEIVE);
    }

    if (m_ui->checkBoxCreate->isChecked())
    {
        strOp = QString(" where (typeoperation = %1").arg(AccountOpCreate);
    }
    if (m_ui->checkBoxEdit->isChecked())
    {
        if (strOp.isEmpty())
        {
            strOp = QString(" where (typeoperation = %1").arg(AccountOpEdit);
        }
        else
        {
            strOp += QString(" or typeoperation = %1").arg(AccountOpEdit);
        }
    }
    if (m_ui->checkBoxRemove->isChecked())
    {
        if (strOp.isEmpty())
        {
            strOp = QString(" where (typeoperation = %1").arg(AccountOpRemove);
        }
        else
        {
            strOp += QString(" or typeoperation = %1").arg(AccountOpRemove);
        }
    }
    if (m_ui->checkBoxPaid->isChecked())
    {
        if (strOp.isEmpty())
        {
            strOp = QString(" where (typeoperation = %1").arg(AccountOpPaid);
        }
        else
        {
            strOp += QString(" or typeoperation = %1").arg(AccountOpPaid);
        }
    }
    if (m_ui->checkBoxRestore->isChecked())
    {
        if (strOp.isEmpty())
        {
            strOp = QString(" where (typeoperation = %1").arg(AccountOpRestore);
        }
        else
        {
            strOp += QString(" or typeoperation = %1").arg(AccountOpRestore);
        }
    }
    if (strOp.isEmpty())
    {
        /** apenas para zerar o select */
        strOp += QString(" and m.reason = 99 ");
    }
    else
    {
        strOp += QString(")");
    }

    if (m_ui->groupBoxPeriod->isChecked())
    {
        strDate = QString(" and date between '%1' and '%2'")
                            .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
                            .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB));
    }

    if (m_ui->groupBoxUser->isChecked())
    {
        int userId;

        disconnect(m_ui->comboBoxUser, SIGNAL(currentIndexChanged(int)), this, SLOT(InitialConfig()));

        GET_COMBOBOX_ID(userId, m_ui->comboBoxUser);
        strUser = QString(" and userid = %1 ").arg(userId);

        connect(m_ui->comboBoxUser, SIGNAL(currentIndexChanged(int)), this, SLOT(InitialConfig()));
    }


    m_accountModel->setQuery(QString::fromUtf8(SQL_SELECT_ACCOUNTS)
                                            .arg(strAux)
                                            .arg(strOp)
                                            .arg(strDate)
                                            .arg(strUser));
    m_ui->tableViewAccountHistory->setModel(m_accountModel);
    m_ui->tableViewAccountHistory->selectRow(0);

    ConfigHeaderTable();

    QModelIndex currentIndex = m_ui->tableViewAccountHistory->currentIndex();
    m_ui->tableViewAccountHistory->setCurrentIndex(m_accountModel->index(currentIndex.row(),2));

    m_ui->tableViewAccountHistory->selectRow(row);
}

void AccountHistoryReport::ConfigHeaderTable(void)
{
    m_ui->tableViewAccountHistory->hideColumn(0);
    m_ui->tableViewAccountHistory->hideColumn(1);
    m_ui->tableViewAccountHistory->hideColumn(2);

    m_accountModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Descrição"));
    m_accountModel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Tipo de\nConta"));
    m_accountModel->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Tipo de\nOperação"));
    m_accountModel->setHeaderData(6, Qt::Horizontal, QString::fromUtf8("Responsável"));
    m_accountModel->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Data"));
    m_accountModel->setHeaderData(8, Qt::Horizontal, QString::fromUtf8("Hora"));

    m_ui->tableViewAccountHistory->setItemDelegateForColumn(3, new ColumnObs);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(4, new ColumnCenter);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(5, new ColumnFinancierAccountOperation);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(6, new ColumnObs);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(7, new ColumnDate);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(8, new ColumnCenter);

    m_ui->tableViewAccountHistory->setColumnWidth(3, 260);
    m_ui->tableViewAccountHistory->setColumnWidth(4, 90);
    m_ui->tableViewAccountHistory->setColumnWidth(5, 90);
    m_ui->tableViewAccountHistory->setColumnWidth(6, 160);
    m_ui->tableViewAccountHistory->setColumnWidth(7, 80);
    m_ui->tableViewAccountHistory->setColumnWidth(8, 75);
}

void AccountHistoryReport::GetUserValues(void)
{
    m_userModel->setQuery(QString(SQL_SELECT_USER_COMBO));
    m_ui->comboBoxUser->setModel(m_userModel);
    m_ui->comboBoxUser->setModelColumn(1);
}

void AccountHistoryReport::Launcher(void)
{
#if 0
    QSqlQueryModel  *getSum = new QSqlQueryModel,
                    *getTotal = new QSqlQueryModel;

    DSMReport       *report = new DSMReport();
    int             commissionTotalAmount = 0;
    float           total = 0, commissionTotal = 0, commissionTotalPorc = 0;
    QString         strSeller, strSellerHeader;

    if ( !report->load( "sell_commissions" ) )
    {
       QMessageBox::critical( this, "Erro", "Falha ao carregar arquivo modelo." );
       delete report;
       return;
    }

    getTotal->setQuery(QString(SQL_SELL_COMMISSIONS)
                       .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
                       .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB))
                       .arg(total));

    for (int index = 0; index < getTotal->rowCount(); index++)
    {
        commissionTotalAmount    += getTotal->record(index).value("quantity").toInt();
        commissionTotal          += getTotal->record(index).value("total").toDouble();
        commissionTotalPorc      += getTotal->record(index).value("porctotal").toDouble();
    }

    report->setQuery("commission", QString(SQL_SELL_COMMISSIONS)
                                .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
                                .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB))
                                .arg(total));

    report->setAttributeValue("START_DATE", m_ui->dateEditSince->date().toString(FMT_DATE));
    report->setAttributeValue("END_DATE", m_ui->dateEditUntil->date().toString(FMT_DATE));
    report->setAttributeValue("TOTAL_AMOUNT", commissionTotalAmount);
    report->setAttributeValue("TOTAL_TOTAL", total);
    report->setAttributeValue("TOTAL_PORC_TOTAL", commissionTotalPorc);

    if ( !report->show() )
    {
        QMessageBox::critical( this, windowTitle(), QString::fromUtf8( "Não foi possível exibir o relatório." ) );
    }

    delete report;
    delete getSum;
    delete getTotal;
#endif
}
