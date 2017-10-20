#include "editcommon_area.h"
#include "ui_editcommon_area.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editcommon_area::Editcommon_area(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editcommon_area)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editcommon_area::~Editcommon_area()
{
    delete ui;
}
void Editcommon_area::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editcommon_area::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editcommon_area::SetModel(common_area* mod)
{
   m_mod = mod;
   Load();
}


void Editcommon_area::Save()
{
    if( ui->LnEdtname->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Oops","Por favor, informe um nome para este local");
        ui->LnEdtname->setFocus();
        return;
    }

    common_area* mod =  m_mod;
    if( m_mod == NULL)
        mod = new common_area;

    mod->setname(ui->LnEdtname->text());
    mod->setdescription(ui->textEdit->toPlainText());
    mod->setinterval(ui->comboBox->currentIndex()?0:1); // invertido
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

void Editcommon_area::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtname->setText(m_mod->getname());
    ui->textEdit->setText(m_mod->getdescription());
    if( m_mod->getinterval() == 0 )
        ui->comboBox->setCurrentIndex(1);
    else
        ui->comboBox->setCurrentIndex(0);

}

void Editcommon_area::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
common_area* Editcommon_area::GetSaved()
{
   return m_lastMod;

}

