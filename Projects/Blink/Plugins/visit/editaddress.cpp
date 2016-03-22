#include "editaddress.h"
#include "ui_editaddress.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>

#define PORTEIRO_FUL_PATH "photo_image2.jpg"

Editaddress::Editaddress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editaddress)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;

   // CriaTabela();

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    ui->lineEditNumero->setFocus();
}


Editaddress::~Editaddress()
{
    delete ui;
}
void Editaddress::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editaddress::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editaddress::SetModel(Address* mod)
{
   m_mod = mod;
   Load();
}

bool Editaddress::CanSave()
{
/*	
    if(ui->lineEditCPF->text().trimmed().isEmpty()
            && ui->lineEditRG->text().trimmed().isEmpty()
            && ui->lineEditVisitante->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite alguma informação sobre o addressnte!!");
        return false;
    }
	*/

    return true;
}

void Editaddress::Save()
{

    if( !CanSave() )
        return;

    Address* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Address;

//    mod->setData(ui->DtEdtData->date());
//    mod->setHora(ui->TmEdtHora->time());
//    mod->setSaida(ui->DtEdtSaida->date());
//    mod->setsaida_hora(ui->TmEdtsaida_hora->time());
	
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

void Editaddress::Load()
{
    if( m_mod == NULL)
      return;
/*    ui->DtEdtData->setDate(m_mod->getData());
    ui->TmEdtHora->setTime(m_mod->getHora());
    ui->DtEdtSaida->setDate(m_mod->getSaida());
    ui->TmEdtsaida_hora->setTime(m_mod->getsaida_hora());

    ui->DtEdtData->setReadOnly(true);
    ui->TmEdtHora->setReadOnly(true);
*/
}

void Editaddress::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Address* Editaddress::GetSaved()
{
   return m_lastMod;

}
