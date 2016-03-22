#include "editphone.h"
#include "ui_editphone.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>

#define PORTEIRO_FUL_PATH "photo_image2.jpg"

Editphone::Editphone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editphone)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;

   // CriaTabela();

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    ui->lineEditNumero->setFocus();
}


Editphone::~Editphone()
{
    delete ui;
}
void Editphone::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editphone::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editphone::SetModel(Phone* mod)
{
   m_mod = mod;
   Load();
}

bool Editphone::CanSave()
{
/*	
    if(ui->lineEditCPF->text().trimmed().isEmpty()
            && ui->lineEditRG->text().trimmed().isEmpty()
            && ui->lineEditVisitante->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite alguma informação sobre o phonente!!");
        return false;
    }
	*/

    return true;
}

void Editphone::Save()
{

    if( !CanSave() )
        return;

    Phone* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Phone;

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

void Editphone::Load()
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

void Editphone::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Phone* Editphone::GetSaved()
{
   return m_lastMod;

}
