#include "editpreaut.h"
#include "ui_editpreaut.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include "Visitante.h"
#include "dweller.h"


Editpreaut::Editpreaut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editpreaut)
{
    ui->setupUi(this);

    if( QCoreApplication::instance()->arguments().contains("--black"))
    {
        /*QString qss  = qApp->styleSheet();
        qss += " QWidget {"\
               " background-color: black;"\
               " }";


        qApp->setStyleSheet(qss);
        */

        ui->topmenu->setVisible(false);
    }
    
    m_mod = NULL;
    m_lastMod = NULL;

    connect(ui->lineEditVisit, SIGNAL(found(int)), this, SLOT(found(int)));
    connect(ui->lineEditVisit, SIGNAL(notFound()), this, SLOT(notFound()));

    ui->lineEditVisit->setFocus();

    ui->lineEditVisit->setSelect("select rg || ' | ' || nome || ' | ' || cpf, id, nome, rg, cpf from visitante where removed <> true order by id");
    ui->lineEditVisit->Add(ui->lineEditRG);
    ui->lineEditVisit->Add(ui->lineEditCPF);


    ui->lineEditAuthorizer->setSelect("select d.name || ' | ' || a.numero || ' ' || t.name , d.id, d.name, a.numero, t.name "\
         "from dweller d "\
         "inner join tower t on t.id= d.tower "\
         "inner join ap a on a.id = d.ap  where d.removed = false");
    ui->lineEditAuthorizer->Add(ui->lineEditAP);
    ui->lineEditAuthorizer->Add(ui->lineEditTower);

//    ui->CmbBxvisit->setTable("visitante");
//    ui->CmbBxvisit->setField("Nome");
//    ui->CmbBxvisit->setCanAdd(true);
//    ui->CmbBxvisit->setUserName("dsm");
//    ui->CmbBxvisit->completer()->setFilterMode(Qt::MatchContains );
//    ui->CmbBxauthorizer->setTable("dweller");
//    ui->CmbBxauthorizer->setField("Name");
//    ui->CmbBxauthorizer->setCanAdd(false);
//    ui->CmbBxauthorizer->setUserName("dsm");
//    ui->CmbBxauthorizer->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxreason->setTable("reason");
    ui->CmbBxreason->setField("Description");
    ui->CmbBxreason->setCanAdd(true);
    ui->CmbBxreason->setUserName("dsm");
    ui->CmbBxreason->completer()->setFilterMode(Qt::MatchContains );

    ui->CmbBxDestination->setTable("destination");
    ui->CmbBxDestination->setField("Description");
    ui->CmbBxDestination->setCanAdd(true);
    ui->CmbBxDestination->setUserName("dsm");
    ui->CmbBxDestination->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));

//    connect(ui->CmbBxvisit, SIGNAL(activated(int)), this, SLOT(visitActivated(int)));
//    connect(ui->CmbBxauthorizer, SIGNAL(activated(int)), this, SLOT(AutActivated(int)));


    ui->DtEdtautsince->setDate(QDate::currentDate());
    ui->DtEdtvalidate->setDate(QDate::currentDate().addMonths(3));

    connect(ui->lineEditVisit, SIGNAL(editingFinished()),this, SLOT(EditRGFinished()));


    /*
    QTime time;

    time.fromString("17:00", "HH:MM");
    ui->TmEdthorafim->setTime(time);
    time.fromString("08:00", "HH:MM");
    ui->TmEdthoraini->setTime(time);
    */
}

Editpreaut::~Editpreaut()
{
    delete ui;
}
void Editpreaut::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editpreaut::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editpreaut::SetModel(preaut* mod)
{
   m_mod = mod;
   Load();
}


void Editpreaut::Save()
{
    QString visit  = ui->lineEditVisit->text();

//    if( 1 >  ui->CmbBxvisit->model()->data(ui->CmbBxvisit->model()->index(ui->CmbBxvisit->currentIndex(), 0)).toInt())
    if( visit.trimmed().isEmpty() )
    {
        QMessageBox::warning(this, "Oops!","Informe o nome do visitante!");
        ui->lineEditVisit->setFocus();
        return;
    }

    preaut* mod =  m_mod;
    if( m_mod == NULL )
        mod = new preaut;

    mod->setsunday( ui->ChkBxsunday->isChecked() );
    mod->setmonday( ui->ChkBxmonday->isChecked() );
    mod->settuesday( ui->ChkBxtuesday->isChecked()) ;
    mod->setwednesday( ui->ChkBxwednesday->isChecked() );
    mod->setthursday( ui->ChkBxthursday->isChecked() );
    mod->setfriday( ui->ChkBxfriday->isChecked() );
    mod->setsaturday( ui->ChkBxsaturday->isChecked() );

    mod->setvisit(ui->lineEditVisit->getCurrentId());
//    mod->setvisit(ui->CmbBxvisit->model()->data(ui->CmbBxvisit->model()->index(ui->CmbBxvisit->currentIndex(), 0)).toInt());

    mod->sethoraini(ui->TmEdthoraini->time());
    mod->sethorafim(ui->TmEdthorafim->time());
    mod->setvalidate(ui->DtEdtvalidate->date());
    mod->setauthorizer(ui->lineEditAuthorizer->getCurrentId());
//    mod->setauthorizer(ui->CmbBxauthorizer->model()->data(ui->CmbBxauthorizer->model()->index(ui->CmbBxauthorizer->currentIndex(), 0)).toInt());

    mod->setobs(ui->textEdit->toPlainText());
    mod->setautsince(ui->DtEdtautsince->date());
    mod->setreason(ui->CmbBxreason->model()->data(ui->CmbBxreason->model()->index(ui->CmbBxreason->currentIndex(), 0)).toInt());
    mod->setdestination(ui->CmbBxDestination->model()->data(ui->CmbBxDestination->model()->index(ui->CmbBxDestination->currentIndex(), 0)).toInt());

    bool bRet = mod->Save();
    if( m_lastMod )
       delete m_lastMod;
    m_lastMod = mod;
    m_mod = NULL;
    if( bRet )
    {
       Visitante *visitante = Visitante::findByid(mod->getvisit());
       if(visitante)
       {
          visitante->setRG(ui->lineEditRG->text().remove(".").remove("-"));
          visitante->setCPF(ui->lineEditCPF->text().remove(".").remove("-"));
          if( visitante->getRG().isEmpty() )
             visitante->setRG(" ");
          if( visitante->getCPF().isEmpty() )
             visitante->setCPF(" ");

          visitante->Save();
          delete visitante;
       }

       QMessageBox::information(this, "Sucesso!","Informações foram salvas com sucesso!");
       accept();
    }
    else
       QMessageBox::warning(this, "Oops","Não foi possivel salvar");
    
}

void Editpreaut::Load()
{
    if( m_mod == NULL)
      return;
    ui->ChkBxsunday->setChecked(m_mod->getsunday());
    ui->ChkBxmonday->setChecked(m_mod->getmonday());
    ui->ChkBxtuesday->setChecked(m_mod->gettuesday());
    ui->ChkBxwednesday->setChecked(m_mod->getwednesday());
    ui->ChkBxthursday->setChecked(m_mod->getthursday());
    ui->ChkBxfriday->setChecked(m_mod->getfriday());
    ui->ChkBxsaturday->setChecked(m_mod->getsaturday());

//    ui->CmbBxvisit->setCurrentId(m_mod->getvisit());
    ui->TmEdthoraini->setTime(m_mod->gethoraini());
    ui->TmEdthorafim->setTime(m_mod->gethorafim());
    ui->DtEdtvalidate->setDate(m_mod->getvalidate());
//    ui->CmbBxauthorizer->setCurrentId(m_mod->getauthorizer());
    ui->textEdit->setText(m_mod->getobs());
    ui->DtEdtautsince->setDate(m_mod->getautsince());
    ui->CmbBxreason->setCurrentId(m_mod->getreason());
    ui->CmbBxDestination->setCurrentId(m_mod->getdestination());

    Visitante *visitante = Visitante::findByid(m_mod->getvisit());
    if( visitante )
    {
       ui->lineEditVisit->setCurrentId(m_mod->getvisit());
       ui->lineEditRG->setText( visitante->getRG() );
       ui->lineEditCPF->setText( visitante->getCPF() );
       ui->lineEditVisit->setText( visitante->getNome() );
       delete visitante;
    }
    Dweller *pDw = Dweller::findByid(m_mod->getauthorizer());
    if( pDw )
    {
        ui->lineEditAuthorizer->setCurrentId(m_mod->getauthorizer());
        Ap *ap = Ap::findByid(pDw->getap());
        if( ap )
        {
            ui->lineEditAP->setText(ap->getNumber());
            delete ap;
        }
        Tower *tw = Tower::findByid(pDw->gettower());
        if( tw )
        {
            ui->lineEditTower->setText(tw->getName());
            delete tw;
        }
        ui->lineEditAuthorizer->setText(pDw->getName());
        delete pDw;
    }


}

void Editpreaut::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
preaut* Editpreaut::GetSaved()
{
   return m_lastMod;

}

void Editpreaut::visitActivated(int)
{
    /*
  int id = ui->CmbBxvisit->model()->data(ui->CmbBxvisit->model()->index(ui->CmbBxvisit->currentIndex(), 0)).toInt()    ;
  Visitante *vis = Visitante::findByid(id);
  if( vis )
  {
      /// pegar telefone depois.
      ui->lineEditRG->setText(vis->getRG());
      ui->lineEditCPF->setText(vis->getCPF());
      QPixmap mypix = vis->getImage();
      ui->LblPhoto->setPixmap(mypix);
      delete vis;
  }
  */
}
void Editpreaut::AutActivated(int)
{
/*
  int id = ui->CmbBxauthorizer->model()->data(ui->CmbBxauthorizer->model()->index(ui->CmbBxauthorizer->currentIndex(), 0)).toInt()    ;
  Dweller *dw = Dweller::findByid(id);
  if( dw )
  {
      /// pegar telefone depois.
      /// pegar torre depois.
      /// pegar ap depois.
      //ui->lineEdit(vis->getCPF());
      QPixmap mypix = dw->getImage();
      ui->LblPhotoAut->setPixmap(mypix);
      delete dw;
  }
*/
}

void Editpreaut::found( int id )
{
    Visitante *pVis = Visitante::findByid(id);
    if( pVis )
    {
          QPixmap mypix = pVis->getImage();
          ui->LblPhoto->setPixmap(mypix);
          ui->lineEditCPF->setEnabled(false);
          ui->lineEditRG->setEnabled(false);
          delete pVis;
    }
}

void Editpreaut::notFound()
{
    //m_idVisitante = 0;
    QPixmap mypix;// = pVis->getImage();
    ui->LblPhoto->setPixmap(mypix);

    ui->lineEditCPF->setEnabled(true);
    ui->lineEditRG->setEnabled(true);
}
