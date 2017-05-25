#include "enterbillet.h"
#include "ui_enterbillet.h"

#define SQL_SELECTLOT_QUERY                 "select l.id, l.lotenumber, l.validitydate, (l.quantity - l.reservestock) as quantity from dsm_lotecontrol l, dsm_product p where l.productid = '%1' and l.quantity > 0 and p.id = l.productid order by l.validitydate"
#define SQL_SELECT_PRODUCT                  "select * from dsm_product where id = '%1'"
#define SQL_SELECT_PRODUCT_ITEMS_ENTRANCE   "select i.id, i.productid, p.descricao from %1 i inner join dsm_product p on productid = p.id and p.controlarvalidade = 'true' order by i.id"

#define COL_CURRENT_ID          0
#define COL_PRODUCT_ID          1
#define COL_PRODUCT_DESCRIPTION 2
#define COL_PRODUCT_AMOUNT      3
#define COL_PRODUCT_PRICE       4

EnterBillet::EnterBillet(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::EnterBillet)
{
    m_ui->setupUi(this);

    m_entranceId = 0;

    ConfigureHeaderTableLotsIndicated();

    connect(m_ui->pushButtonAdd, SIGNAL(clicked()), this, SLOT(AddBillet()));
}

EnterBillet::~EnterBillet()
{
    delete m_ui;
}

void EnterBillet::changeEvent(QEvent *e)
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

void EnterBillet::showEvent(QShowEvent*)
{
    m_billetList.clear();
    UpdateTable();
    m_ui->dateEditBilletVenc->setDate(QDate::currentDate());
    m_ui->radioButtonAVista->setChecked(true);
    m_ui->spinBoxParcelas->setFocus();
}

void EnterBillet::closeEvent(QCloseEvent*)
{
    ClearTable();
    m_billetList.clear();
}

void EnterBillet::ConfigureHeaderTableLotsIndicated(void)
{
    m_ui->tableWidgetBilletsIndicated->setColumnCount(2);
    m_ui->tableWidgetBilletsIndicated->setHorizontalHeaderItem(0, new QTableWidgetItem(QString::fromUtf8("Data Vencimento")));
    m_ui->tableWidgetBilletsIndicated->setHorizontalHeaderItem(1, new QTableWidgetItem(QString::fromUtf8("Valor")));

    m_ui->tableWidgetBilletsIndicated->setItemDelegateForColumn(0, new ColumnCenter);
    m_ui->tableWidgetBilletsIndicated->setItemDelegateForColumn(1, new ColumnRight);

    m_ui->tableWidgetBilletsIndicated->setColumnWidth(0, 140);
    m_ui->tableWidgetBilletsIndicated->horizontalHeader()->setStretchLastSection(true);
}

void EnterBillet::keyPressEvent(QKeyEvent* event)
{
    switch ( event->key() )
    {
        case Qt::Key_F1:
        {
            if(dsmRound::PowerRound(m_totalPaid) < dsmRound::PowerRound(m_entranceTotalValue))
            {
                QMessageBox::information(this, "Aviso!", QString::fromUtf8("Os valores indicados ao boleto não conferem com o valor total da nota.\nResta R$ %1").arg(DSMMoney::MoneyHumanForm2(dsmRound::PowerRound(m_entranceTotalValue - m_totalPaid))));
                return;
            }

            if (m_billetList.count() > 0)
            {
                DSM_Billet_t billetItemList;
                int     index = 1;
                bool    flag = true;

                foreach (billetItemList, m_billetList)
                {
                    AccountToPayModel *accountToPayModel = new AccountToPayModel;

                    accountToPayModel->setDescription(QString::fromUtf8("BOLETO %2/%3 NF Nº %1 DE %4")
                                                                .arg(m_ui->lineEditNumber->text())
                                                                .arg(index)
                                                                .arg(m_billetList.count())
                                                                .arg(m_entranceDate.toString(FMT_DATE)));
                    accountToPayModel->setIssueDate(QDate::currentDate());
                    accountToPayModel->setVencDate(billetItemList.venc);
                    accountToPayModel->setAccountTypeId(AccountTypeDefaultBillet);
                    accountToPayModel->setPaid((m_ui->radioButtonAVista->isChecked()));
                    accountToPayModel->setValue(billetItemList.value);
                    if (m_ui->radioButtonAVista->isChecked())
                    {
                        accountToPayModel->setValuePaid(billetItemList.value);
                        accountToPayModel->setPaidDate(QDate::currentDate());
                    }
                    accountToPayModel->setSupplierId(m_supplierId);
                    accountToPayModel->setBankId(0);
                    accountToPayModel->setObs(QString::fromUtf8("BOLETO REFERENTE AO PAGAMENTO DA NOTA FISCAL Nº %1.")
                                              .arg(m_ui->lineEditNumber->text()));
                    if (!accountToPayModel->Create())
                    {
                        flag = false;
                        break;
                    }
                    else
                    {
                        AccountToPayModel *accountToPayModel = new AccountToPayModel;
                        AccountToPayHistoryModel *accountToPayHistoryModel = new AccountToPayHistoryModel;

                        accountToPayHistoryModel->setAccountToPayId(accountToPayModel->lastInsertId());
                        accountToPayHistoryModel->setTypeOperation(AccountOpCreate);
                        accountToPayHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
                        accountToPayHistoryModel->setDate(QDate::currentDate());
                        accountToPayHistoryModel->setTime(QTime::currentTime());

                        accountToPayHistoryModel->Create();

                        delete accountToPayModel;
                        delete accountToPayHistoryModel;
                    }

                    delete accountToPayModel;

                    index++;
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
        }
        break;

        case Qt::Key_F8:
        {
            RemoveLastItem();
        }
        break;

        case Qt::Key_Escape:
        {
            this->close();
        }
        break;

        default:
        {
            QDialog::keyPressEvent( event );
        }
    }
}

void EnterBillet::UpdateTable(void)
{
    disconnect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));

    int row;
    QTableWidgetItem    *pitem1;
    QTableWidgetItem    *pItem2;

    ClearTable();

    m_totalPaid = 0;

    foreach (DSM_Billet_t billetItemList, m_billetList)
    {
        pitem1 = new QTableWidgetItem(billetItemList.venc.toString(FMT_DATE));
        pItem2 = new QTableWidgetItem(DSMMoney::MoneyHumanForm2(billetItemList.value));

        row = m_ui->tableWidgetBilletsIndicated->rowCount();
        m_ui->tableWidgetBilletsIndicated->insertRow(row);
        m_ui->tableWidgetBilletsIndicated->setItem(row, 0, pitem1);
        m_ui->tableWidgetBilletsIndicated->setItem(row, 1, pItem2);

        m_totalPaid += billetItemList.value;
    }

    ClearFields();

    connect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
}

void EnterBillet::ClearTable(void)
{
    int rowCount;

    rowCount = m_ui->tableWidgetBilletsIndicated->rowCount();

    for (int i = rowCount; --i >= 0;)
        m_ui->tableWidgetBilletsIndicated->removeRow(i);
}

void EnterBillet::ClearFields(void)
{
    QRadConfig::GoTo(m_ui->dateEditBilletVenc);
    m_ui->spinBoxParcelas->setValue(1);
}

void EnterBillet::RemoveLastItem(void)
{
    if (m_billetList.count() > 0)
    {
        m_billetList.pop_back();
    }

    UpdateTable();
}

double EnterBillet::GetTotalAmount(DSM_Lot_t lotList)
{
    double totalAmount = 0;

    foreach(DSM_LotItems_t lotItemList, lotList.items)
    {
        totalAmount += lotItemList.amount;
    }
    return totalAmount;
}

void EnterBillet::GetEntranceId(int entranceId)
{
    m_entranceId = entranceId;
    EntranceModel *entranceModel = EntranceModel::findById(m_entranceId);

    m_ui->lineEditNumber->setText(entranceModel->getTaxDocNumber());
    m_ui->lineEditTotalValue->setText(QString("R$ %1").arg(DSMMoney::MoneyHumanForm2(entranceModel->getTotalValue())));

    m_entranceTotalValue = dsmRound::PowerRound(entranceModel->getTotalValue());
    m_entranceDate = entranceModel->getTaxDocDate();

    m_supplierId = entranceModel->getSupplierId();

    delete entranceModel;
}

void EnterBillet::AddBillet(void)
{
    disconnect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));

    DSM_Billet_t billetItemList;
    bool         flag = true;
    double      totalList = 0;

    foreach (billetItemList, m_billetList)
    {
        if ( billetItemList.venc == m_ui->dateEditBilletVenc->date() )
        {
            QMessageBox::information(this, MSG_INFO_TITLE, MSG_BILLET_ALREADY_EXIST);
            flag = false;
        }

        totalList += dsmRound::PowerRound(billetItemList.value);

    }

    if(dsmRound::PowerRound(totalList) >= dsmRound::PowerRound(m_entranceTotalValue))
    {
        QMessageBox::information(this, MSG_INFO_TITLE, QString::fromUtf8("O valor a ser associado, irá ultrapassar o valor total do boleto."));
        flag = false;
    }

    if (flag)
    {
        int portions = m_ui->spinBoxParcelas->value();
        int count = m_billetList.count();

        for (int index = 0 + count; index < portions + count; index++)
        {
            billetItemList.venc  = m_ui->dateEditBilletVenc->date();
            m_ui->dateEditBilletVenc->setDate(m_ui->dateEditBilletVenc->date().addDays(m_ui->spinBoxParcelasDays->value()));
            billetItemList.value = 0;

            m_billetList.append(billetItemList);

            m_billetList[index].value = dsmRound::PowerRound( dsmRound::PowerRound(m_entranceTotalValue - totalList) / portions);
            m_totalPaid += dsmRound::PowerRound( dsmRound::PowerRound(m_entranceTotalValue - totalList) / portions);
        }

        if(dsmRound::PowerRound(m_totalPaid) < dsmRound::PowerRound(m_entranceTotalValue))
        {
            m_billetList.last().value += dsmRound::PowerRound(m_entranceTotalValue - m_totalPaid);
            m_totalPaid += dsmRound::PowerRound(m_entranceTotalValue - m_totalPaid);
        }
        else if(dsmRound::PowerRound(m_totalPaid) > dsmRound::PowerRound(m_entranceTotalValue))
        {
            m_billetList.last().value -= dsmRound::PowerRound(m_totalPaid - m_entranceTotalValue);
            m_totalPaid -= dsmRound::PowerRound(m_totalPaid - m_entranceTotalValue);
        }
    }

    UpdateTable();

    connect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
}

void EnterBillet::changeData(QModelIndex indexA, QModelIndex indexB)
{
    if(!indexA.isValid())
        return;

    Q_UNUSED(indexB);

    if(indexA.column() == 1)
    {
        if (m_billetList.count() > 0)
        {
            double newValue = DSMMoney::MoneyComputerForm2(m_ui->tableWidgetBilletsIndicated->item(indexA.row(), 1)->data(Qt::EditRole).toString());
            double total = 0;

            DSM_Billet_t billetItemList;

            foreach (billetItemList, m_billetList)
            {
                total += dsmRound::PowerRound(billetItemList.value);
            }

            double oldValue = dsmRound::PowerRound(m_billetList.at(indexA.row()).value);

            total -= oldValue;

            double newTotal = total + newValue;

            if(dsmRound::PowerRound(newTotal) > dsmRound::PowerRound(m_entranceTotalValue))
            {
                QMessageBox::information(this, "Aviso!", QString::fromUtf8("O valor desejado ultrapassa o valor total da nota"));
                disconnect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
                m_ui->tableWidgetBilletsIndicated->item(indexA.row(),1)->setText(DSMMoney::MoneyHumanForm2(oldValue));
                connect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
                return;
            }

            m_totalPaid = newTotal;

            m_billetList[indexA.row()].value = newValue;
        }
    }
    else if(indexA.column() == 0)
    {
        if (m_billetList.count() > 0)
        {
            QDate oldDate = m_billetList[indexA.row()].venc;

            DSM_Billet_t billetItemList;

            foreach (billetItemList, m_billetList)
            {
                if(!QDate::fromString(m_ui->tableWidgetBilletsIndicated->item(indexA.row(), 0)->data(Qt::EditRole).toString(),FMT_DATE).isValid())
                {
                    disconnect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
                    m_ui->tableWidgetBilletsIndicated->item(indexA.row(),0)->setText(oldDate.toString(FMT_DATE));
                    connect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
                    return;
                }
                if ( billetItemList.venc == QDate::fromString(m_ui->tableWidgetBilletsIndicated->item(indexA.row(), 0)->data(Qt::EditRole).toString(),FMT_DATE))
                {
                    QMessageBox::information(this, MSG_INFO_TITLE, MSG_BILLET_ALREADY_EXIST);
                    disconnect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
                    m_ui->tableWidgetBilletsIndicated->item(indexA.row(),0)->setText(oldDate.toString(FMT_DATE));
                    connect(m_ui->tableWidgetBilletsIndicated->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(changeData(QModelIndex,QModelIndex)));
                    return;
                }
            }

            m_billetList[indexA.row()].venc = QDate::fromString(m_ui->tableWidgetBilletsIndicated->item(indexA.row(), 0)->data(Qt::EditRole).toString(),FMT_DATE);
        }
    }
}
