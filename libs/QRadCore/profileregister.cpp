#include "profileregister.h"
#include "ui_profileregister.h"
#include <QSqlRecord>
#include <QSqlField>
#include <QMessageBox>
#include "qradshared.h"
#include "qradmessages.h"

#define SQL_SELECT_PROFILES "select * from profile where id = %1"

ProfileRegister::ProfileRegister(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ProfileRegister)
{
    m_ui->setupUi(this);

    m_profileId = 0;
    m_profileName.clear();
    m_ui->tabWidget->setCurrentIndex(0);
    m_ui->lineEditProfile->setFocus();

    ConfigDsmType();

    connect(m_ui->btnSave, SIGNAL(pressed()), this, SLOT(SaveProfile()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(close()));

    /** Produtos */
    connect(m_ui->checkBoxProducts, SIGNAL(clicked(bool)), this, SLOT(OnClickedProducts(bool)));
    connect(m_ui->checkBoxProductRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxAbcFarmaRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxOrderRegistrationRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxTransfRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxMovementRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxChangeGroupsRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxLotManagerRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxProductBalanceRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxSngpcRegister, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxOffer, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));
    connect(m_ui->checkBoxCalcDare, SIGNAL(clicked()), this, SLOT(OnClickedProduct()));

    /** Clientes */
    connect(m_ui->checkBoxClients, SIGNAL(clicked(bool)), this, SLOT(OnClickedClients(bool)));
    connect(m_ui->checkBoxClientRegister, SIGNAL(clicked()), this, SLOT(OnClickedClient()));
    connect(m_ui->checkBoxAccountManagerRegister, SIGNAL(clicked()), this, SLOT(OnClickedClient()));
    connect(m_ui->checkBoxAgreementRegister, SIGNAL(clicked()), this, SLOT(OnClickedClient()));
    connect(m_ui->checkBoxManagerSellsRegister, SIGNAL(clicked()), this, SLOT(OnClickedClient()));
    connect(m_ui->checkBoxRestoreClients,SIGNAL(clicked()),this,SLOT(OnClickedClient()));


}

ProfileRegister::~ProfileRegister()
{
    delete m_ui;
}

void ProfileRegister::changeEvent(QEvent *e)
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

void ProfileRegister::SendCurrentTab(int currentTab)
{
    m_ui->tabWidget->setCurrentIndex(currentTab);
}

void ProfileRegister::SendProfileId(int profileId)
{

    ProfileModel *profileModel = ProfileModel::findById(profileId);

    if (profileModel != NULL)
    {
        m_ui->lineEditProfile->setText(profileModel->getName());

        /** DSM-PAF */
        m_ui->checkBoxPresellRegister->setChecked(profileModel->getPreSellRegister());
        m_ui->checkBoxRestorePreSellRegister->setChecked(profileModel->getRestorePreSellRegister());
        m_ui->checkBoxViewSellsRegister->setChecked(profileModel->getViewSellsRegister());
        m_ui->checkBoxPopDrugRegister->setChecked(profileModel->getPopDrugRegister());
        m_ui->checkBoxCashierRegister->setChecked(profileModel->getCashierRegister());
        m_ui->checkBoxCashierViewRegister->setChecked(profileModel->getCashierViewRegister());
        m_ui->checkBoxCashierCancelRegister->setChecked(profileModel->getCashierCancelRegister());
        m_ui->checkBoxTaxMenuRegister->setChecked(profileModel->getTaxMenuRegister());
        m_ui->checkBoxPaymentwayRegister->setChecked(profileModel->getPaymentwayRegister());

        m_ui->checkBoxCanceliItemOnSell->setChecked(profileModel->getCancelItemOnSell());

        m_ui->doubleSpinBoxMaxDiscount->setValue(profileModel->getMaxDiscountAllowed());
        m_ui->checkBoxPED->setChecked(profileModel->getPEDRegister());
        m_ui->checkBoxOpenCashierEachLogin->setChecked(profileModel->getOpenCashierEachLogin());
        m_ui->checkBoxSellProductsControl->setChecked(profileModel->getSellProductControl());


        /** DSM-SG */
        m_ui->checkBoxUserRegister->setChecked(profileModel->getUserRegister());
        m_ui->checkBoxProfileRegister->setChecked(profileModel->getProfileRegister());
        m_ui->checkBoxGroupRegister->setChecked(profileModel->getGroupRegister());
        m_ui->checkBoxProductRegister->setChecked(profileModel->getProductRegister());
        m_ui->checkBoxAbcFarmaRegister->setChecked(profileModel->getABCFarmaRegister());
        m_ui->checkBoxOrderRegistrationRegister->setChecked(profileModel->getOrderRegistrationRegister());
        m_ui->checkBoxTransfRegister->setChecked(profileModel->getTransfRegister());
        m_ui->checkBoxMovementRegister->setChecked(profileModel->getMovementRegister());
        m_ui->checkBoxChangeGroupsRegister->setChecked(profileModel->getChangeGroupsRegister());
        m_ui->checkBoxMoveProductsRegister->setChecked(profileModel->getMoveProductsRegister());
        m_ui->checkBoxLotManagerRegister->setChecked(profileModel->getLotManagerRegister());
        m_ui->checkBoxFiscalListsRegister->setChecked(profileModel->getFiscalListsRegister());
        m_ui->checkBoxSngpcRegister->setChecked(profileModel->getSngpcRegister());
        m_ui->checkBoxProductBalanceRegister->setChecked(profileModel->getProductBalanceRegister());
        m_ui->checkBoxManufacturerRegister->setChecked(profileModel->getManufacturerRegister());
        m_ui->checkBoxSupplierRegister->setChecked(profileModel->getSupplierRegister());
        m_ui->checkBoxStoreRegister->setChecked(profileModel->getStoreRegister());
        m_ui->checkBoxCRMRegister->setChecked(profileModel->getCRMRegister());
        m_ui->checkBoxClientRegister->setChecked(profileModel->getClientRegister());
        m_ui->checkBoxChangeClientLimit->setChecked(profileModel->getChangeClientLimit());
        m_ui->checkBoxAllowSaveCredit->setChecked(profileModel->getSaveCredit());
        m_ui->checkBoxAccountManagerRegister->setChecked(profileModel->getAccountManagerRegister());
        m_ui->checkBoxAgreementRegister->setChecked(profileModel->getAgreementManagerRegister());
        m_ui->checkBoxManagerSellsRegister->setChecked(profileModel->getManagerSellsRegister());
        m_ui->checkBoxDsmHistoryRegister->setChecked(profileModel->getDsmHistoryRegister());
        m_ui->checkBoxConfigRegister->setChecked(profileModel->getConfigRegister());
        m_ui->checkBoxRestoreClients->setChecked(profileModel->getRestoreClientsRegister());
        m_ui->checkBoxCalcDare->setChecked(profileModel->getCalcDareRegister());
        m_ui->checkBoxProductOrderRegister->setChecked(profileModel->getProductOrderRegister());
        m_ui->checkBoxProductOrderFinish->setChecked(profileModel->getProductOrderFinish());
        m_ui->checkBoxPromotionRegister->setChecked(profileModel->getPromotionRegister());
        m_ui->checkBoxReserveProduct->setChecked(profileModel->getReserveProduct());

        /** Financier */
        m_ui->checkBoxAccountTypeRegister->setChecked(profileModel->getAccountTypeRegister());
        m_ui->checkBoxBankRegister->setChecked(profileModel->getBankRegister());
        m_ui->checkBoxAccountToPayRegister->setChecked(profileModel->getAccountToPayRegister());
        m_ui->checkBoxAccountToReceiveRegister->setChecked(profileModel->getAccountToReceiveRegister());
        m_ui->checkBoxAccountHistoryRegister->setChecked(profileModel->getAccountHistoryRegister());
        m_ui->checkBoxAccountCalendarRegister->setChecked(profileModel->getAccountCalendarRegister());

        /** Reports */
        m_ui->checkBoxComissionReport->setChecked(profileModel->getCommissionReport());
        m_ui->checkBoxSellersRankingReport->setChecked(profileModel->getSellersRankingReport());
        m_ui->checkBoxAccountTypeReport->setChecked(profileModel->getAccountTypeReport());
        m_ui->checkBoxProductRankingReport->setChecked(profileModel->getProductRankingReport());
        m_ui->checkBoxClientRankingReport->setChecked(profileModel->getClientRankingReport());
        m_ui->checkBoxSellDeliveryRegister->setChecked(profileModel->getSellDeliveryReport());
        m_ui->checkBoxProductEntranceReport->setChecked(profileModel->getProductEntranceReport());
        m_ui->checkBoxGroupEntranceReport->setChecked(profileModel->getGroupEntranceReport());
        m_ui->checkBoxTableSells->setChecked(profileModel->getTableSellReport());
        m_ui->checkBoxBalanceReport->setChecked(profileModel->getBalanceReport());
        m_ui->checkBoxStationaryProductsReport->setChecked(profileModel->getStationaryProductsReport());
        m_ui->checkBoxTotalStock->setChecked(profileModel->getTotalStockReport());
        m_ui->checkBoxCashierHistoryReport->setChecked(profileModel->getCashierHistoryReport());
        m_ui->checkBoxGroupSellReport->setChecked(profileModel->getGroupSellReport());
        m_ui->checkBoxCommissionSellReport->setChecked(profileModel->getCommissionSellReport());
        m_ui->checkBoxProductReport->setChecked(profileModel->getProductReport());
        m_ui->checkBoxProductLostReport->setChecked(profileModel->getProductLostReport());
        m_ui->checkBoxSellManageReport->setChecked(profileModel->getSellManagerReport());
        m_ui->checkBoxSellbySellReport->setChecked(profileModel->getSellBySellReport());
        m_ui->checkBoxStockPositionReport->setChecked(profileModel->getStockPositionReport());
        m_ui->checkBoxStockPositionAnvisaReport->setChecked(profileModel->getStockPositionAnvisaReport());
        m_ui->checkBoxPisCofinsReport->setChecked(profileModel->getPISCofinsReport());
        m_ui->checkBoxOffer->setChecked(profileModel->getDiscountRegister());
        m_ui->checkBoxSaleCanceled->setChecked(profileModel->getSaleCanceledReport());
        m_ui->checkBoxMovementProduct->setChecked(profileModel->getMovementProductReport());
        m_ui->checkBoxReceiptLooseReport->setChecked(profileModel->getReceiptLooseReport());
        m_ui->checkBoxManufacturerSellReport->setChecked(profileModel->getManufacturerSellReport());
        m_ui->checkBoxComissionPaf->setChecked(profileModel->getCommissionPaf());

        /** Delivery */
        m_ui->checkBoxMotoBoy->setChecked(profileModel->getMotoboy());

        OnClickedProduct();
        OnClickedClient();

        m_profileId = profileId;
    }

    /** New Profile x Group comission **/

    QSqlQueryModel *profilecom = new QSqlQueryModel;
    profilecom->setQuery( QString("select g.description, pc.value  from dsm_group g left join profilecomission pc on pc.groupid = g.id and pc.profileid = %1 where g.removed = false order by g.description").arg(m_profileId));

    QStringList TblHeader;
    TblHeader << "Grupo" << QString::fromUtf8("Comissão %");

    m_ui->tableWidgetcommission->setColumnCount( 2 ) ;
    m_ui->tableWidgetcommission->setRowCount( profilecom->rowCount() );
    m_ui->tableWidgetcommission->setHorizontalHeaderLabels( TblHeader );
    m_ui->tableWidgetcommission->horizontalHeader()->setSectionResizeMode( 0, QHeaderView::Stretch );

    qDebug() << "TotalRegistros: " << profilecom->rowCount() ;
    for( int iRow = 0; iRow < profilecom->rowCount(); iRow++ )
    {
        m_ui->tableWidgetcommission->setItem ( iRow, 0, new QTableWidgetItem( profilecom->record(iRow).field(0).value().toString()));

        Qt::ItemFlags flags =  m_ui->tableWidgetcommission->item(iRow,0)->flags();

        flags &= ~Qt::ItemIsEditable;
        m_ui->tableWidgetcommission->item(iRow,0)->setFlags(flags);
        m_ui->tableWidgetcommission->setItem ( iRow, 1, new QTableWidgetItem(profilecom->record(iRow).field(1).value().toString()));
    }

    if( m_profileId )
    {
        profilecomissionList *pcomm = profilecomission::findByProfileid(m_profileId);
        if( pcomm && pcomm->count())
        {
            m_ui->groupBoxComissionSpe->setChecked(true);
        }
    }
}

void ProfileRegister::SaveProfile(void)
{
    if (!m_ui->lineEditProfile->text().isEmpty())
    {
        ProfileModel *profileModel = new ProfileModel;

        m_profileName = m_ui->lineEditProfile->text();

        /** name */
        profileModel->setName(m_ui->lineEditProfile->text().toUpper());

        /** presell_register */
        profileModel->setPreSellRegister(m_ui->checkBoxPresellRegister->isChecked());

        /** restorepresell_register */
        profileModel->setRestorePreSellRegister(m_ui->checkBoxRestorePreSellRegister->isChecked());

        /** viewsells_register */
        profileModel->setViewSellsRegister(m_ui->checkBoxViewSellsRegister->isChecked());

        /** fiscalmng_register */
        profileModel->setFiscalListsRegister(m_ui->checkBoxFiscalListsRegister->isChecked());

        /** popdrug_register */
        profileModel->setPopDrugRegister(m_ui->checkBoxPopDrugRegister->isChecked());

        /** cashier_register */
        profileModel->setCashierRegister(m_ui->checkBoxCashierRegister->isChecked());

        /** cashierview_register */
        profileModel->setCashierViewRegister(m_ui->checkBoxCashierViewRegister->isChecked());

        /** cashiercancel_register */
        profileModel->setCashierCancelRegister(m_ui->checkBoxCashierCancelRegister->isChecked());

        /** taxmenu_register */
        profileModel->setTaxMenuRegister(m_ui->checkBoxTaxMenuRegister->isChecked());

        /** paymentway_register */
        profileModel->setPaymentwayRegister(m_ui->checkBoxPaymentwayRegister->isChecked());

        /** cancelitemonsell */
        profileModel->setCancelItemOnSell(m_ui->checkBoxCanceliItemOnSell->isChecked());

        /** user_register */
        profileModel->setUserRegister(m_ui->checkBoxUserRegister->isChecked());

        /** profile_register */
        profileModel->setProfileRegister(m_ui->checkBoxProfileRegister->isChecked());

        /** group_register */
        profileModel->setGroupRegister(m_ui->checkBoxGroupRegister->isChecked());

        /** product_register */
        profileModel->setProductRegister(m_ui->checkBoxProductRegister->isChecked());

        /** abcfarma_register */
        profileModel->setABCFarmaRegister(m_ui->checkBoxAbcFarmaRegister->isChecked());

        /** orderregistration_register */
        profileModel->setOrderRegistrationRegister(m_ui->checkBoxOrderRegistrationRegister->isChecked());

        /** transf_register */
        profileModel->setTransfRegister(m_ui->checkBoxTransfRegister->isChecked());

        /** movement_register */
        profileModel->setMovementRegister(m_ui->checkBoxMovementRegister->isChecked());

        /** changegroups_register */
        profileModel->setChangeGroupsRegister(m_ui->checkBoxChangeGroupsRegister->isChecked());

        /** moveproducts_register */
        profileModel->setMoveProductsRegister(m_ui->checkBoxMoveProductsRegister->isChecked());

        /** lotmanager_register */
        profileModel->setLotManagerRegister(m_ui->checkBoxLotManagerRegister->isChecked());

        /** productbalance_register */
        profileModel->setProductBalanceRegister(m_ui->checkBoxProductBalanceRegister->isChecked());

        /** sngpc_register */
        profileModel->setSngpcRegister(m_ui->checkBoxSngpcRegister->isChecked());

        /** manufacturer_register */
        profileModel->setManufacturerRegister(m_ui->checkBoxManufacturerRegister->isChecked());

        /** supplier_register */
        profileModel->setSupplierRegister(m_ui->checkBoxSupplierRegister->isChecked());

        /** store_register */
        profileModel->setStoreRegister(m_ui->checkBoxStoreRegister->isChecked());

        /** crm_register */
        profileModel->setCRMRegister(m_ui->checkBoxCRMRegister->isChecked());

        /** client_register */
        profileModel->setClientRegister(m_ui->checkBoxClientRegister->isChecked());

        /** changeclientlimit */
        profileModel->setChangeClientLimit(m_ui->checkBoxChangeClientLimit->isChecked());

        /** saveCredit */
        profileModel->setSaveCredit(m_ui->checkBoxAllowSaveCredit->isChecked());

        /** accountmanager_register */
        profileModel->setAccountManagerRegister(m_ui->checkBoxAccountManagerRegister->isChecked());

        /** agreementmanager_register */
        profileModel->setAgreementManagerRegister(m_ui->checkBoxAgreementRegister->isChecked());

        /** managersells_register */
        profileModel->setManagerSellsRegister(m_ui->checkBoxManagerSellsRegister->isChecked());

        /** dsmhistory_register */
        profileModel->setDsmHistoryRegister(m_ui->checkBoxDsmHistoryRegister->isChecked());

        /** config_register */
        profileModel->setConfigRegister(m_ui->checkBoxConfigRegister->isChecked());

        /** maxdiscountallowed */
        profileModel->setMaxDiscountAllowed(m_ui->doubleSpinBoxMaxDiscount->value());

        /** selldelivery_register */
        profileModel->setSellDeliveryReport(m_ui->checkBoxSellDeliveryRegister->isChecked());

        /** accounttype_report */
        profileModel->setAccountTypeReport(m_ui->checkBoxAccountTypeReport->isChecked());

        /** commission_report */
        profileModel->setCommissionReport(m_ui->checkBoxComissionReport->isChecked());

        /** sellersranking_report */
        profileModel->setSellersRankingReport(m_ui->checkBoxSellersRankingReport->isChecked());

        /** productranking_report */
        profileModel->setProductRankingReport(m_ui->checkBoxProductRankingReport->isChecked());

        /** clientranking_report */
        profileModel->setClientRankingReport(m_ui->checkBoxClientRankingReport->isChecked());

        /** productentrance_report */
        profileModel->setProductEntranceReport(m_ui->checkBoxProductEntranceReport->isChecked());

        /** groupentrance_report */
        profileModel->setGroupEntranceReport(m_ui->checkBoxGroupEntranceReport->isChecked());

        /** tablesell_report */
        profileModel->setTableSellReport(m_ui->checkBoxTableSells->isChecked());

        /** stationaryproducts_report */
        profileModel->setStationaryProductsReport(m_ui->checkBoxStationaryProductsReport->isChecked());

        /** totalstock_report */
        profileModel->setTotalStockReport(m_ui->checkBoxTotalStock->isChecked());

        /** balance_report */
        profileModel->setBalanceReport(m_ui->checkBoxBalanceReport->isChecked());

        /** cashierhistory_report */
        profileModel->setCashierHistoryReport(m_ui->checkBoxCashierHistoryReport->isChecked());

        /** groupsell_report */
        profileModel->setGroupSellReport(m_ui->checkBoxGroupSellReport->isChecked());

        /** manufacturersell_report */
        profileModel->setManufacturerSellReport(m_ui->checkBoxManufacturerSellReport->isChecked());

        /** commissionsell_report */
        profileModel->setCommissionSellReport(m_ui->checkBoxGroupSellReport->isChecked());

        /** product_report */
        profileModel->setProductReport(m_ui->checkBoxProductReport->isChecked());

        /** productlost_report */
        profileModel->setProductLostReport(m_ui->checkBoxProductLostReport->isChecked());

        /** sellmanager_report */
        profileModel->setSellManagerReport(m_ui->checkBoxSellManageReport->isChecked());

        /** sellbysell_report */
        profileModel->setSellBySellReport(m_ui->checkBoxSellbySellReport->isChecked());

        /** stockposition_report */
        profileModel->setStockPositionReport(m_ui->checkBoxStockPositionReport->isChecked());

        /** stockpositionanvisa_report */
        profileModel->setStockPositionAnvisaReport(m_ui->checkBoxStockPositionAnvisaReport->isChecked());

        /** piscofins_report */
        profileModel->setPISCofinsReport(m_ui->checkBoxPisCofinsReport->isChecked());

        /** motoboy */
        profileModel->setMotoboy(m_ui->checkBoxMotoBoy->isChecked());

        /** ped_register */
        profileModel->setPEDRegister(m_ui->checkBoxPED->isChecked());

        /** restoreclients_register */
        profileModel->setRestoreClientsRegister(m_ui->checkBoxRestoreClients->isChecked());

        /** accounttype_register */
        profileModel->setAccountTypeRegister(m_ui->checkBoxAccountTypeRegister->isChecked());

        /** bank_register */
        profileModel->setBankRegister(m_ui->checkBoxBankRegister->isChecked());

        /** accounttopay_register */
        profileModel->setAccountToPayRegister(m_ui->checkBoxAccountToPayRegister->isChecked());

        /** accounttoreceive_register */
        profileModel->setAccountToReceiveRegister(m_ui->checkBoxAccountToReceiveRegister->isChecked());

        /** accounthistory_register */
        profileModel->setAccountHistoryRegister(m_ui->checkBoxAccountHistoryRegister->isChecked());

        /** accountcalendar_register */
        profileModel->setAccountCalendarRegister(m_ui->checkBoxAccountCalendarRegister->isChecked());

        /** discount_register */
        profileModel->setDiscountRegister(m_ui->checkBoxOffer->isChecked());

        /** caldare_register */
        profileModel->setCalcDareRegister(m_ui->checkBoxCalcDare->isChecked());

        /** salecanceled_report */
        profileModel->setSaleCanceledReport(m_ui->checkBoxSaleCanceled->isChecked());

        /** movementproduct_report */
        profileModel->setMovementProductReport(m_ui->checkBoxMovementProduct->isChecked());

        /** receiptloose_report */
        profileModel->setReceiptLooseReport(m_ui->checkBoxReceiptLooseReport->isChecked());

        /** productorder_register */
        profileModel->setProductOrderRegister(m_ui->checkBoxProductOrderRegister->isChecked());

        /** product_reserve */
        profileModel->setReserveProduct(m_ui->checkBoxReserveProduct->isChecked());

        /** productorder_finish */
        profileModel->setProductOrderFinish(m_ui->checkBoxProductOrderFinish->isChecked());

        /** promotion_register */
        profileModel->setPromotionRegister(m_ui->checkBoxPromotionRegister->isChecked());

        profileModel->setOpenCashierEachLogin(m_ui->checkBoxOpenCashierEachLogin->isChecked());

        profileModel->setSellProductControl(m_ui->checkBoxSellProductsControl->isChecked());

        /** comission PAF*/
        profileModel->setCommissionPaf(m_ui->checkBoxComissionPaf->isChecked());

        if (m_profileId != 0)
        {
            /** id */
            profileModel->setId(m_profileId);
        }
        if(!profileModel->Save())
        {
            QMessageBox::critical(this, MSG_ERROR_DATABASE_TITLE, MSG_ERROR_DATABASE);
        }
        else
        {
            QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_RECORD_OK);
        }
        this->close();


        if( m_ui->groupBoxComissionSpe->isChecked() )
        {

            for( int iRow = 0; iRow < m_ui->tableWidgetcommission->rowCount(); iRow++ )
            {
                Group *pgroup = Group::findByName(m_ui->tableWidgetcommission->item(iRow,0)->text());
                if( ! pgroup )
                    continue;

                profilecomission *pcomm = profilecomission::findBy(profileModel->getId(),pgroup->getId());
                if( pcomm )
                {
                    qDebug() << __FUNCTION__ << __LINE__ << "Getid: " << pcomm->getId();
                    qDebug() << __FUNCTION__ << __LINE__ << "getvalue: " << pcomm->getvalue();
                    pcomm->updatevalue(m_ui->tableWidgetcommission->item(iRow,1)->text().toDouble());
                    qDebug() << __FUNCTION__ << __LINE__ ;
                    qDebug() << __FUNCTION__ << __LINE__ << "after save: " << pcomm->getvalue();
                    delete pcomm;
                    qDebug() << __FUNCTION__ << __LINE__ ;
                }
                else
                {

                    pcomm =  new profilecomission();

                    pcomm->setgroupid( pgroup->getId() );
                    pcomm->setprofileid( profileModel->getId());
                    pcomm->setvalue(m_ui->tableWidgetcommission->item(iRow,1)->text().toDouble());
                    pcomm->Save();
                    delete pcomm;
                }
                delete pgroup;

            }

        }
        else
        {
            QSqlQueryModel *modelaux = new QSqlQueryModel;
            modelaux->setQuery( QString("delete from profilecomission where profileid = %1").arg(profileModel->getId()) );
            delete modelaux;

        }
        delete profileModel;
    }
}

QString ProfileRegister::GetProfileName(void)
{
    return m_profileName;
}


void ProfileRegister::OnClickedProducts(bool checked)
{
    m_ui->checkBoxProductRegister->setChecked(checked);
    m_ui->checkBoxAbcFarmaRegister->setChecked(checked);
    m_ui->checkBoxOrderRegistrationRegister->setChecked(checked);
    m_ui->checkBoxTransfRegister->setChecked(checked);
    m_ui->checkBoxMovementRegister->setChecked(checked);
    m_ui->checkBoxChangeGroupsRegister->setChecked(checked);
    m_ui->checkBoxLotManagerRegister->setChecked(checked);
    m_ui->checkBoxProductBalanceRegister->setChecked(checked);
    m_ui->checkBoxSngpcRegister->setChecked(checked);
    m_ui->checkBoxOffer->setChecked(checked);
    m_ui->checkBoxCalcDare->setChecked(checked);
}

void ProfileRegister::OnClickedProduct(void)
{
    bool checked = (m_ui->checkBoxProductRegister->isChecked() ||
                    m_ui->checkBoxAbcFarmaRegister->isChecked() ||
                    m_ui->checkBoxOrderRegistrationRegister->isChecked() ||
                    m_ui->checkBoxTransfRegister->isChecked() ||
                    m_ui->checkBoxMovementRegister->isChecked() ||
                    m_ui->checkBoxChangeGroupsRegister->isChecked() ||
                    m_ui->checkBoxLotManagerRegister->isChecked() ||
                    m_ui->checkBoxProductBalanceRegister->isChecked() ||
                    m_ui->checkBoxSngpcRegister->isChecked() ||
                    m_ui->checkBoxOffer->isChecked() ||
                    m_ui->checkBoxCalcDare->isChecked());

    m_ui->checkBoxProducts->setChecked(checked);
}

void ProfileRegister::OnClickedClients(bool checked)
{
    m_ui->checkBoxClientRegister->setChecked(checked);
    m_ui->checkBoxAccountManagerRegister->setChecked(checked);
    m_ui->checkBoxAgreementRegister->setChecked(checked);
    m_ui->checkBoxManagerSellsRegister->setChecked(checked);
    m_ui->checkBoxRestoreClients->setChecked(checked);
}

void ProfileRegister::OnClickedClient(void)
{
    bool checked = (m_ui->checkBoxClientRegister->isChecked() ||
                    m_ui->checkBoxAccountManagerRegister->isChecked() ||
                    m_ui->checkBoxAgreementRegister->isChecked() ||
                    m_ui->checkBoxManagerSellsRegister->isChecked() ||
                    m_ui->checkBoxRestoreClients->isChecked());

    m_ui->checkBoxClients->setChecked(checked);
}

void ProfileRegister::ConfigDsmType(void)
{
    DSM_Type_t type = dsmConfig::GetDsmType();

    m_ui->checkBoxPopDrugRegister->setVisible((type == DrugType)?true:false);
    m_ui->checkBoxAbcFarmaRegister->setVisible((type == DrugType)?true:false);
    m_ui->checkBoxSngpcRegister->setVisible((type == DrugType)?true:false);
    m_ui->checkBoxCRMRegister->setVisible((type == DrugType)?true:false);
}
