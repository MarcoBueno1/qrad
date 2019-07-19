#include "editvisit.h"
#include "ui_editvisit.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QShortcut>
#include "camera.h"
#include "Visitante.h"
#include "dweller.h"
#include "ap.h"
#include "tower.h"
#include "preaut.h"
#include <windows.h>

// alter table visit add column company integer;

#define PORTEIRO_FUL_PATH "visit.jpg"


void Editvisit::RemovePhotoFile()
{
    QRAD_IN()
    if(QFile::exists(PORTEIRO_FUL_PATH))
    {
        debug_message("Removendo arquivo Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());

        QFile *remove= new QFile(PORTEIRO_FUL_PATH);

        bool bRes = remove->remove();
        if( !bRes )
        {
            debug_message("Erro ao remover arquivo Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());
        }
        delete remove;
    }
    QRAD_OUT()
}


Editvisit::Editvisit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editvisit)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    m_idVisitante = 0;
    m_foto = 0;
    m_CurrentTickCount = 0;
    m_bfound = false;
   // m_CurrentVisitanteId = 0;
   // CriaTabela();

    RemovePhotoFile();

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonBaterFoto, SIGNAL(clicked()),this, SLOT(baterFoto()));
    connect(ui->lineEditRG, SIGNAL(found(int)), this, SLOT(found(int)));
    connect(ui->lineEditRG, SIGNAL(notFound()), this, SLOT(notFound()));

    ui->lineEditPreAutorizado->setReadOnly(true);
    ui->lineEditAnunciarChegada->setReadOnly(true);

    ui->DtEdtData->setDate(QDate::currentDate());
    ui->TmEdtHora->setTime(QTime::currentTime());
    ui->lineEditRG->setFocus();

    ui->lineEditRG->setSelect("select rg || ' | ' || nome || ' | ' || cpf, id, rg, cpf, nome from visitante where removed <> true order by id");
    ui->lineEditRG->Add(ui->lineEditCPF);
    ui->lineEditRG->Add(ui->lineEditVisitante);


    //ui->lineEditMorador->setSelect("select d.name || ' | ' || t.name || ' | ' || a.numero, d.id, d.name, a.numero, t.name, d.ramal "
    ui->lineEditMorador->setSelect("select d.name || ' | ' || a.numero || ' ' || t.name , d.id, d.name, a.numero, t.name, d.ramal "\
         "from dweller d "\
         "inner join tower t on t.id= d.tower "\
         "inner join ap a on a.id = d.ap  where d.removed = false");
    ui->lineEditMorador->Add(ui->lineEditAP);
    ui->lineEditMorador->Add(ui->lineEditTorre);
    ui->lineEditMorador->Add(ui->lineEditRamal);


    ui->comboBoxReason->setTable("reason");
    ui->comboBoxReason->setField("description");
    ui->comboBoxReason->setPermission("usevisit");
    ui->comboBoxReason->setCanAdd(true);
    ui->comboBoxReason->setUserName("dsm");
    if( ui->comboBoxReason->completer() )
        ui->comboBoxReason->completer()->setFilterMode(Qt::MatchContains );


    ui->comboBoxCompany->setTable("company.Empresa");
    ui->comboBoxCompany->setField("name.Nome");
    ui->comboBoxCompany->setPermission("usevisit");
    ui->comboBoxCompany->setCanAdd(true);
    ui->comboBoxCompany->setUserName("dsm");
    if( ui->comboBoxCompany->completer() )
        ui->comboBoxCompany->completer()->setFilterMode(Qt::MatchContains );


    m_deliveryto = new QSqlQueryModel;
    m_deliveryto->setQuery(SQL_SELECT_CLIENT_COMBO);
    ui->comboBoxDeliveryTo->setModel(m_deliveryto);
    ui->comboBoxDeliveryTo->setModelColumn(1);
    if(ui->comboBoxDeliveryTo->completer())
        ui->comboBoxDeliveryTo->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->groupBoxDelivery, SIGNAL(clicked(bool)), this, SLOT(DeliveryCheck(bool)));
    connect(ui->lineEditRG, SIGNAL(editingFinished()),this, SLOT(EditRGFinished()));


    ui->groupBoxDelivery->setVisible(false);

    ui->groupBoxSaida->setVisible(false);

    /*
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowContextHelpButtonHint;
    setWindowFlags( flags );
    */
}


Editvisit::~Editvisit()
{
    delete m_deliveryto;
    delete ui;
}
void Editvisit::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
}
void Editvisit::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editvisit::SetModel(visit* mod)
{
   m_mod = mod;
   Load();
}

bool Editvisit::CanSave()
{
    QString CPF  = ui->lineEditCPF->text();
    QString RG   = ui->lineEditRG->text();
    QString Nome = ui->lineEditVisitante->text();

    CPF = CPF.remove(".").remove("-").trimmed();


    if( Nome.trimmed().isEmpty() )
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite o Nome do Visitante!!");
        ui->lineEditVisitante->selectAll();
        ui->lineEditVisitante->setFocus();
        return false;
    }

    if(CPF.trimmed().isEmpty()
            && RG.trimmed().isEmpty()
            && Nome.trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite alguma informação sobre o Visitnte!!");
        ui->lineEditVisitante->selectAll();
        ui->lineEditVisitante->setFocus();
        return false;
    }
    if( ui->lineEditTorre->text().trimmed().isEmpty() )
    {
        QMessageBox::information(this, "Oops!", "Por favor, informe dados do Morador que autorizou!!");
        ui->lineEditMorador->selectAll();
        ui->lineEditMorador->setFocus();
        return false;
    }

    return true;
}


void Editvisit::Save()
{
  QRAD_IN()

  if( !CanSave() )
  {
       QRAD_OUT()
       return;
  }

    Visitante *pVis = Visitante::findByid(m_idVisitante);
    if(!pVis)
    {
        if(!ui->lineEditRG->text().trimmed().isEmpty())
        {
           VisitanteList *pVLst = Visitante::findBy(QString("select * from visitante where rg='%1' and removed <> true order by id limit 1")
                                    .arg(ui->lineEditRG->text().trimmed()));
           if( pVLst && pVLst->count())
           {
               debug_message("Visitante já cadastrado, encontrados:%d com o RG:%s\n",
                             pVLst->count(),
                             ui->lineEditRG->text().trimmed().toLatin1().data());
               pVis = new Visitante;
               pVis->copyFrom(pVLst->at(0));
           }
        }

        if( !pVis && (!ui->lineEditCPF->text().trimmed().isEmpty()))
        {
            VisitanteList *pVLst = Visitante::findBy(QString("select * from visitante where cpf='%1' and removed <> true  order by id limit 1")
                                     .arg(ui->lineEditCPF->text().trimmed()));
            if( pVLst && pVLst->count())
            {
                debug_message("Visitante já cadastrado, encontrados:%d com o CPF:%s\n",
                              pVLst->count(),
                              ui->lineEditCPF->text().trimmed().toLatin1().data());

                pVis = new Visitante;
                pVis->copyFrom(pVLst->at(0));
            }
        }        
    }
    if(!pVis)
    {
        debug_message("Visitante NAO cadastrado, cadastrando: RG:%s CPF:%s Nome:%s\n",
                      ui->lineEditRG->text().trimmed().toLatin1().data(),
                      ui->lineEditCPF->text().trimmed().toLatin1().data(),
                      ui->lineEditVisitante->text().toLatin1().data());

        pVis = new Visitante;
        pVis->setRG(ui->lineEditRG->text().trimmed());
        QString CPF =ui->lineEditCPF->text().trimmed();
        CPF.remove("-").remove(".");
        pVis->setCPF(CPF);
        pVis->setNome(ui->lineEditVisitante->text());
        if( m_foto && QFile::exists(PORTEIRO_FUL_PATH) )
        {
            debug_message("Salvando arquivo Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());

            bool bSave = pVis->saveImage(PORTEIRO_FUL_PATH);
            if( !bSave )
            {
                debug_message("Erro ao Salvar arquivo Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());
            }
        }
        pVis->Save();
        debug_message("Novo Visitante id:%d\n", pVis->getid())
    }
    else // atualizar dados do visitante
    {
        bool bSave = false;
        if( pVis->getRG().isEmpty() && !ui->lineEditRG->text().trimmed().isEmpty())
        {
            debug_message("Alterando RG do visitante id:%d para RG:%s\n", pVis->getid(), ui->lineEditRG->text().trimmed().toLatin1().data());
            pVis->setRG(ui->lineEditRG->text().trimmed());
            bSave = true;
        }
        QString CPF =ui->lineEditCPF->text().trimmed();
        CPF.remove("-").remove(".");
        if( pVis->getCPF().isEmpty() && !CPF.trimmed().isEmpty())
        {
             debug_message("Alterando CPF do visitante id:%d para CPF:%s\n", pVis->getid(), CPF.toLatin1().data());
             pVis->setCPF(CPF);
             bSave = true;
        }
        if( pVis->getNome().length() < ui->lineEditVisitante->text().trimmed().length())
        {
             debug_message("Alterando Nome do visitante id:%d para Nome:%s\n", pVis->getid(), ui->lineEditVisitante->text().toLatin1().data());
             pVis->setNome(ui->lineEditVisitante->text().trimmed());
             bSave = true;
        }
        if( m_foto )
        {
            debug_message("Alterando Foto do visitante id:%d para Md5:%s\n", pVis->getid(), Model::fileChecksum(PORTEIRO_FUL_PATH).data());

            bSave = pVis->saveImage(PORTEIRO_FUL_PATH);
            if( !bSave )
            {
                debug_message("Erro ao Salvar arquivo Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());
            }
            bSave = true;
        }
        if( bSave )
        {
            bool bResult = pVis->Save();
            debug_message("Salvando visitante id:%d bResult:%s\n", pVis->getid(), bResult?"true":"false");
        }

        debug_message("Visitante já existente:%d\n", pVis->getid());
    }

    visit* mod =  m_mod;
    if( m_mod == NULL)
        mod = new visit;

    mod->setData(ui->DtEdtData->date());
    mod->setHora(ui->TmEdtHora->time());
    mod->setVisitante(pVis->getid());
    delete pVis;

    mod->setSaida(ui->DtEdtSaida->date());
    mod->setsaida_hora(ui->TmEdtsaida_hora->time());
    mod->setReason(ui->comboBoxReason->model()->data(ui->comboBoxReason->model()->index(ui->comboBoxReason->currentIndex(), 0)).toInt());
    mod->setautorizador(ui->lineEditMorador->getCurrentId());

    bool bRet = mod->Save();
    if( m_lastMod )
       delete m_lastMod;
    m_lastMod = mod;
    m_mod = NULL;
    if( bRet )
    {
       debug_message("Visita salva com sucesso, id:%d\n", mod->getid());

       QMessageBox::information(this, "Sucesso!","Informações foram salvas com sucesso!");
       RemovePhotoFile();
       accept();
    }
    else
    {
       debug_message("Problema ao salvar visita, id:%d\n", mod->getid());

       QMessageBox::warning(this, "Oops","Não foi possivel salvar");
    }

    QRAD_OUT()
}

void Editvisit::Load()
{
    if( m_mod == NULL)
      return;
    /// inicialmente tratar edicao como sendo saida
    ///  neste caso desabilitar todos os campos que foram preenchidos na entrada
    ///  para evitar erro operacional
   ui->DtEdtData->setEnabled(false);
   ui->TmEdtHora->setEnabled(false);
   ui->comboBoxReason->setEnabled(false);
   ui->comboBoxCompany->setEnabled(false);
   ui->groupBoxEntrada->setEnabled(false);
   ui->lineEditRG->setEnabled(false);
   ui->lineEditCPF->setEnabled(false);
   ui->lineEditVisitante->setEnabled(false);
   ui->lineEditMorador->setEnabled(false);
   ui->lineEditAP->setEnabled(false);
   ui->lineEditTorre->setEnabled(false);
   ui->lineEditRamal->setEnabled(false);
   if( m_mod->getType() == visNormal )
   {
       ui->labelStatus->setText("Finalizar Vizita");
       ui->PshBtnSave->setText("C&oncluir");
   }
   else
   {
       ui->labelStatus->setText("Finalizar Entrega do Documento/Objeto");
       ui->PshBtnSave->setText("C&oncluir");

   }
   QShortcut *shortcut = new QShortcut(QKeySequence("Alt+o"), this);
   QObject::connect(shortcut, SIGNAL(activated()), ui->PshBtnSave, SLOT(click()));

   ui->pushButtonBaterFoto->setEnabled(false);

    ui->DtEdtData->setDate(m_mod->getData());
    ui->TmEdtHora->setTime(m_mod->getHora());
//    ui->CmbBxVisitante->setCurrentId();
    //ui->DtEdtSaida->setDate(m_mod->getSaida());
   // ui->TmEdtsaida_hora->setTime(m_mod->getsaida_hora());

   // ui->DtEdtData->setReadOnly(true);
   // ui->TmEdtHora->setReadOnly(true);
    ui->comboBoxReason->setCurrentId(m_mod->getReason());

    ui->groupBoxSaida->setVisible(true);
    ui->DtEdtSaida->setDate(QDate::currentDate());
    ui->TmEdtsaida_hora->setTime(QTime::currentTime());

    Visitante *pVis = Visitante::findByid(m_mod->getVisitante());
    if( pVis )
    {
          ui->lineEditRG->setText(pVis->getRG());
          ui->lineEditCPF->setText(pVis->getCPF());
          ui->lineEditVisitante->setText(pVis->getNome());
          if( pVis->getLoId() )
          {
              QPixmap mypix = pVis->getImage();
              ui->LblPhoto->setPixmap(mypix);
          }
          else
          {
              debug_message("Visitante id: %d não tem imagem associada\n", pVis->getid())
          }
          delete pVis;
    }
    Dweller *dw = Dweller::findByid(m_mod->getautorizador());
    if( dw )
    {
        ui->lineEditMorador->setCurrentId(m_mod->getautorizador());
/*        ui->lineEditMorador->setText(dw->getName());

        Ap *ap = Ap::findByid(dw->getAp());
        debug_message("dw->getAp(): %d\n", dw->getAp());
        if( ap )
        {
           debug_message("Ap: %s\n", ap->getNumber().toLatin1().data());
           ui->lineEditAP->setText(ap->getNumber());
           delete ap;
        }
        Tower *t = Tower::findByid(dw->getTower());
        if( t )
        {
            ui->lineEditTorre->setText(t->getName());
            delete t;
        }
        ui->lineEditRamal->setText(dw->getRamal());*/
        delete dw;
    }

}

void Editvisit::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
visit* Editvisit::GetSaved()
{
   return m_lastMod;

}

void Editvisit::baterFoto()
{
  QRAD_IN()

  Camera *cam = new Camera;

  debug_message("Antes de bater foto Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());

  cam->setPath(PORTEIRO_FUL_PATH);

  if( QDialog::Accepted == cam->exec())
  {
      debug_message("Nova foto Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());

      QImage myImage ;
      myImage.load(PORTEIRO_FUL_PATH);
      ui->LblPhoto->setPixmap(QPixmap::fromImage(myImage));
      ui->PshBtnSave->setFocus(Qt::TabFocusReason);
      m_foto = 1;
  }
  else
  {
      debug_message("Usuario cancelou a foto Md5:%s\n", Model::fileChecksum(PORTEIRO_FUL_PATH).data());
  }
  delete cam;
  ui->lineEditRG->setFocus();

  QRAD_OUT()

}

void Editvisit::found( int id, bool force )
{
   if( m_bfound && !force)
       return;

   m_bfound = true;

    Visitante *pVis = Visitante::findByid(id);
    if( pVis )
    {
         // QMessageBox::information(this,"Edit found", "Edit found");
          QPixmap mypix = pVis->getImage();
          ui->LblPhoto->setPixmap(mypix);
          ui->lineEditCPF->setEnabled(false);
          ui->lineEditRG->setEnabled(false);
          ui->lineEditVisitante->setEnabled(false);

          if(force)
          {
              if( !pVis->getCPF().trimmed().isEmpty())
                  ui->lineEditCPF->setText(pVis->getNome());
              if( !pVis->getNome().trimmed().isEmpty())
                  ui->lineEditVisitante->setText(pVis->getNome());
          }

          preaut *pre = preaut::findValidByVisitor(id);
          if( pre )
          {
              ui->lineEditPreAutorizado->setText("SIM");
              QString styleSheet = QString("QLineEdit{%1 %2}")
                                   .arg("color: white;"/*FG_COLOR_NORMAL*/) //
                                   .arg("background: green;"); //BG_COLOR_NORMAL

              ui->lineEditPreAutorizado->setStyleSheet(styleSheet);

              if(pre->getNoAnnounce() == true)
              {
                  ui->lineEditAnunciarChegada->setText("NÃO");
                  ui->lineEditAnunciarChegada->setStyleSheet(styleSheet);
              }

              //// dados do autorizador
              Dweller *Dw = Dweller::findByid(pre->getauthorizer());
              if( Dw )
              {
                  ui->lineEditMorador->setCurrentId(pre->getauthorizer());
                 /* ui->lineEditMorador->setText(Dw->getName());
                  Ap *ap = Ap::findByid(Dw->getAp());
                  if( ap )
                  {
                      ui->lineEditAP->setText(ap->getNumber());
                      delete ap;
                  }
                  Tower *tw = Tower::findByid(Dw->getTower());
                  if( tw )
                  {
                      ui->lineEditTorre->setText(tw->getName());
                      delete tw;
                  }
                  ui->lineEditRamal->setText(Dw->getRamal());
                  delete Dw;*/
              }

              ui->comboBoxReason->setCurrentId( pre->getreason() );
              ui->pushButtonBaterFoto->setFocus();

              delete pre;
          }
          else
          {
              ui->lineEditPreAutorizado->setText("NÃO");
              QString styleSheet = QString("QLineEdit{%1 %2}")
                                   .arg(FG_COLOR_NORMAL) //
                                   .arg(BG_COLOR_NORMAL);

              ui->lineEditPreAutorizado->setStyleSheet(styleSheet);
              ui->lineEditAnunciarChegada->setText("SIM");
              ui->lineEditAnunciarChegada->setStyleSheet(styleSheet);
              ui->DtEdtData->setFocus();
          }
          m_idVisitante = id;
          delete pVis;
    }
    else
    {
       // QMessageBox::information(this,"Edit found", QString("Edit found, not found id: %1").arg(id));

    }
}
void Editvisit::notFound()
{
    m_idVisitante = 0;
    m_bfound = false;
    QPixmap mypix;// = pVis->getImage();
    ui->LblPhoto->setPixmap(mypix);

    ui->lineEditPreAutorizado->setText("NÃO");
    QString styleSheet = QString("QLineEdit{%1 %2}")
                         .arg(FG_COLOR_NORMAL) //
                         .arg(BG_COLOR_NORMAL);

    ui->lineEditPreAutorizado->setStyleSheet(styleSheet);
    ui->lineEditAnunciarChegada->setText("SIM");
    ui->lineEditAnunciarChegada->setStyleSheet(styleSheet);
//    delete pVis;
}

void Editvisit::DeliveryCheck(bool bChecked)
{
    ui->groupBoxAutorizadoPor->setEnabled(!bChecked );
}
void Editvisit::EditRGFinished()
{
   int Elapsed =  GetTickCount() - m_CurrentTickCount;
   m_CurrentTickCount = GetTickCount();
   if(  (Elapsed >  1000) && !m_bfound)
   {
       m_bfound = true;
//       QMessageBox::information(this,"Edit RG Finished", "Edit Rg Finished: Completer esta invisivel");
       //return;

       VisitanteList *visitantes = NULL;
       if( !ui->lineEditRG->text().trimmed().isEmpty())
       {
          visitantes = Visitante::findBy(QString("select * from visitante where rg = '%1' and removed <> true order by id limit 1").arg(ui->lineEditRG->text().trimmed()));
       }
       else if( !ui->lineEditCPF->text().remove("-").remove(".").trimmed().isEmpty())
       {
           visitantes = Visitante::findBy(QString("select * from visitante where cpf = '%1' and removed <> true order by id limit 1").arg(ui->lineEditRG->text().trimmed()));
       }
       if( visitantes && visitantes->count())
       {

          found(visitantes->at(0)->getid(), true);
       }
    // codigo de procura exata pelo RG.
    // QMessageBox::information(this,"Edit RG Finished", "Edit Rg Finished");
       //m_bfound = true;
   }
}
