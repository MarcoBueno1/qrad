#include "editphone.h"
#include "ui_editphone.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editphone::Editphone(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editphone)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxoperator->setTable("Operadora");
    ui->CmbBxoperator->setField("Name");
    ui->CmbBxoperator->setCanAdd(true);
    ui->CmbBxoperator->setUserName("dsm");
    if(ui->CmbBxoperator->completer())
        ui->CmbBxoperator->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxtype->setTable("phonetype");
    ui->CmbBxtype->setField("type");
    ui->CmbBxtype->setCanAdd(true);
    ui->CmbBxtype->setUserName("dsm");
    if(ui->CmbBxtype->completer())
        ui->CmbBxtype->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
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


void Editphone::Save()
{
    QString number = ui->LnEdtnumber->text();

    number = number.remove("(").remove(")").remove("-");
    if( number.trimmed().length() < 10)
    {
        QMessageBox::warning(this, "Oops","Numero de telefone inválido, poucos números..");
        ui->LnEdtnumber->setFocus();
        return;
    }
    Phone* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Phone;

    //mod->setdwellerid(ui->SpnBxdwellerid->value());
    mod->setNumber(number);
    mod->setOperator(ui->CmbBxoperator->model()->data(ui->CmbBxoperator->model()->index(ui->CmbBxoperator->currentIndex(), 0)).toInt());

    mod->setType(ui->CmbBxtype->model()->data(ui->CmbBxtype->model()->index(ui->CmbBxtype->currentIndex(), 0)).toInt());

    mod->setWatsApp(ui->ChkBxwatsapp->isChecked());
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
    ui->LnEdtnumber->setText(m_mod->getNumber());
    ui->CmbBxoperator->setCurrentId(m_mod->getOperator());
    ui->CmbBxtype->setCurrentId(m_mod->getType());
    ui->ChkBxwatsapp->setChecked(m_mod->getWatsApp());

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

