#include "newdayreserve.h"
#include "ui_newdayreserve.h"
#include "common_area.h"
#include "reserve.h"
#include "dweller.h"
#include "ap.h"
#include "tower.h"
#include "qraddebug.h"
#include "qradshared.h"
#include <QCompleter>
#include "inviteds.h"

NewDayReserve::NewDayReserve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewDayReserve)
{
    ui->setupUi(this);

    m_reserve = NULL;



    m_litedb = QSqlDatabase::addDatabase("QSQLITE", "lite");

//    db.setDatabaseName("prolog.db");
    m_litedb.setDatabaseName(":memory:");
    m_litedb.setHostName("localhost");
    m_litedb.setPassword("1");
    m_litedb.setUserName("1");


    if(!m_litedb.open())
    {
        debug_message("Impossivel conectar no QSQLITE banco\n");
    }
    else
    {
        debug_message("Conectado ao QSQLITE!\n");
    }

    m_model= new QSqlQueryModel;
    m_modelCombo= new QSqlQueryModel;
    m_delegate = new ColumnReserve;

    ui->timeEditStart->setReadOnly(true);
    ui->timeEditEnd->setReadOnly(true);

    ui->timeEditStart->setToolTip("Para inserir horário de inicio e fim, selecione com o mouse a linha da tabela abaixo");
    ui->timeEditEnd->setToolTip("Para inserir horário de inicio e fim, selecione com o mouse a linha da tabela abaixo");

    connect(ui->pushButtonSave,SIGNAL(clicked(bool)),this,SLOT(Save(bool)));
    connect(ui->pushButtonInvited,SIGNAL(clicked(bool)),this,SLOT(DoInvited(bool)));
    ui->pushButtonInvited->setVisible(false);
}

NewDayReserve::~NewDayReserve()
{
    delete m_model;
    delete m_modelCombo;
    delete m_delegate;
    m_litedb.close();
    delete ui;
}

void NewDayReserve::DoInvited(bool)
{
    Inviteds *pi = new Inviteds;

    pi->Exec(m_reserve->getid());
}

void NewDayReserve::Save(bool)
{
  /// salvar
  if( ui->comboBox->currentIndex() < 0 )
  {
      QMessageBox::warning(this, "Oops!", "Por favor, selecione o rsponsável pela reserva!");
      ui->comboBox->setFocus();
      return;
  }
  if( ui->timeEditStart->time() == ui->timeEditEnd->time() )
  {
      QMessageBox::warning(this, "Oops!", "Por favor, selecione os horários de inicio e fim desta reserva!");
      ui->tableView->setFocus();
      return;
  }
  //// verificar se intervalo esta disponivel
  QDate currentDate = m_date;
  QString Con;
  int nTimes = 1;
  if( ui->groupBoxRepeat->isChecked())
      nTimes = ui->spinBoxVezes->value()+1;

  for( int i = 0; i < nTimes; i++  )
  {
      if( m_reserve == NULL )
          Con = QString("select * from reserve where ((('%1' >= time_start and '%1' < time_end) or ('%2' >= time_start and '%2' <= time_end)) or ('%1' < time_start and '%2' > time_end)) and canceled <> true and date_start='%3' and commona_id = %4 ")
                                .arg(ui->timeEditStart->time().toString(FMT_TIME))
                                .arg(ui->timeEditEnd->time().toString(FMT_TIME))
                                .arg(currentDate.toString(FMT_DATE_DB))
                                .arg(m_CommonId);

      else
          Con = QString("select * from reserve where ((('%1' >= time_start and '%1' < time_end) or ('%2' >= time_start and '%2' <= time_end)) or ('%1' < time_start and '%2' > time_end)) and id <> %3 and canceled <> true and date_start='%4' and commona_id = %5 ")
                                .arg(ui->timeEditStart->time().toString(FMT_TIME))
                                .arg(ui->timeEditEnd->time().toString(FMT_TIME))
                                .arg(m_reserve->getid())
                                .arg(currentDate.toString(FMT_DATE_DB))
                                .arg(m_CommonId);


      debug_message("SQL consistense:%s\n",Con.toLatin1().data());

      reserveList *res = reserve::findBy(Con);
      if( res && res->count())
      {
          QMessageBox::warning(this, "Oops!", QString("O intervalo de horário selecionado no dia %1 já está reservado para outra pessoa!")
                                              .arg(currentDate.toString(FMT_DATE)));
          ui->tableView->setFocus();
          return;
      }

      if( ui->groupBoxRepeat->isChecked())
      {
          switch( ui->comboBoxRepetirForma->currentIndex())
          {
            case 0: // Diariamente
                    currentDate = currentDate.addDays(1);
                    break;
            case 1: // Semanalmente
                    currentDate = currentDate.addDays(7);
                    break;
            case 2: // Mensalmente
                    currentDate = currentDate.addMonths(1);
                    break;
          }
      }

  }
  QString Abstract = QString("Deseja realmente salvar esta reserva?\nDetalhes:\n\nLocal: %4\nResponsável: %1\nInício: %2\nFim: %3\n%5")
                     .arg(ui->comboBox->currentText())
                     .arg(ui->timeEditStart->time().toString(FMT_TIME_REST))
                     .arg(ui->timeEditEnd->time().toString(FMT_TIME_REST))
                     .arg(m_CommonName)
                     .arg(ui->groupBoxRepeat->isChecked()?QString("Repetir %1, %2 vezes").arg(ui->comboBoxRepetirForma->currentText()).arg(ui->spinBoxVezes->value()):"");

  if(QMessageBox::No == QMessageBox::question(this,
                        "Atenção",
                        Abstract,
                        QMessageBox::Yes| QMessageBox::No, QMessageBox::Yes))
  {
      return;
  }

  /// loop nunca vai acontecer quando for edicao
  /// pois o repeat group box estará invisivel neste caso...
  currentDate = m_date;
  bool bFail = false;
  for( int i = 0; i < nTimes; i++  )
  {
      reserve *Reserve;
      if( m_reserve == NULL )
      {
          Reserve= new reserve;
          Reserve->setresponsable(m_modelCombo->index(ui->comboBox->currentIndex(),1).data().toInt());
      }
      else
          Reserve = m_reserve;

      Reserve->setcommona_id(m_CommonId);
      Reserve->setdate_start(currentDate);
      Reserve->setdate_end(currentDate);
      Reserve->settime_start(ui->timeEditStart->time());
      Reserve->settime_end(ui->timeEditEnd->time());
      if( Reserve->Save() )
      {

          if( 1 == nTimes )
              if( QMessageBox::Yes == QMessageBox::question(this, "Atenção", "Deseja adicionar convidados agora?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
              {
                  reserve *ReserveAux = m_reserve;
                  m_reserve = Reserve;
                  DoInvited(true);
                  m_reserve = ReserveAux;
              }

          delete Reserve;
   //       accept();
      }
      else
      {
          QMessageBox::warning(this, "Oops!", QString("Problema ao salvar a reserva, detalhe:%1").arg(Reserve->lastError().text()));
          if( m_reserve == NULL )
              delete Reserve;
          bFail = true;
          break;
      }
      if( ui->groupBoxRepeat->isChecked())
      {
          switch( ui->comboBoxRepetirForma->currentIndex())
          {
            case 0: // Diariamente
                    currentDate = currentDate.addDays(1);
                    break;
            case 1: // Semanalmente
                    currentDate = currentDate.addDays(7);
                    break;
            case 2: // Mensalmente
                    currentDate = currentDate.addMonths(1);
                    break;
          }
      }

  }
  if( !bFail)
  {

      accept();
  }

}

QString NewDayReserve::GetResponsable(int dwellerid)
{
    QString result;

    Dweller *dw = Dweller::findByid(dwellerid);
    if( !dw )
        return result;

    Ap *ap =  Ap::findByid(dw->getap());
    if( !ap )
    {
        delete dw;
        return result;
    }

    Tower *tw = Tower::findByid(dw->gettower());
    if( !tw )
    {
        delete dw;
        delete ap;
        return result;
    }

    result = ap->getNumber() + " " + tw->getName().mid(0,1) + " - " + dw->getName();

    delete dw;
    delete ap;
    delete tw;
    return result;

}

int NewDayReserve::RunTimes()
{
  common_area *ca = common_area::findByid(m_CommonId,true);
  if( !ca )
      return 0;

  //commona_id | date_start
  QString sqlReserve = QString("select * from reserve where date_start = '%1' and commona_id=%2 and canceled <> true")
          .arg(m_date.toString(FMT_DATE_DB))
          .arg(m_CommonId);

  reserveList *res = reserve::findBy(sqlReserve);

  debug_message("SQL Reserves: %s\n", sqlReserve.toLatin1().data());

  CreateTempTable();

  QTime time = QTime(0,0,0);


//  if( ca->getinterval() == midhour )
  {

      QString Responsable;
      for( int i = 0; i < ((ca->getinterval()==midhour)?48:24); i++ )
      {

          Responsable = "";
          for( int j = 0; res && (j < res->count()); j++)
          {

              if(( time >= res->at(j)->gettime_start()) && ( time <res->at(j)->gettime_end()))
              {

                  Responsable = GetResponsable(res->at(j)->getresponsable());
                  debug_message("Responsavel: %s hora:%s\n", Responsable.toLatin1().data(), time.toString(FMT_TIME_REST).toLatin1().data());
                  break;
              }

          }


          QTime timeend = time.addSecs(60*(ca->getinterval()==midhour?30:60));
          InsertTemTbl(time, timeend, Responsable);
          time = timeend;
      }
  }
  return 1;
}

int NewDayReserve::Edit(int id, QString CommonName)
{
   m_reserve =    reserve::findByid(id, true);
   if( NULL == m_reserve )
   {
       QMessageBox::warning(this,
                            "Oops!",
                            QString("Não foi possível encontrar a reserva id:%1").arg(id));
       return QDialog::Rejected;
   }
   ui->pushButtonInvited->setVisible(true);

   m_CommonName = CommonName;
   ui->lineEditLocal->setEnabled(false);
   ui->lineEditLocal->setText(CommonName);
   ui->groupBoxRepeat->setVisible(false);

   m_CommonId = m_reserve->getcommona_id();
   m_date = m_reserve->getdate_start();

   setWindowTitle(QString("Editando Reserva de %1, Dia %2").arg(CommonName).arg(m_date.toString(FMT_DATE)));
   ui->groupBoxHorarios->setTitle(QString("Dia %1, Horários").arg(m_date.toString(FMT_DATE)));


   FillComboBox();

   for( int i = 0; i < m_modelCombo->rowCount(); i++ )
   {
       if( m_modelCombo->index(i,1).data().toInt() == m_reserve->getresponsable())
       {
           ui->comboBox->setCurrentIndex(i);
           ui->comboBox->setEnabled(false);
           break;
       }
   }

   //// caso o trecho acima não econtre o responsavel, pode ficar visualmente estranho...


   RunTimes();
   debug_message("depois do RunTimes\n");
   PopulateTableView();

   debug_message("depois do PopulateTableView\n");

   return exec();

}

int NewDayReserve::Exec(int CommonId, QString CommonName,  QDate date)
{
   m_CommonId = CommonId;
   m_CommonName = CommonName;
   m_date = date;

   ui->lineEditLocal->setEnabled(false);
   ui->lineEditLocal->setText(CommonName);
   setWindowTitle(QString("Nova Reserva de %1, Dia %2").arg(CommonName).arg(m_date.toString(FMT_DATE)));
   ui->groupBoxHorarios->setTitle(QString("Dia %1, Horários").arg(m_date.toString(FMT_DATE)));
   debug_message("antes do RunTimes\n");

   FillComboBox();

   RunTimes();
   debug_message("depois do RunTimes\n");
   PopulateTableView();

   debug_message("depois do PopulateTableView\n");

   ui->comboBox->setFocus();

   return exec();
}
int NewDayReserve::InsertTemTbl(QTime timestart,
                                QTime timeend,
                                QString responsable)
{
    QString sqlInsrt = QString("insert into times(date_start, date_end, responsable) values( '%1','%2', '%3')")
                      .arg(timestart.toString(FMT_TIME_REST))
                      .arg(timeend.toString(FMT_TIME_REST))
                      .arg(responsable);

    QSqlQueryModel *qInsert = new QSqlQueryModel;
    qInsert->setQuery(sqlInsrt,m_litedb);
    debug_message(QString(" InsertTempTbl:LastError=%1\n").arg(qInsert->lastError().text()).toLatin1().data());

    delete qInsert;

}
bool NewDayReserve::CreateTempTable()
{
    QString strCreate = QString("Create table times ("\
    "id INTEGER PRIMARY KEY autoincrement, "\
    "date_start TEXT, "\
    "date_end TEXT, "\
    "responsable TEXT);");

    QSqlQueryModel *qCreate = new QSqlQueryModel;
    qCreate->setQuery(strCreate,m_litedb);
    debug_message(QString(" CreateTable:LastError=%1\n").arg(qCreate->lastError().text()).toLatin1().data());

    delete qCreate;
    return true;
}

void NewDayReserve::PopulateTableView()
{
   QString select = QString("select date_start, date_end, responsable from times order by id");

   m_model->setQuery(select,m_litedb);

   m_model->setHeaderData(0, Qt::Horizontal, QString::fromUtf8("Início"));
   m_model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Fim"));
   m_model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Responsável"));

   ui->tableView->setModel(m_model);
   ui->tableView->horizontalHeader()->setStretchLastSection(true);
   ui->tableView->setItemDelegateForColumn(0, m_delegate);
   ui->tableView->setItemDelegateForColumn(1, m_delegate);
   ui->tableView->setItemDelegateForColumn(2, m_delegate);

   connect(
    ui->tableView->selectionModel(),
               SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
               this,
               SLOT(OnSelectionChanged(QItemSelection,QItemSelection)));

}

void NewDayReserve::FillComboBox()
{
    QString SQLCombo = " select a.numero || ' ' || substring(t.name from 1 for 1) || '-' || d.name as \"Responsável\","\
                       " d.id "\
                       " from dweller d  "\
                       " inner join ap a on a.id = d.ap  "\
                       " inner join tower t on t.id = d.tower  "\
                       " where d.removed <> true and d.movedout <> true "\
                       " order by t.id, a.id";


   m_modelCombo->setQuery(SQLCombo);
   ui->comboBox->setModel(m_modelCombo);
   ui->comboBox->setModelColumn(0);

   if( ui->comboBox->completer() )
       ui->comboBox->completer()->setFilterMode(Qt::MatchContains );


}
void NewDayReserve::OnSelectionChanged(QItemSelection ,QItemSelection )
{
//   QItemSelectionModel* selection = ui->tableView->selectionModel();

//   QItemSelectionRange range = selection->selection().front();

   QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

   // Multiple rows can be selected
   if(selection.count())
   {
       QModelIndex index = selection.at(0);
       ui->timeEditStart->setTime(m_model->index(index.row(),0).data().toTime());
       index = selection.at(selection.count()-1);
       ui->timeEditEnd->setTime(m_model->index(index.row(),1).data().toTime());
   }
}
