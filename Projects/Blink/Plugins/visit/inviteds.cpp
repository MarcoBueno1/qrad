#include "inviteds.h"
#include "ui_inviteds.h"
#include "neweditinvited.h"
#include <QMessageBox>
#include "qraddebug.h"

#define SQL "select name as \"Nome\", documment as \"Documento\", id from invited where reserve = %1 and removed <> true order by name"

Inviteds::Inviteds(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Inviteds)
{
    ui->setupUi(this);

    m_reserveId = 0;
    m_model = new QSqlQueryModel;

    connect(ui->pushButtonNovo, SIGNAL(pressed()),this, SLOT(NewInvited()));
    connect(ui->pushButtonEditar, SIGNAL(pressed()),this, SLOT(EditInvited()));
    connect(ui->pushButtonRemover, SIGNAL(pressed()),this, SLOT(RemoveInvited()));

    setWindowTitle("Lista de Convidados");
}

Inviteds::~Inviteds()
{
    delete m_model;
    delete ui;
}

void Inviteds::NewInvited()
{
    NewEditInvited *nI = new NewEditInvited;

    if( QDialog::Accepted == nI->Exec(m_reserveId))
        UpdateTable();
    delete nI;
}

void Inviteds::EditInvited()
{
    int id = m_model->index(ui->tableView->currentIndex().row(),2).data().toInt();
    if( !id )
    {
        QMessageBox::information(this,"Oops!", "Selecione um item para editar!");
        ui->tableView->setFocus();
        return;
    }

    NewEditInvited *nI = new NewEditInvited;
    if( QDialog::Accepted == nI->Exec(m_reserveId,id))
        UpdateTable();

    delete nI;
}

void Inviteds::RemoveInvited()
{
    int id = m_model->index(ui->tableView->currentIndex().row(),2).data().toInt();
    if( !id )
    {
        QMessageBox::information(this,"Oops!", "Selecione um item para remover!");
        ui->tableView->setFocus();
        return;
    }
    Invited *Inv = Invited::findById(id, true);
    if( !Inv )
    {
        QMessageBox::information(this,"Oops!", "Selecione um item para remover!");
        ui->tableView->setFocus();
        return;
    }
    if( QMessageBox::No== QMessageBox::question(this, "Atenção!", "Deseja remover o item selecionado?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
    {
        delete Inv;
        return;
    }
    Inv->updateRemoved(true);
    delete Inv;
    UpdateTable();
}


int Inviteds::Exec( int reserveId )
{
    m_reserveId = reserveId;
    UpdateTable();

    return exec();
}
void Inviteds::UpdateTable()
{
     QString sql = QString(SQL).arg(m_reserveId);
     debug_message("SQL: %s\n", sql.toLatin1().data());

     m_model->setQuery(sql);
     ui->tableView->setModel(m_model);
     ui->tableView->hideColumn(2);

     for (int c = 0; c < ui->tableView->horizontalHeader()->count(); ++c)
     {
         if( ui->tableView->isColumnHidden(c) )
             continue;
         ui->tableView->horizontalHeader()->setSectionResizeMode(
             c, QHeaderView::Stretch);
     }
}
