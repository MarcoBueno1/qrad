#include "neweditinvited.h"
#include "ui_neweditinvited.h"
#include <QMessageBox>

NewEditInvited::NewEditInvited(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewEditInvited)
{
    ui->setupUi(this);
    connect( ui->pushButtonCancel, SIGNAL(pressed()),this,SLOT(reject()));
    connect( ui->pushButtonSave, SIGNAL(pressed()),this,SLOT(Save()));

    m_invited = NULL;
}

NewEditInvited::~NewEditInvited()
{
    delete ui;
}


void NewEditInvited::Save()
{
   if( ui->lineEditName->text().trimmed().isEmpty() )
   {
       QMessageBox::warning(this,"Oops!", "Por favor preencha o nome!");
       ui->lineEditName->setFocus();
       return;
   }

   if( ui->lineEditCPF->text().trimmed().isEmpty()
       && (QMessageBox::No == QMessageBox::question(this, "Atenção", "Deseja deixar o campo CPF / RG vazio?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
     )
   {
       ui->lineEditCPF->setFocus();
       return;
   }
   if( !m_invited )
       m_invited = new Invited;

   m_invited->setName(ui->lineEditName->text().trimmed());
   m_invited->setDocumment(ui->lineEditCPF->text().trimmed());
   m_invited->setReserve(m_reserveId);

   if( !m_invited->Save() )
   {
       QMessageBox::warning(this, "Oops!", QString("Erro: %1").arg(m_invited->lastError().text()));
       return;
   }
   delete m_invited;
   accept();
}

int NewEditInvited::Exec( int reserveId, int InvitedId )
{
    m_reserveId = reserveId;
    if( InvitedId )
    {
        m_invited = Invited::findById(InvitedId,true);
        if( m_invited )
        {
            ui->lineEditName->setText(m_invited->getName());
            ui->lineEditCPF->setText(m_invited->getDocumment());
        }
    }
    return exec();
}
