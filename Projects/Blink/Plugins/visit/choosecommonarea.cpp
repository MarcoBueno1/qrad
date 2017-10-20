#include "choosecommonarea.h"
#include "ui_choosecommonarea.h"
#include "calendarreserve.h"

#define SQL "select name, id from common_area where removed<> true and tp <> 2 and name <> ''  order by name"


ChooseCommonArea::ChooseCommonArea(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseCommonArea)
{
    ui->setupUi(this);
    m_model = new QSqlQueryModel;
    m_model->setQuery(SQL);
    ui->comboBox->setModel(m_model);
    ui->comboBox->setModelColumn(0);

    connect(ui->pushButtonCancelar, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui->pushButtonReservar, SIGNAL(clicked(bool)), this, SLOT(doReserve(bool)));
}

ChooseCommonArea::~ChooseCommonArea()
{
    delete m_model;
    delete ui;
}
void ChooseCommonArea::doReserve(bool)
{
    ///
    /// chamar calendario aqui
    ///
    ///
    calendarreserve *res = new calendarreserve;
    res->Exec(m_model->index(ui->comboBox->currentIndex(),1).data().toInt(),
              m_model->index(ui->comboBox->currentIndex(),0).data().toString());
//    res->exec();
    delete res;
}
