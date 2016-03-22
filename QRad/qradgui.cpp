#include "qradgui.h"
#include "ui_qradgui.h"
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QSqlRelationalDelegate>

#define QRAD_TBL_TYPES \
"CREATE TABLE QRAD_TYPES (\n"\
"id integer primary key autoincrement, \n"\
"type character variyng \n"\
");\n"

#define QRAD_TBL_FKEY \
"CREATE TABLE QRAD_TBL_TYPES (\n"\
"id integer primary key autoincrement, \n"\
"type character variyng \n"\
");\n"


#define QRAD_TBL_SKELS \
"CREATE TABLE QRAD_SKELS (\n"\
"id integer primary key autoincrement, \n"\
"Field character varying, \n"\
"typeid integer,  \n"\
"foretableid integer,  \n"\
"forefieldid integer,  \n"\
"tableid integer,  \n"\
"FOREIGN KEY(typeid) REFERENCES QRAD_TYPES(id),\n"\
"FOREIGN KEY(tableid) REFERENCES QRAD_TABLES(id)\n"\
");\n"

#define QRAD_TBL_TABLES\
    "CREATE TABLE QRAD_TABLES (\n"\
    "id integer primary key autoincrement, \n"\
    "Foreing character varying \n"\
    ");\n"


#define QRAD_TYPES_INSERT_INT \
    "insert into QRAD_TYPES(type) values('int');\n"

#define QRAD_TYPES_INSERT_QSTRING \
    "insert into QRAD_TYPES(type) values('QString');\n"

#define QRAD_TYPES_INSERT_DOUBLE \
    "insert into QRAD_TYPES(type) values('double');\n"

#define QRAD_TYPES_INSERT_QDATE \
    "insert into QRAD_TYPES(type) values('QDate');\n"

#define QRAD_TYPES_INSERT_QTIME \
    "insert into QRAD_TYPES(type) values('QTime');\n"

#define QRAD_TYPES_INSERT_BOOL \
    "insert into QRAD_TYPES(type) values('bool');\n"

QRadGui::QRadGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRadGui)
{
    ui->setupUi(this);

    setWindowTitle("QRad Builder");
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery(QRAD_TBL_TYPES);
    model->setQuery("SELECT * FROM QRAD_TYPES");
    if( model->rowCount() < 1)
    {
        model->setQuery(QRAD_TYPES_INSERT_INT);
        model->setQuery(QRAD_TYPES_INSERT_DOUBLE);
        model->setQuery(QRAD_TYPES_INSERT_BOOL);
        model->setQuery(QRAD_TYPES_INSERT_QSTRING);
        model->setQuery(QRAD_TYPES_INSERT_QDATE);
        model->setQuery(QRAD_TYPES_INSERT_QTIME);
    }

    model->setQuery(QRAD_TBL_SKELS);
    model->setQuery(QRAD_TBL_TABLES);
    delete model;

    connect(ui->PshBtnCancel,SIGNAL(clicked()), this, SLOT(reject()));

    QSqlRelationalTableModel *modelFields = new QSqlRelationalTableModel;

    modelFields->setTable("QRAD_SKELS");
    modelFields->setRelation(2, QSqlRelation("QRAD_TYPES", "id", "type"));
   // modelFields->relationModel(2)->setFilter("id=2");
    modelFields->setRelation(3, QSqlRelation("QRAD_TABLES", "id", "Foreing"));
    modelFields->setRelation(4, QSqlRelation("QRAD_SKELS", "forefieldid", "Field"));
    modelFields->select();
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    modelFields->insertRow(modelFields->rowCount(QModelIndex()));
    ui->tableView->setModel(modelFields);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(5);
    modelFields->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Name"));
    modelFields->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Type"));
    modelFields->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Foreing"));
    modelFields->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("For. Field"));
    ui->tableView->horizontalHeader()->setStretchLastSection(true);


    QSqlTableModel *modelTables = new QSqlTableModel;
    modelTables->setTable("QRAD_TABLES");
    modelTables->select();
    modelTables->insertRow(modelTables->rowCount(QModelIndex()));
    ui->tableViewTables->setModel(modelTables);
    ui->tableViewTables->hideColumn(0);
    modelTables->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Table"));
    ui->tableViewTables->horizontalHeader()->setStretchLastSection(true);

}

QRadGui::~QRadGui()
{
    delete ui;
}
