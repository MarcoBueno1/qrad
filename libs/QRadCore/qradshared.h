#ifndef QRadSHARED_H
#define QRadSHARED_H

#include <QRadioButton>
#include <QDateTime>
#include <QApplication>
#include <QMetaType>

#define __FINANCIER__

#define WIDTH_SCREEN    800
#define HEIGHT_SCREEN   600

/** Formatação de Data e Hora */
#define FMT_TIME        "HH:mm:ss"
#define FMT_TIME_PLUS   "HH:mm:ss.zzz"
#define FMT_TIME_REST   "HH:mm"
#define FMT_DATE        "dd/MM/yyyy"
#define FMT_DATE_DB     "yyyy-MM-dd"
#define FMT_DATE_CAL    "MM/yyyy"
#define FMT_DATE_LOG    "dd-MM-yyyy"
#define FMT_DATE_LOG2   "yyyyMMdd"
#define FMT_TIME_LOG    "HHmmss"

#define SQL_EMPTY   0
#define END_STRING  '\0'

#ifdef _WIN32
#define DIR_INSTALL     "c:\\lnx\\"
#else
#define DIR_INSTALL     "/opt/QRad/bin/"
#endif


#define CONFIG_FILE          "config.enc"
#define CONFIG_FILE_BKP      "config.enc.bkp"
#define CONFIG_FILE_PATH     ((QFile::exists(DIR_INSTALL))?QString("%1%2").arg(DIR_INSTALL).arg(CONFIG_FILE):CONFIG_FILE)
#define CONFIG_FILE_BKP_PATH ((QFile::exists(DIR_INSTALL))?QString("%1%2").arg(DIR_INSTALL).arg(CONFIG_FILE_BKP):CONFIG_FILE_BKP)

#define LOG_FILE_AUX        "QRad.log"
#define LOG_FILE_PATH       ((QFile::exists(DIR_INSTALL))?QString("%1%2").arg(DIR_INSTALL ).arg(LOG_FILE_AUX):LOG_FILE_AUX)

#define REGEXP_INTEGER_NOT_NULL "[1-9][0-9]{0,7}"
#define REGEXP_INTEGER_2        "[1-9]\\d{0,1}"
#define REGEXP_INTEGER_3        "[1-9]\\d{0,2}"
#define REGEXP_INTEGER_4        "[1-9]\\d{0,3}"
#define REGEXP_INTEGER_14       "[0-9]\\d{1,14}"
#define REGEXP_FLOAT_4_3        "[0-9]\\d{0,3}(|,[0-9]\\d{0,2})"
#define REGEXP_PERCENT          "(^100|^[0-9]{1,2}(\\,[0-9]{1,2})?)$"
#define REGEXP_MONEY            "(^[1-9]{1}[0-9]{0,2}((\\.\\d{3}){0,3}|\\d{0,3})|^0)(\\,[0-9]{1,2})?$"
#define REGEXP_DATE             "[0-3][0-9]/[0-9]{1,2}/[0-9]{4,4}"
#define REGEXP_PHONE            "\\(\\d{2}\\)\\s\\d{4}-\\d{4}"
#define REGEXP_PHONE2           "\\d{10}"
#define REGEXP_PHONE_NUMBER9    "\\(\\d{2}\\)\\s\\d{4,5}-\\d{4}"
#define REGEXP_TAXDOC_NUMBER    "[0-9]{1,8}"
#define REGEXP_CRZ              "[0-9]{1,8}"
#define REGEXP_CPF              "\\d{3}\\.\\d{3}\\.\\d{3}\\-\\d{2}"
#define REGEXP_CNPJ             "\\d{2,3}\\.\\d{3}\\.\\d{3}\\/\\d{4}\\-\\d{2}"
#define REGEXP_BARCODE          "\\d{0,14}"
#define REGEXP_NCM              "\\d{0,8}"
#define REGEXP_CCF              "\\d{0,6}"
#define REGEXP_MS_REGISTER      "\\d.\\d{4}.\\d{4}.\\d{3}-\\d"
#define REGEXP_REMOVE_ALL       "\\.|/|-|;|\\s|\\(|\\)"

#define MASK_CNPJ   "00.000.000/0000-00;"
#define MASK_CPF    "000.000.000-00;"
#define MASK_PHONE  "(00)0000-0000"

#define STR_EMPTY   (unsigned char*)""

#define MAX_PAYMENTS            20

#define ST_BARCODE_FOUNDED      1
#define ST_STATUS_OK            0
#define ST_AMOUNT_ZERO          -1
#define ST_BARCODE_NOT_FOUND    -2

/** Pre-Venda */
#define COL_CURRENT_ID          0
#define COL_PRODUCT_ID          1
#define COL_PRODUCT_DESCRIPTION 2
#define COL_PRODUCT_AMOUNT      3
#define COL_PRODUCT_PRICE       4
#define COL_PRODUCT_DISCOUNT    5
#define COL_PRODUCT_TOTAL       6

/** Entrada com Nota */
#define COL_CURRENT_ENT_ID              0
#define COL_PRODUCT_ENT_ID              1
#define COL_PRODUCT_ENT_BARCODE         2
#define COL_PRODUCT_ENT_DESCRIPTION     3
#define COL_PRODUCT_ENT_AMOUNT          4
#define COL_PRODUCT_ENT_COST            5
#define COL_PRODUCT_ENT_DISCOUNT        6
#define COL_PRODUCT_ENT_EXTRADISCOUNT   7
#define COL_PRODUCT_ENT_TOTAL_DISCOUNT  8
#define COL_PRODUCT_ENT_TOTAL_COST      9
#define COL_PRODUCT_ENT_GAIN            10
#define COL_PRODUCT_ENT_PRICE2          11
#define COL_PRODUCT_ENT_UPDATE          12

/** Transferencia */
#define COL_PRODUCT_TRANSF_COST        4
#define COL_PRODUCT_TRANSF_TOTAL_COST  5
#define COL_PRODUCT_TRANSF_PRICE       6
#define COL_PRODUCT_TRANSF_TOTAL       7

#define RB_NOT_FOCUSED  -1

#define LINE_EDIT_STYLESHEET    QString(" QLineEdit{ border: 1px solid gray; border-radius: 3px;padding : 2px ; selection-background-color: rgb(119, 174, 255);} "\
" QLineEdit::focus{ border: 1.5px solid rgb(35, 113, 224); border-radius: 3px; padding : 2px} "\
" QLineEdit::disabled{ border: 1px solid rgb(230, 221, 213); border-radius: 3px; background-color: rgb(236, 236, 236);padding : 2px }"\
" QLineEdit:read-only{ background-color : transparent  ; border-color : transparent  ;} "\
" QLineEdit::focus:read-only{ background-color : transparent  ; border-color : transparent  ;} ")

#define SPINBOX_STYLESHEET QString("QDoubleSpinBox ,QSpinBox { border: 1px solid darkgrey; border-radius: 3px; selection-background-color: rgb(119, 174, 255); } "\
"QDoubleSpinBox::focus , QSpinBox::focus {  border: 1.5px solid rgb(35, 113, 224);  border-right-color: darkgrey; border-right-width: 1px; } "\
"QDoubleSpinBox::up-button , QSpinBox::up-button { subcontrol-origin: border; subcontrol-position: top right; width: 12px; border: 1px solid darkgrey; "\
"border-top-right-radius:3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1#dadbde) ; } "\
"QDoubleSpinBox::up-button:pressed , QSpinBox::up-button:pressed{ background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa) ;} "\
"QDoubleSpinBox::down-button , QSpinBox::down-button { subcontrol-origin: border; subcontrol-position: bottom right;  width: 12px;  border: 1px solid darkgrey; border-bottom-right-radius:3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1#dadbde)  ; } "\
"QDoubleSpinBox::down-button:pressed , QSpinBox::down-button:pressed{  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }"\
"QDoubleSpinBox::up-arrow , QSpinBox::up-arrow{ image: url(:/png/up_arrow.png); height: 4px;} "\
"QDoubleSpinBox::up-arrow:disabled, QDoubleSpinBox::up-arrow:off , QSpinBox::up-arrow:disabled , QSpinBox::up-arrow:off{ image: url(:/png/up_arrow_disabled.png); } "\
"QDoubleSpinBox::down-arrow , QSpinBox::down-arrow { image: url(:/png/down_arrow.png); height: 4px; } "\
"QDoubleSpinBox::down-arrow:disabled, QSpinBox::down-arrow:disabled "\
"QDoubleSpinBox::down-arrow:off , QSpinBox::down-arrow:off{image: url(:/png/down_arrow_disabled.png);} ")

#define DOUBLE_SPINBOX_STYLESHEET QString("QDoubleSpinBox ,QSpinBox { border: 1px solid darkgrey; border-radius: 3px; selection-background-color: rgb(119, 174, 255); } "\
"QDoubleSpinBox::focus , QSpinBox::focus {  border: 1.5px solid rgb(35, 113, 224);  border-right-color: darkgrey; border-right-width: 1px; } "\
"QDoubleSpinBox::up-button , QSpinBox::up-button { subcontrol-origin: border; subcontrol-position: top right; width: 12px; border: 1px solid darkgrey; "\
"border-top-right-radius:3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1#dadbde) ; } "\
"QDoubleSpinBox::up-button:pressed , QSpinBox::up-button:pressed{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa) ;} "\
"QDoubleSpinBox::down-button , QSpinBox::down-button { subcontrol-origin: border; subcontrol-position: bottom right;  width: 12px;  border: 1px solid darkgrey; border-bottom-right-radius:3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1#dadbde)  ; } "\
"QDoubleSpinBox::down-button:pressed , QSpinBox::down-button:pressed{  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }"\
"QDoubleSpinBox::up-arrow , QSpinBox::up-arrow{ image: url(:/png/up_arrow.png); height: 4px;} "\
"QDoubleSpinBox::up-arrow:disabled, QDoubleSpinBox::up-arrow:off , QSpinBox::up-arrow:disabled , QSpinBox::up-arrow:off{ image: url(:/png/up_arrow_disabled.png); } "\
"QDoubleSpinBox::down-arrow , QSpinBox::down-arrow { image: url(:/png/down_arrow.png); height: 4px; } "\
"QDoubleSpinBox::down-arrow:disabled, QSpinBox::down-arrow:disabled "\
"QDoubleSpinBox::down-arrow:off , QSpinBox::down-arrow:off{image: url(:/png/down_arrow_disabled.png);} ")

#define DOUBLE_SPINBOX_STYLESHEET_PROMOTION QString("QDoubleSpinBox ,QSpinBox { border: 1px solid darkgrey; border-radius: 3px; selection-background-color: rgb(119, 174, 255); } "\
"QDoubleSpinBox::focus , QSpinBox::focus {  border: 2.5px solid rgb(100, 240, 100);  border-right-color: darkgrey; border-right-width: 1px; } "\
"QDoubleSpinBox::up-button , QSpinBox::up-button { subcontrol-origin: border; subcontrol-position: top right; width: 12px; border: 1px solid darkgrey; "\
"border-top-right-radius:3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1#dadbde) ; } "\
"QDoubleSpinBox::up-button:pressed , QSpinBox::up-button:pressed{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa) ;} "\
"QDoubleSpinBox::down-button , QSpinBox::down-button { subcontrol-origin: border; subcontrol-position: bottom right;  width: 12px;  border: 1px solid darkgrey; border-bottom-right-radius:3px; background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f6f7fa, stop: 1#dadbde)  ; } "\
"QDoubleSpinBox::down-button:pressed , QSpinBox::down-button:pressed{  background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa); }"\
"QDoubleSpinBox::up-arrow , QSpinBox::up-arrow{ image: url(:/png/up_arrow.png); height: 4px;} "\
"QDoubleSpinBox::up-arrow:disabled, QDoubleSpinBox::up-arrow:off , QSpinBox::up-arrow:disabled , QSpinBox::up-arrow:off{ image: url(:/png/up_arrow_disabled.png); } "\
"QDoubleSpinBox::down-arrow , QSpinBox::down-arrow { image: url(:/png/down_arrow.png); height: 4px; } "\
"QDoubleSpinBox::down-arrow:disabled, QSpinBox::down-arrow:disabled "\
"QDoubleSpinBox::down-arrow:off , QSpinBox::down-arrow:off{image: url(:/png/down_arrow_disabled.png);}, ")

#define COLOR_FOCUSED           "rgb(102, 176, 239)"
#define COLOR_DISABLED          "rgb(230, 221, 213)"

#define FG_COLOR_NORMAL         QString("color: black;")
#define BG_COLOR_NORMAL         QString("background: white;")

#define BG_COLOR_DISABLED       QString("background: " COLOR_DISABLED ";")

#define FG_COLOR_FOCUSED        QString("color: white;")
#define BG_COLOR_FOCUSED        QString("background: " COLOR_FOCUSED ";")

#define FG_COLOR_FOCUSED2       QString("color: " COLOR_FOCUSED ";")

#define FG_SEL_COLOR_FOCUSED    QString("selection-background-color: white;")
#define BG_SEL_COLOR_FOCUSED    QString("selection-color: " COLOR_FOCUSED ";")

#define FG_SEL_COLOR_FOCUSED2   QString("selection-background-color: " COLOR_FOCUSED ";")
#define BG_SEL_COLOR_FOCUSED2   QString("selection-color: white;")

#define FG_COLOR_NOT_FOUND      QString("color: white;")
#define BG_COLOR_NOT_FOUND      QString("background: red;")

#define FG_COLOR_RED            QString("color: red;")
#define FG_COLOR_GREEN          QString("color: green;")

#define BG_FIN_COLOR_GREEN      QColor(100, 240, 100)
#define BG_FIN_COLOR_RED        QColor(240, 100, 100)
#define BG_FIN_COLOR_ORANGE     QColor(240, 125, 40)
#define BG_FIN_COLOR_YELLOW     QColor(255, 255, 0)
#define BG_FIN_COLOR_PURPLE     QColor(85, 85, 255)
#define BG_FIN_COLOR_BLACK      QColor(0, 0, 0)
#define BG_FIN_COLOR_BLUE       QColor(0, 85, 255)



#define BG_COLOR_BLUE           QString("background-color: rgb(0, 85, 255);")
#define BG_COLOR_RED            QString("background-color: red;")
#define BG_COLOR_ORANGE         QString("background-color: rgb(255, 170, 0);")

#define AUTO_CONFIG_FOCUS       QString("QLineEdit{ %1 %2 } QLineEdit:focus{ %3 %4 %5 %6}").arg(FG_COLOR_NORMAL).arg(BG_COLOR_NORMAL).arg(FG_COLOR_FOCUSED).arg(BG_COLOR_FOCUSED).arg(FG_SEL_COLOR_FOCUSED).arg(BG_SEL_COLOR_FOCUSED)

/** Select's */
#define SQL_UPDATE_CURRENT_PRODUCT_AMOUNT_TEMP  "update %1 set (productamount,discount, total) = ('%2','%3','%4') where productid = (%5)"
#define SQL_CANCEL_ITEM_PRESELL                 "update QRad_items_presell set (status) = (?) where presaleid=(?)"
#define SQL_SELECT_PRESELLID                    "select id from QRad_sale where tick = %1 and status = 0 order by id desc"
#define SQL_SELECT_ITEMS_PRESELL_OPENED         "select productamount from QRad_items_presell where presaleid = %1 and productid = %2 and status = 0"
#define SQL_INSERT_ERROR                        "insert into QRad_cashier_error (cashiernumber, userid, date, time, errorapi, error, messageapi) values (%1,%2,'%3','%4',%5,%6,'%7')"
#define SQL_SELECT_TICK                         "select id, tick from QRad_sale where tick = %1 and (status = 0 or status = 4 ) and trim(lasthdserial) <> '' "
#define SQL_SELECT_SELLER_NUMBER                "select u.id, u.name from QRad_user u, QRad_profile p where u.profile = p.id and p.presell_register = true and u.id=%1 and u.removed = false"
#define SQL_SELECT_MOTOBOY_NUMBER               "select u.id, u.name from QRad_user u, QRad_profile p where u.profile = p.id and p.motoboy = true  and u.id=%1 and u.removed = false"
#define SQL_SELECT_CLIENT_BY_PHONE              "select id from QRad_client where (phone= '%1' or cellphone = '%1') and removed = 'false' and ((phone is not null and length(phone) > 7) or ((cellphone is not null and length(cellphone) > 7))) "
#define SQL_SELECT_CLIENT_BY_DOCNUMBER          "select id from QRad_client_abstract where documentnumber='%1' and removed<>'true'"
#define SQL_SELECT_CLIENT_BY_ID                 "select * from QRad_client_abstract where id='%1'"
#define SQL_SELECT_PRODUCT_BY_BARCODE           "select id, precodevenda, fracao, descmaximo from QRad_product where (codigodebarra = '%1' or codigodebarra2 = '%1') and removido <> 'true'"
#define SQL_SELECT_PRODUCT_BY_ADITIONALCODE     "select p.id from QRad_product p inner join QRad_supplier_product_code sp on sp.productid = p.id inner join QRad_supplier s on s.id = sp.supplierid where p.removido = false and s.removido = false and sp.removed = false and s.cnpjcpf = '%1' and sp.code = '%2'"
#define SQL_CREATE_TEMP_TABLE_ITEMS             "create temporary table %1 (id serial primary key, presaleid integer, productid integer, productprice numeric(11,2), productamount numeric(11,3), discount numeric(11,2), total numeric(11,2), prescbookid integer)"
#define SQL_CREATE_TEMP_TABLE_ITEMS_ENTRANCE    "create temporary table %1 (id serial primary key, productid integer, cost numeric(11,2), quantity numeric(11,3), discount numeric(11,2), extradiscount numeric(11,2), totaldiscount numeric(11,2), totalcost numeric(11,2), gain numeric(11,2), price numeric(11,2), updateprice boolean default false)"
#define SQL_CREATE_TEMP_TABLE_ITEMS_TRANSF      "create temporary table %1 (id serial primary key, presaleid integer, productid integer, productcost numeric(11,2), totalcost numeric(11,2), productprice numeric(11,2), productamount numeric(11,3), discount numeric(11,2), total numeric(11,2), prescbookid integer)"
#define SQL_SELECT_PRODUCT_ITEMS                "select i.id, i.productid, p.descricao, i.productamount, i.productprice, i.discount, total, to_char(i.productprice, 'FM9G999G990D00') as price, to_char(i.discount, 'FM9G999G990D00') as disc, to_char(total, 'FM9G999G990D00') as tot, p.codigodebarra from %1 i inner join QRad_product p on productid = p.id order by i.id"
#define SQL_SELECT_PRODUCT_ITEMS_TRANSF         "select i.id, i.productid, p.descricao, i.productamount, i.productcost, i.totalcost, i.productprice, total from %1 i inner join QRad_product p on productid = p.id order by i.id"
#define SQL_UPDATE_CURRENT_PRODUCT_AMOUNT       "update %1 set (productamount,total) = (%2,%3) where id = (%4)"
#define SQL_UPDATE_CURRENT_PRODUCT_AMOUNT_TRANSF "update %1 set (productamount,total, totalcost) = (%2,%3,%4) where id = (%5)"
#define SQL_CLEAN_QRad_TEMP_ITEMS                "delete from %1"
#define SQL_TOTAL_PRICE                         "select sum(total) from %1"
#define SQL_TOTAL_PRICE_TRANSF                  "select sum(totalcost) from %1"
#define SQL_DELETE_ITEM_TEMP_TABLE              "delete from %1 where id=%2"
#define SQL_DROP_TEMP_TABLE_ITEMS               "drop table %1"
#define SQL_INSERT_TEMPORARY_PRODUCT            "insert into %1 (productid, productamount, productprice, discount, total) values (%2,%3,%4,%5,%6)"
#define SQL_INSERT_TEMPORARY_PRODUCT_TRANSF     "insert into %1 (productid, productamount, productcost, totalcost, productprice, total) values (%2,%3,%4,%5,%6,%7)"
#define SQL_INSERT_TEMPORARY_PRODUCT_AUX        "insert into %1 (productid, productamount, productprice, discount, total, prescbookid) values (%2,%3,%4,%5,%6,%7)"
#define SQL_DELETE_TEMPORARY_PRODUCT            "delete from %1"
#define SQL_UPDATE_RESERVE_STOCK_LOTCONTROL     "update QRad_lotecontrol set reservestock = (reservestock + ?) where id = (?)"
#define SQL_UPDATE_RESERVE_STOCK                "update QRad_product set reservestock = (reservestock + ?) where id = (?)"
#define SQL_UPDATE_RESERVE_STOCK_PROMOTION      "update QRad_promotion set reservestock = (reservestock + ?) where id = (?)"
#define SQL_UPDATE_RESERVE_STOCK_LOTCONTROL_PROMOTION     "update QRad_promotion_lot set reservestock = (reservestock + ?) where promotionid = (?) and loteid = (?)"

#define SQL_UPDATE_PRESELL_PAYMENTS             "update QRad_paymentway_presell set (status) = (?) where presaleid=(?)"
#define SQL_UPDATE_SELL_PAYMENTS                "update QRad_debt set (status) = (?) where saleid=(?)"
#define SQL_SELECT_PRESELL_PAYMENTS             "select pp.price, pp.paymentwayid, pp.paymentwayauxtype, pa.*, p.total from QRad_paymentway_presell pp, QRad_sale p, QRad_paymentway pa where pp.presaleid = p.id and pp.paymentwayid = pa.id and pp.status = 0 and p.id = (%1)"
#define SQL_SELECT_SELL_PAYMENTS                "select ps.value, ps.paymentwayauxtype, pa.*, s.total from QRad_debt ps, QRad_sale s, QRad_paymentway pa where ps.saleid = s.id and ps.paymentwayid = pa.id and ps.status = 0 and s.id = (%1)"
#define SQL_SELECT_PRODUCTS_ON_SELL             "select i.id, i.productid, p.descricao, i.productamount, i.productprice, i.discount, i.total, i.promotionid from QRad_sale_item i, QRad_product p where i.saleid=(%1) and i.status = 0 and i.productid = p.id"
#define SQL_SELECT_PRODUCTS_ON_SELL_CANCELED    "select i.id, i.productid, p.descricao, i.productamount, i.productprice, i.discount, i.total from QRad_sale_item i, QRad_product p where i.saleid=(%1) and i.productid = p.id"


#define SQL_SELECT_PRODUCT_ITEMS_LOTCONTROL "select i.id, i.productid, p.descricao, i.productamount, i.productprice, i.discount, total from %1 i, QRad_product p where i.productid = p.id and p.controlarvalidade = 'true' order by i.id"
#define SQL_SELECT_LOT_INFO                 "select * from QRad_lotecontrol where id = (%1)"
#define SQL_UPDATE_PRODUCT_STOCK            "update QRad_product set estoque=(estoque + ?) where id = (?)"
#define SQL_UPDATE_PRODUCT_STOCK_ORDER_REG  "update QRad_product set estoque=(estoque + ?), solicitacao = 0 where id = (?)"
#define SQL_UPDATE_LOTCONTROL_STOCK         "update QRad_lotecontrol set quantity = (quantity + ?) where id = (?)"
#define SQL_INSERT_LOTCONTROL_STOCK         "insert into QRad_lotecontrol(productid, quantity, lotenumber, validitydate) values(?, ?, ?, ?)"

#define SQL_SELECT_PROFILE                  "select p.* from QRad_user u, QRad_profile p where u.profile = p.id and u.id = (%1)"
#define SQL_SELECT_MANUFACTURER_COMBO       "select id, manufacturer from QRad_manufacturer where removido = false and manufacturer <> '' order by manufacturer"
#define SQL_SELECT_SUBSTANCE_COMBO          "select id, susbtance from QRad_anv_dcb "
#define SQL_SELECT_GROUP_COMBO              "select id, description from QRad_group where removed = false and description <> '' order by description"
#define SQL_SELECT_SUPPLIER_COMBO           "select id, descricao from QRad_supplier where removido = false and descricao <> '' order by descricao"
#define SQL_SELECT_STORE_COMBO              "select id, razaosocial as descricao from QRad_store where removido = false and razaosocial <> '' order by descricao"
#define SQL_SELECT_BANK_COMBO               "select id, description from QRad_fin_bank where removed = false and description <> '' order by description"
#define SQL_SELECT_CLASSIFICATION_COMBO     "select id, (code || '-' || description) as description from QRad_anvdrugclass"
#define SQL_SELECT_ACCOUNTTYPE_COMBO        "select id, description from QRad_fin_accounttype where removed = false and description <> '' and (type = %1 or type = %2) order by description"
#define SQL_SELECT_CLIENT_COMBO             "select id, name from QRad_client_abstract where removed = false and name <> '' order by name"
#define SQL_SELECT_SELLER_COMBO             "select u.id, u.name from QRad_user u, QRad_profile p where u.profile = p.id and p.presell_register = true and u.removed = false order by u.name"
#define SQL_SELECT_USER_COMBO               "select u.id, u.name from QRad_user u where u.removed = false order by u.name"
#define SQL_SELECT_MEASURE_COMBO            "select id, acronym from QRad_measure order by id"
#define SQL_SELECT_ALIQUOT_COMBO            "select id, value || ' ' ||  description as  aliq from QRad_aliquot order by id"
#define SQL_SELECT_ANVISAUNDMEDIDA_COMBO    "select * from QRad_anvunidmedmedicamento"

#define SQL_SELECT_SELL_ITEMS       "select QRad_product.codigodebarra, QRad_product.descricao, i.productamount, i.productprice, i.discount, i.productcommission, i.total from QRad_sale_item i, QRad_product where i.status = 0 and i.saleid = (%1) and i.productid = QRad_product.id"
#define SQL_SELECT_PRESELL_ITEMS    "select QRad_product.codigodebarra, QRad_product.descricao, i.productamount, i.productprice, i.discount, i.total from QRad_items_presell i, QRad_product where i.status = 0 and i.presaleid = %1 and i.productid = QRad_product.id"
#define SQL_SELECT_PRESELL_ITEMS_G  "select p.codigodebarra, p.descricao, i.productamount, i.productprice, i.discount, i.total from QRad_sale_item i inner join QRad_sale ps on ps.id = i.saleid inner join QRad_product p on p.id = i.productid where i.status = 0 and ps.status = 0 and ps.tick = %1"

#define SQL_SELECT_AUTH_PRESELL "select u.id from QRad_user u where u.id = %1 and u.password = '%2' and removed = 'false'"

#define SQL_SELECT_GROUP_FILTER         " and grupo = (%1)"
#define SQL_SELECT_MANUFACTURER_FILTER  " and fabricante = (%1)"

#define SQL_SELECT_CLIENT   "select " \
                            "    c.id, " \
                            "    c.name, " \
                            "    c.documentnumber as cpf, " \
                            "    c.phone, " \
                            "    NULL as company, " \
                            "    t.description as type, " \
                            "    NULL as credit, " \
                            "    c.clienttypeid " \
                            "from " \
                            "    QRad_client_abstract  c " \
                            "inner join QRad_uf u on u.id = c.ufid " \
                            "inner join QRad_clienttype t on t.id = c.clienttypeid " \
                            "where " \
                            "    c.removed = false and " \
                            "    u.id = c.ufid " \
                            "    order by c.name "

#define TXT_AGREEMENT_PAID          QString::fromUtf8("PAGAMENTO DE CONVÊNIO %1")
#define TXT_AGREEMENT_EMPLOYEE_PAID QString::fromUtf8("BAIXA DO CONVENIADO %1.")
#define TXT_ACCOUNT_PAID            QString::fromUtf8("PAGAMENTO DE CONTA %1")

#define OBS_AGREEMENT_PAID          QString::fromUtf8("BAIXA DE CONVÊNIO DOS DIAS %1 às %2 e %3.")
#define OBS_ACCOUNT_PAID            QString::fromUtf8("PAGAMENTO DE CONTA.")

#define UNCHECKEDANEX "uncheckedfile" // use this define in files unchecked ( anex in email )

/** Estrutura para os radioButtons que serão criados em tempo de execução */
typedef struct
{
    QRadioButton    *radioButton;
    QString         description;
    int             paymentwayType;
    bool            bHasTef;
}QRad_RadioButtons_t;

typedef enum
{
    noTicketError = 0,
    errTicketEmpty,
    errTicketUsed,
    errTicketNoClientMatch
}QRad_TicketErrors_t;

typedef struct
{
    int     lotId;
    double   amount;
    QString lotNumber;
    QDate   lotValidityDate;
    bool    deleted;
}QRad_LotItems_t;

typedef struct
{
    int     id;
    QList<QRad_LotItems_t> items;
    double amountTotal;
}QRad_Lot_t;

typedef struct
{
    QDate       date;
    QStringList descriptionToPay;
    QStringList descriptionToReceive;
}QRad_AccountCalendar_t;

typedef struct
{
    QString productName;
    float   productQuantity;
    float   lotQuantity;
}QRad_LotStatus_t;

typedef enum{
    typeWithLotControl = 0,
    typeWithLotControlwithMS,
    typeWithoutLotControl,
    typeSuggestionBuy,
    typeProductSell,
    typeProductSugestion,
    typeProductSellGroup,
    typeProductCatalog
}QRadReportType_t;

typedef struct
{
    int     index;
    int     type;
    QString sentence;
    QString query;
    bool    orderByProduct;
}QRad_ProductReport_t;

class QRad_EntranceInfo
{
public:
    QRad_EntranceInfo();
    void clear();

    QString number;
    QDate   dateEmission;
    float   ICMS;
    float   baseICMS;
    float   ICMSST;
    float   baseICMSST;
    float   frete;
    float   insurance;
    float   discount;
    float   others;
    float   IPI;
    float   totalValue;
    float   amount;
    float   total;
    QString ncm;
};

class SuggestionPriceInfo
{
public:
    SuggestionPriceInfo();
    void clear();

    float costPrice;
    double discountBuy;
    double boxPackagePrice;
    double impostImportation;
    double IPI;
    double freightPercent;
    double suframaPercent;
    QString ICMS;
    double gainValue;
    double fixedCostPercent;
    double federalPercent;
    float sellPrice;
    double amount;
    double total;
    QString ncm;

};

class CheckProductsInfo
{
public:
    CheckProductsInfo();
    void clear();

    int productId;
    float costPrice;
    float totalCost;
    float amount;

};

typedef struct
{
    double   value;
    QDate   venc;
}QRad_Billet_t;

#define _begin_action() QApplication::processEvents(); QApplication::setOverrideCursor(Qt::WaitCursor);
#define _end_action()   QApplication::setOverrideCursor(Qt::ArrowCursor);

/** Plugin List */

#define ACTION_QUESTION_CLIENT                      "QuestionClient"
#define ACTION_NEW_CLIENT                           "AddClient"
#define ACTION_NEW_COMPANY                          "AddCompany"
#define ACTION_MNG_CLIENT                           "ManageClient"
#define ACTION_MNG_CLIENT_EXEC                      "ManageClientExec"
#define ACTION_MNG_ACCOUNT                          "ManageAccount"
#define ACTION_MNG_AGREEMENT                        "ManageAgreement"
#define ACTION_MNG_SELLS                            "ManageSells"
#define ACTION_MNG_ACCOUNT_PAF                      "ManageAccountPAF"
#define ACTION_MNG_AGREEMENT_PAF                    "ManageAgreementPAF"
#define ACTION_COMMISSION_REPORT                    "ShowComissionReport"
#define ACTION_SHOW_COMMISSIONSELL                  "ShowCommissionSell"
#define ACTION_SELLERS_RANKING_REPORT               "ShowSellersRankingReport"
#define ACTION_PRODUCT_RAKING_REPORT                "ShowProductRankingReport"
#define ACTION_CLIENT_RAKING_REPORT                 "ShowClientRankingReport"
#define ACTION_PRODUCT_ENTRANCE_REPORT              "ShowProductEntranceReport"
#define ACTION_TABLE_SELL_REPORT                    "ShowTableSellReport"
#define ACTION_STATIONARY_PRODUCTS_REPORT           "ShowStationaryProductsReport"
#define ACTION_TOTALSTOCK_REPORT                    "ShowTotalStockReport"
#define ACTION_SHOW_ABCFARMA                        "ShowABCFarma"
#define ACTION_BALANCE_REPORT                       "ShowBalanceReport"
#define ACTION_DELIVERY_REPORT                      "ShowDeliveryReport"
#define ACTION_SHOW_QRad_HISTORY                     "ShowQRadHistory"
#define ACTION_SHOW_CASHIER_REPORT                  "ShowCashierReport"
#define ACTION_SHOW_CASHIER                         "ShowCashierView"
#define ACTION_SHOW_LOTMANAGER                      "ShowLotManager"
#define ACTION_SHOW_CREDIT_REPORT                   "ShowCreditReport"
#define ACTION_SHOW_GROUPSELL                       "ShowGroupSell"
#define ACTION_SHOW_GROUPENTRANCE                   "ShowGroupEntrance"
#define ACTION_SHOW_SELLMANAGER                     "ShowSellManager"
#define ACTION_SHOW_SELLBYSELL                      "ShowSellBySell"
#define ACTION_SHOW_STOCK_POSITION                  "ShowStockPosition"
#define ACTION_SHOW_STOCK_POSITION_ANVISA           "ShowStockPositionAnvisa"
#define ACTION_SHOW_SHIPPER			    "ShowShipper"
#define ACTION_SHOW_SHIPPER_SEARCH_MODE             "ShowShipperSearch"
#define ACTION_SHOW_PRODUCT_REPORT                  "ShowProductReport"
#define ACTION_SHOW_PRODUCT_LOST_REPORT             "ShowProductLostReport"
#define ACTION_SHOW_PRODUCT_BALANCE                 "ShowProductBalance"
#define ACTION_SHOW_PRODUCT_MANAGER                 "ShowProductManager"
#define ACTION_SHOW_PRODUCT_TRANSF                  "ShowProductTransf"
#define ACTION_SHOW_SELL_DELIVERY                   "ShowSellDelivery"
#define ACTION_SHOW_SELL_CANCELED_REPORT            "ShowSellCanceledReport"
#define ACTION_SHOW_SUGGESTION_REPORT               "ShowProductSuggestionBuyReport"
#define ACTION_SHOW_SALE_CANCELED_ITEMS             "ShowSaleCanceledItems"
#define ACTION_SHOW_PRODUCT_CHANGE_RESERVE_STOCK    "ShowProductChangeReserveStock"
#define ACTION_SHOW_REPLICATOR                      "ShowReplicator"

/* SNGPC Features  */
#define ACTION_SHOW_SNGPC                           "ShowSngpc"
#define ACTION_SHOW_SNGPCCONFIG                     "ShowSngpcConfig"
#define ACTION_SHOW_SNGPCHISTORY                    "ShowSngpcHistory"
#define ACTION_SHOW_SNGPCINVENTORY                 "ShowSngpcInventory"
#define ACTION_SHOW_SNGPC_PRODUCT                   "ShowProductAnv"
#define ACTION_SHOW_SNGPC_PRODUCT_REPORT            "ShowProductAnvReport"


#define ACTION_SHOW_PAF_RESTAURANT                  "ShowPAFRestaurant"
#define ACTION_SHOW_PAF_PRESELL                     "ShowPAFPreSell"
#define ACTION_SHOW_PAF_PRESELL_OPEN_IT              "ShowPAFPreSellOpenIt"
#define ACTION_SHOW_NFE_SEP                         "ShowNFESep"
#define ACTION_SHOW_PAF_RESTORE_PRESELL             "ShowPAFRestorePreSell"
#define ACTION_SHOW_PAF_CASHIER                     "ShowPAFCashier"
#define ACTION_SHOW_PAF_CASHIERNC                   "ShowPAFCashiernc"
#define ACTION_SHOW_CANEL_NFCE                      "ShowPAFCancelNFCE"
#define ACTION_SHOW_PAF_CANCELINVOICE               "ShowPAFCancelInvoice"
#define ACTION_SHOW_PAF_PAYMENTWAYREG               "ShowPAFPaymentwayReg"
#define ACTION_SHOW_PAF_PAYMENTWAYAUXREG            "ShowPAFPaymentwayAuxReg"
#define ACTION_SHOW_PISCOFINS                       "ShowPisCofins"
#define ACTION_SHOW_FINANCIER_ACCOUNTTYPE           "ShowAccountType"
#define ACTION_SHOW_FINANCIER_BANK                  "ShowBank"
#define ACTION_SHOW_FINANCIER_ACCOUNTTOPAY          "ShowAccountToPay"
#define ACTION_SHOW_FINANCIER_ACCOUNTTORECEIVE      "ShowAccountToReceive"
#define ACTION_SHOW_FINANCIER_ACCOUNTHISTORYREPORT  "ShowAccountHistoryReport"
#define ACTION_SHOW_FINANCIER_ACCOUNTTYPEREPORT     "ShowAccountTypeReport"
#define ACTION_SHOW_FINANCIER_ACCOUNTCALENDAR       "ShowAccountCalendar"
#define ACTION_SHOW_RESTORECLIENTS                  "ShowRestoreClients"
#define ACTION_SHOW_ENTERBILLET                     "ShowEnterBillet"
#define ACTION_SHOW_NFE_SALE                        "ShowNFESale"
#define ACTION_SHOW_NFE_TRIBUTE_MANAGER             "ShowNFETributeManager"
#define ACTION_SHOW_NFE_TRIBUTE_REGISTER            "ShowNFETributeRegister"
#define ACTION_SHOW_NFE_CFOP_NCM                    "ShowCFOPxNCM"
#define ACTION_SHOW_NF_SALE                         "ShowNFSale"
#define ACTION_NFE_REPORT                           "ShowNFEReport"
#define ACTION_NFCE_REPORT                           "ShowNFCEReport"
#define ACTION_MOVEMENT_PRODUCT                     "ShowMovementProduct"
#define ACTION_SHOW_RECEIPT_LOOSE_REPORT            "ShowReceiptLooseReport"
#define ACTION_SHOW_PRODUCT_RETROSPECT              "ShowProductRetrospect"
#define ACTION_NEW_NFE_MULTI_SALE                   "NewNfeMultiSale"
#define ACTION_NEW_NFE_TRANSF                       "NewNfeTransf"
#define ACTION_NFE_DANFE                            "NewNfeDanfe"
#define ACTION_NFE_CANCEL                           "NewNfeCancel"
#define ACTION_SHOW_PROMOTION                       "ShowPromotion"
#define ACTION_SHOW_MANUFACTURERSELL_REPORT         "ShowManufacturerSellReport"
#define ACTION_SHOW_CONSISTENCY                     "ShowConsistency"
#define ACTION_SHOW_CASHIERIO_REPORT                "ShowCashierIOReport"
#define ACTION_SHOW_PRODUTS_FINISHEDRECENTLY_REPORT "ShowProductsFinishedRecentlyReport"
#define ACTION_SHOW_SPECIAL_ENTRY                   "ShowSpecialEntry"
#define ACTION_SHOW_PRODUCT_DISCOUNT                "ShowProductDiscount"
#define ACTION_SHOW_SELL_RETROSPECT                 "ShowSellRetrospect"

#define ACTION_SHOW_SELLERSELL_REPORT               "ShowSellerSellReport"
#define ACTION_SHOW_CLIENTBYSELLER_REPORT           "ShowClientBySellerReport"
#define ACTION_SHOW_PAYMENTPLAN                     "ShowPaymentPlan"
#define ACTION_SHOW_BANKACCOUNT                     "ShowBankAccount"

#define ACTION_SHOW_PROPDESC                        "ShowPropDesc"
#define ACTION_SHOW_SG_STORES                       "ShowStoreManager"
#define ACTION_SHOW_SG_COMPANIES                    "ShowCompaniesManager"


#define ACTION_UPDATE_LICENSE                  "UpdateLicense"
#define ACTION_UPDATE_NET_STORES               "UpdateNetStores"
#define ACTION_SEND_ENTRANCE_TO_REMOTE         "UpdateRemoteEntrances"
#define ACTION_SHOW_UPDATE_WIDNOW              "ShowUpdateWindow"
#define ACTION_PROCESS_PHONE_CREDIT            "PhoneCreditSale"
#define ACTION_PROCESS_PHONE_SETTINGS          "PhoneSettings"

#define ACTION_SHOW_BOLETO                      "m2boleto"
#define ACTION_PRODUCT_IMPORT_ORDER             "ShowProductSuggestion"
#define ACTION_SHOW_SERIALNUMBER_LINK           "serialnumberlink"
#define ACTION_SHOW_SEND_MESSAGE                "ShowSendMessage"
#define ACTION_SHOW_DANFE_NFECE                 "ShowDanfe"
#define ACTION_SHOW_DANFE_NFECEMANAGER          "ShowDanfeManager"
#define ACTION_SHOW_SERIALNUMBER_LINK_MANAGER   "serialnumberlinkmanager"
#define ACTION_SHOW_SERIALNUMBER_INSALE         "serialnumberlinksale"
#define ACTION_SHOW_PRODUCT_ENTRANCE_OUT        "ShowProductEntranceOut"
#define ACTION_SHOW_QRadUPDATE                   "QRadupdate"

/** Tax Menu */
#define PLUGIN_ACTION_SHOW              "ShowMenu"
#define PLUGIN_ACTION_LX                "LX"
#define PLUGIN_ACTION_LMFC              "LMFC"
#define PLUGIN_ACTION_LMFS              "LMFS"
#define PLUGIN_ACTION_MFD_MIRROR        "MFDMIRROR"
#define PLUGIN_ACTION_MFD_FILE          "MFDFILE"
#define PLUGIN_ACTION_MF_FILE           "MFFILE"
#define PLUGIN_ACTION_PRODUCT_TABLE     "PRODUCTTABLE"
#define PLUGIN_ACTION_STOCK             "STOCK"
#define PLUGIN_ACTION_ECF_MOVIMENT      "ECFMOVIMENT"
#define PLUGIN_ACTION_PAYMENTWAY        "PAYMENTWAY"
#define PLUGIN_ACTION_PAFID             "PAFID"
#define PLUGIN_ACTION_SALES             "SALES"
#define PLUGIN_ACTION_PRODUCTION_INDEX  "PRODUCTIONINDEX"
#define PLUGIN_ACTION_READZMOVIMENT     "ReadZMoviment"
#define PLUGIN_ACTION_NFGSE             "NOTAFISCALDAGENTE"
#define PLUGIN_ACTION_CONFIG            "CONFIGPARAM"
#define PLUGIN_ACTION_SPED              "SPED"

/* Discount plugin */

#define ACTION_DISCOUNT_SHOW "Show"
#define ACTION_DISCOUNT_INCSALECOUNT "IncSaleCount"
#define ACTION_DISCOUNT_DECSALECOUNT "DecSaleCount"

/* NFE Launcher Plugin */
#define ACTION_NFELAUNCHER_EXEC "Exec"

/* Tag Printer Plugin */
#define ACTION_TAGPRINTERCONFIG_SHOW "ShowTagPrinterConfig"
#define ACTION_TAGPRINTERDIALOG_SHOW "ShowTagPrinterDialog"

#define ACTION_POSSALE_SHOW "ShowPossale"

#ifdef _WIN32
    #define LIBRARY(name) name + ".dll"
#else
    #define LIBRARY(name) "lib" + name + ".so"
#endif

#define START_PROGRESS_DIALOG(progress, step) { progress->setValue(step); progress->show(); progress->update(); QCoreApplication::processEvents();}
#define UPDATE_PROGRESS_DIALOG(progress, step) { progress->setValue(step); progress->update(); QCoreApplication::processEvents();}

#define GET_COMBOBOX_ID(id, combobox) {\
    combobox->setModelColumn(0); \
    id = combobox->currentText().toInt(); \
    combobox->setModelColumn(1); \
}

#define QRad_LOG(str) { QRadLog::Log(__FILE__, str); }
#define QRad_IN QRadLog::Log(__FILE__, QString(">> %1").arg(__PRETTY_FUNCTION__))
#define QRad_OUT QRadLog::Log(__FILE__, QString("<< %1").arg(__PRETTY_FUNCTION__))
#define QRad_LOG_ERROR(str) { QRadLog::LogError(__FILE__, str); }

#define SET_MINIMUM ((QRadConfig::ShowDecimals())?0.001:1)
#define SET_DECIMALS ((QRadConfig::ShowDecimals())?3:0)

#define FORMAT_BALANCE  (QRadConfig::ShowDecimals()?"999999D000":"999999")
#define FORMAT_ZERO (QRadConfig::ShowDecimals()?"0,000":"0")


Q_DECLARE_METATYPE(QList<int>);

#endif // QRadSHARED_H
