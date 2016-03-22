#include "editvisit.h"
#include "ui_editvisit.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include "camera.h"
#include "Visitante.h"

#define PORTEIRO_FUL_PATH "photo_image2.jpg"

Editvisit::Editvisit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editvisit)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    m_idVisitante = 0;

   // CriaTabela();

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonBaterFoto, SIGNAL(clicked()),this, SLOT(baterFoto()));
    ui->DtEdtData->setDate(QDate::currentDate());
    ui->TmEdtHora->setTime(QTime::currentTime());
    ui->lineEditRG->setFocus();

    ui->lineEditRG->setSelect("select rg || ' | ' || nome, id, rg, cpf, nome from Visitante");
    ui->lineEditRG->Add(ui->lineEditCPF);
    ui->lineEditRG->Add(ui->lineEditVisitante);


    ui->lineEditMorador->setSelect("select nome || ' | ' || torre || ' | ' || ap, id, nome, ap, torre, ramal from morador");
    ui->lineEditMorador->Add(ui->lineEditAP);
    ui->lineEditMorador->Add(ui->lineEditTorre);
    ui->lineEditMorador->Add(ui->lineEditRamal);

}


Editvisit::~Editvisit()
{
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
    if(ui->lineEditCPF->text().trimmed().isEmpty()
            && ui->lineEditRG->text().trimmed().isEmpty()
            && ui->lineEditVisitante->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite alguma informação sobre o visitnte!!");
        return false;
    }

    return true;
}

void Editvisit::Save()
{

    if( !CanSave() )
        return;

    Visitante *pVis = Visitante::findByid(ui->lineEditRG->getCurrentId());
    if(!pVis)
    {
        pVis = new Visitante;
        pVis->setRG(ui->lineEditRG->text());
        pVis->setCPF(ui->lineEditCPF->text());
        pVis->setNome(ui->lineEditVisitante->text());
        pVis->saveImage("");
        pVis->Save();
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

void Editvisit::Load()
{
    if( m_mod == NULL)
      return;
    ui->DtEdtData->setDate(m_mod->getData());
    ui->TmEdtHora->setTime(m_mod->getHora());
//    ui->CmbBxVisitante->setCurrentId();
    ui->DtEdtSaida->setDate(m_mod->getSaida());
    ui->TmEdtsaida_hora->setTime(m_mod->getsaida_hora());

    ui->DtEdtData->setReadOnly(true);
    ui->TmEdtHora->setReadOnly(true);

    Visitante *pVis = Visitante::findByid(m_mod->getVisitante());
    if( pVis )
    {
          ui->lineEditRG->setText(pVis->getRG());
          ui->lineEditCPF->setText(pVis->getCPF());
          ui->lineEditVisitante->setText(pVis->getNome());
          delete pVis;
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
  Camera *cam = new Camera;

  cam->setPath(PORTEIRO_FUL_PATH);
  if( QDialog::Accepted == cam->exec())
  {
      QImage myImage ;
      myImage.load(PORTEIRO_FUL_PATH);
      ui->LblPhoto->setPixmap(QPixmap::fromImage(myImage));
      ui->PshBtnSave->setFocus(Qt::TabFocusReason);
  }
  delete cam;
}

void Editvisit::CriaTabela()
{
    QString str = "create table teste ( id integer, rg character variyng, cpf character variyng, nome character variyng);";
    QSqlQuery *q = new QSqlQuery;
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 1,'1054264393', '62040863087','Marco Antonio Bueno da Silva' )";
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 2,'1055001001', '52014339287', 'Tricia Bueno da Silva' )";
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 3,'1100121231', '12345678901','Miguel Bueno da Silva' )";
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 4,'1234567890', '00000000001','Theo Bueno da Silva' )";
    q->exec(str);


    str = "create table morador ( id integer, ap character variyng, torre character variyng, nome character variyng, ramal character variyng);";
    q->exec(str);

    str = "insert into morador ( id, ap, torre, nome, ramal ) values ( 1,'104', 'Olympic','Marco Antonio Bueno da Silva', '1104' )";
    q->exec(str);

    str = "insert into morador ( id, ap, torre, nome, ramal ) values ( 2,'103', 'Marine', 'Tricia Bueno da Silva', '2103' )";
    q->exec(str);

    str = "insert into morador ( id, ap, torre, nome, ramal ) values ( 3,'102', 'Green','Miguel Bueno da Silva', '3102' )";
    q->exec(str);

    str = "insert into morador ( id, ap, torre, nome, ramal ) values ( 4,'101', 'Olympic','Theo Bueno da Silva', '1101' )";
    q->exec(str);





    delete q;
}
