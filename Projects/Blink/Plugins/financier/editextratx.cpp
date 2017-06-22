#include "editextratx.h"
#include "ui_editextratx.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editextratx::Editextratx(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editextratx)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    ui->CmbBxmotivo->setTable("reasonextratx");
    ui->CmbBxmotivo->setField("Description.Descrição");
    ui->CmbBxmotivo->setCanAdd(true);
    ui->CmbBxmotivo->setUserName("dsm");
    ui->CmbBxmotivo->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));

    ui->dateEdit->setDate(QDate::currentDate().addDays(15));

    ui->lineEditMorador->setSelect("select d.name || ' | ' || t.name || ' | ' || a.numero, d.id, d.name, a.numero, t.name, d.ramal "\
         "from dweller d "\
         "inner join tower t on t.id= d.tower "\
         "inner join ap a on a.id = d.ap  where d.removed = false and d.payer = true");
//    ui->lineEditMorador->Add(ui->lineEditAP);
    //ui->lineEditMorador->Add(ui->lineEditTorre);
//    ui->lineEditMorador->Add(ui->lineEditRamal);
}

Editextratx::~Editextratx()
{
    delete ui;
}
void Editextratx::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editextratx::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editextratx::SetModel(extratx* mod)
{
   m_mod = mod;
   Load();
}


void Editextratx::Save()
{

    extratx* mod =  m_mod;
    if( m_mod == NULL)
        mod = new extratx;

    mod->setMotivo(ui->CmbBxmotivo->model()->data(ui->CmbBxmotivo->model()->index(ui->CmbBxmotivo->currentIndex(), 0)).toInt());
    mod->setData(ui->dateEdit->date());
    mod->setValue(ui->doubleSpinBox->value());
    mod->setTimes(ui->comboBox->currentIndex()+1);
    mod->setAll( !ui->groupBox->isChecked());
    if(ui->groupBox->isChecked())
        mod->setDweller(ui->lineEditMorador->getCurrentId());



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
       QMessageBox::warning(this, "Oops",QString("Não foi possivel salvar\n%1").arg(mod->lastError().text()));
    
}

void Editextratx::Load()
{
    if( m_mod == NULL)
      return;
    ui->CmbBxmotivo->setCurrentId(m_mod->getMotivo());

}

void Editextratx::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
extratx* Editextratx::GetSaved()
{
   return m_lastMod;

}

