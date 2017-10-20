#include "editreserve.h"
#include "ui_editreserve.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editreserve::Editreserve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editreserve)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxcommona_id->setTable("common_area.Local");
    ui->CmbBxcommona_id->setField("name.Nome");
    ui->CmbBxcommona_id->setCanAdd(true);
    ui->CmbBxcommona_id->setUserName("dsm");
    ui->CmbBxcommona_id->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editreserve::~Editreserve()
{
    delete ui;
}
void Editreserve::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editreserve::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editreserve::SetModel(reserve* mod)
{
   m_mod = mod;
   Load();
}


void Editreserve::Save()
{

    reserve* mod =  m_mod;
    if( m_mod == NULL)
        mod = new reserve;

    mod->setcommona_id(ui->CmbBxcommona_id->model()->data(ui->CmbBxcommona_id->model()->index(ui->CmbBxcommona_id->currentIndex(), 0)).toInt());

    mod->setdate_start(ui->DtEdtdate_start->date());
    mod->settime_start(ui->TmEdttime_start->time());
    mod->setdate_end(ui->DtEdtdate_end->date());
    mod->settime_end(ui->TmEdttime_end->time());
    mod->setresponsable(ui->SpnBxresponsable->value());
    mod->setused(ui->ChkBxused->isChecked());
    mod->setobs_before(ui->LnEdtobs_before->text());
    mod->setobs_after(ui->LnEdtobs_after->text());
    mod->setproblem(ui->ChkBxproblem->isChecked());
    bool bRet = mod->Save();
    if( m_lastMod )
       delete m_lastMod;
    m_lastMod = mod;
    m_mod = NULL;
    if( bRet )
    {
       QMessageBox::information(this, "Sucesso!","Informações foram salvas com sucesso!");
       accept();
    }
    else
       QMessageBox::warning(this, "Oops","Não foi possivel salvar");
    
}

void Editreserve::Load()
{
    if( m_mod == NULL)
      return;
    ui->CmbBxcommona_id->setCurrentId(m_mod->getcommona_id());
    ui->DtEdtdate_start->setDate(m_mod->getdate_start());
    ui->TmEdttime_start->setTime(m_mod->gettime_start());
    ui->DtEdtdate_end->setDate(m_mod->getdate_end());
    ui->TmEdttime_end->setTime(m_mod->gettime_end());
    ui->SpnBxresponsable->setValue(m_mod->getresponsable());
    ui->ChkBxused->setChecked(m_mod->getused());
    ui->LnEdtobs_before->setText(m_mod->getobs_before());
    ui->LnEdtobs_after->setText(m_mod->getobs_after());
    ui->ChkBxproblem->setChecked(m_mod->getproblem());

}

void Editreserve::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
reserve* Editreserve::GetSaved()
{
   return m_lastMod;

}

