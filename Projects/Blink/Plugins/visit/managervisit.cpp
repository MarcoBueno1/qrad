#include "managervisit.h"
#include "ui_managervisit.h"
#include "editvisit.h"
#include "column2delegate.h"
#include <QMessageBox>
#include <QDebug>

#define BN_DEFAULT_COLUMN_SEARCH 3

#define SQL_ITEMS_OPENED "select v.id, data_entrada as \"Entrou Em\", hora_entrada as \"Hora\", vi.nome as \"Visitante\",  "\
                        " d.name || ' (' || t.name || ' AP:' || a.numero || ')' as \"Convidado de\",  "\
                        " data_saida as \"Saida / Situação\", saida_hora as \"Hora Saída\", v.visittype as \"Tipo\" from visit v "\
                        " inner join dweller d on d.id = v.autorizador  "\
                        " inner join tower t on t.id = d.tower "\
                        " inner join ap a on a.id = d.ap "\
                        " inner join visitante  vi on vi.id = v.visitante  where data_saida = '2000-01-01'  order by v.id desc "


#define SQL_ITEMS_CLOSED "select v.id, data_entrada as \"Entrou Em\", hora_entrada as \"Hora\", vi.nome as \"Visitante\",  "\
                        " d.name || ' (' || t.name || ' AP:' || a.numero || ')' as \"Convidado de\",  "\
                        " data_saida as \"Saida / Situação\", saida_hora as \"Hora Saída\", v.visittype as \"Tipo\" from visit v "\
                        " inner join dweller d on d.id = v.autorizador  "\
                        " inner join tower t on t.id = d.tower "\
                        " inner join ap a on a.id = d.ap "\
                        " inner join visitante  vi on vi.id = v.visitante  where data_saida <> '2000-01-01'  order by data_saida, saida_hora desc"


/*
#define SQL_ITEMS_CLOSED "select v.id, data_entrada as \"Entrou Em\", hora_entrada as \"Hora\", vi.nome as \"Visitante\", "\
                        " (select d.name || ' (' || t.name || ' AP:' || a.numero || ')' as \"Convidado de\" , v.visittype as \"Tipo\" "\
                        " from dweller d "\
                        " inner join tower t on t.id = d.tower "\
                        "inner join ap a on a.id = d.ap where d.id = v.autorizador), "\
                         "data_saida as \"Saida / Situação\", saida_hora as \"Hora Saída\" from visit v "\
                         "inner join Visitante  vi on vi.id = v.visitante  "\
                         "where data_saida <> '2000-01-01' "\
                         " order by data_saida, saida_hora desc"
*/

#define SQL_ITEMS_ALL "select v.id, data_entrada as \"Entrou Em\", hora_entrada as \"Hora\", vi.nome as \"Visitante\",  "\
    " d.name || ' (' || t.name || ' AP:' || a.numero || ')' as \"Convidado de\",  "\
    " data_saida as \"Saida / Situação\", saida_hora as \"Hora Saída\", v.visittype as \"Tipo\" from visit v "\
    " inner join dweller d on d.id = v.autorizador  "\
    " inner join tower t on t.id = d.tower "\
    " inner join ap a on a.id = d.ap "\
    " inner join visitante  vi on vi.id = v.visitante  order by v.id desc "


Managervisit::Managervisit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Managervisit)
{
    ui->setupUi(this);

    m_keyinterval = NULL;
    m_Model = new QSqlQueryModel;

    m_DateNullDelagate = new ColumnDateTimeNull;
    m_TimeNullDelagate = new ColumnTimeNull;
    m_ColumImage = new ColumnImageMail;

    //QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};");

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));
    connect(ui->tableViewSearch, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));
    connect(ui->tableViewSearch, SIGNAL(notFound()),this,SLOT(notFound()));
    connect(ui->tableViewSearch,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(ui->tableViewSearch,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));

    connect(ui->PshBtnEditar, SIGNAL(clicked()), this, SLOT(doEditar()));
    connect(ui->PshBtnNovo, SIGNAL(clicked()), this, SLOT(doNovo()));
    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));
    connect(ui->radioButtonAll, SIGNAL(released()), this, SLOT(LoadTableView()) );
    connect(ui->radioButtonEnded, SIGNAL(released()), this, SLOT(LoadTableView()) );
    connect(ui->radioButtonOpened, SIGNAL(released()), this, SLOT(LoadTableView()) );

    DoRefresh();

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    flags |= Qt::WindowContextHelpButtonHint;
    setWindowFlags( flags );
    setWindowState(Qt::WindowMaximized);

    setWindowTitle("Gerenciador de Visitas");
}

Managervisit::~Managervisit()
{
    if( m_keyinterval )
    {
        m_keyinterval->stop();
        delete m_keyinterval;
    }
    delete m_Model;
    delete m_DateNullDelagate;
    delete m_TimeNullDelagate;
    delete m_ColumImage;

    delete ui;
}

void Managervisit::KeyPressTimeout()
{
//    if( ui->lineEditSearch->text().trimmed().isEmpty() )
//        ui->tableViewSearch->selectRow(0);
//    else
        ui->tableViewSearch->Search(ui->lineEditSearch->text());
}

void Managervisit::StartTimer( QString )
{
    if( ui->lineEditSearch->text().trimmed().length() == 1 )
        ui->tableViewSearch->selectRow(0);

    if( m_keyinterval )
    {
        m_keyinterval->stop();
        delete m_keyinterval;
    }
    m_keyinterval = new QTimer;
    connect(m_keyinterval, SIGNAL(timeout()), this, SLOT(KeyPressTimeout()));
    m_keyinterval->setSingleShot(true);
    m_keyinterval->setInterval(200);
    m_keyinterval->start();
}
void Managervisit::Found(QModelIndex)
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    ShowCurrentInformations();
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    ui->tableViewSearch->SetNoEmptySearch( false);
}
void Managervisit::notFound()
{
   ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
  // ui->tableViewSearch->selectRow(0);
}
void Managervisit::TableClicked(QModelIndex currentIndex)
{
    qDebug() << "TableClicked";
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations();
}
void Managervisit::CurrentChanged(QModelIndex currentIndex)
{
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations( );
}

void Managervisit::ShowCurrentInformations( void )
{
    QString strTemp;

    if( m_Model->rowCount() )
    {
         strTemp.sprintf(" Itens ( %d de %d ) ",
                         ui->tableViewSearch->currentIndex().row() + 1,
                         m_Model->rowCount() );
         ui->groupBoxItens->setTitle(strTemp);
    }
}

void Managervisit::LoadTableView()
{
    QApplication::processEvents();

    if(ui->radioButtonOpened->isChecked())
    {
        m_Model->setQuery(SQL_ITEMS_OPENED);
        debug_message("SQL %s\n", SQL_ITEMS_OPENED);
    }
    else if(ui->radioButtonEnded->isChecked())
    {
        m_Model->setQuery(SQL_ITEMS_CLOSED);
        debug_message("SQL %s\n", SQL_ITEMS_CLOSED);
    }
    else
    {
        m_Model->setQuery(SQL_ITEMS_ALL);
        debug_message("SQL %s\n", SQL_ITEMS_ALL);
    }


    QApplication::processEvents();
    ui->tableViewSearch->setModel( m_Model);
// mbueno    ui->tableViewSearch->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);


    QApplication::processEvents();
}

void Managervisit::DoRefresh()
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    QApplication::processEvents();

    LoadTableView();

    ui->lineEditSearch->setFocus();
    ConfigureTable();

    QModelIndex index;
    index = ui->tableViewSearch->currentIndex();

    ui->lineEditSearch->setText(index.sibling(index.row(), BN_DEFAULT_COLUMN_SEARCH).data().toString());

    ui->tableViewSearch->SetNoEmptySearch( false );

    refreshTable();
}

void Managervisit::refreshTable()
{
    if(ui->lineEditSearch->text() == "")
    {
        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);
        ui->tableViewSearch->Search("");
        ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
}

void Managervisit::ConfigureTable()
{
      ui->tableViewSearch->addSearchColumn(3);
      ui->tableViewSearch->addSearchColumn(4);
      ui->tableViewSearch->addSearchColumnFilter(3);
      ui->tableViewSearch->addSearchColumnFilter(4);


   // m_Model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Conuna1"));
    //m_Model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Conuna2"));
   // m_Model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Conuna3"));
   // m_Model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Conuna3"));
  //  m_Model->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Conuna4"));

   // ui->tableViewSearch->setColumnWidth(0, 0.06 * ui->tableViewSearch->width());
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("id"));

    ui->tableViewSearch->setItemDelegateForColumn(ui->tableViewSearch->getColumnOf("Saida / Situação"),
                                                  m_DateNullDelagate);

    ui->tableViewSearch->setItemDelegateForColumn(ui->tableViewSearch->getColumnOf("Tipo"),
                                                    m_ColumImage);

    ui->tableViewSearch->setItemDelegateForColumn(ui->tableViewSearch->getColumnOf("Hora Saída"),
                                                  m_TimeNullDelagate);
}

void Managervisit::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "KeyPressEvent";
  //  if(event->key() != Qt::Key_Escape)
    switch(event->key())
    {
      case Qt::Key_PageUp:
      case Qt::Key_Up:
      case Qt::Key_PageDown:
      case Qt::Key_Down:
      case  Qt::Key_F12:
      case Qt::Key_Delete:
                           ui->tableViewSearch->keyPressEvent(event);
                           break;
      case Qt::Key_Escape:
  //                         ui->tableViewSearch->keyPressEvent(event);
                           doSair();
                           break;
      default:
        break;
    }


}
void Managervisit::MatchNewest(visit *newest )
{
    DoRefresh();
    for( int j = 0; j < newest->attributes().count(); j++ )
    {
        if( ui->tableViewSearch->model()->headerData(BN_DEFAULT_COLUMN_SEARCH,Qt::Horizontal).toString() == newest->attributes().at(j)->fieldName())
        {
            ui->lineEditSearch->setText(newest->attributes().at(j)->value().toString());
            ui->tableViewSearch->Search(newest->attributes().at(j)->value().toString());
        }
    }
}
void Managervisit::doEditar()
{
    QRAD_IN();
    Editvisit *edt = new Editvisit;

    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();

    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();

//    qDebug() << nId;
    debug_message("nId %d\n", nId);


    visit *sa = visit::findByid(nId);
    if( !sa )
    {
        QMessageBox::warning(this,"Oops!", "Selecione um item primeiro!");
        QRAD_OUT();
        return;
    }

    edt->SetModel(sa);
    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;

    ui->lineEditSearch->selectAll();
    ui->lineEditSearch->setFocus();
    QRAD_OUT();
}

void Managervisit::doNovo()
{
    QRAD_IN();

    Editvisit *edt = new Editvisit;

    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
        debug_message("Criada nova vista nId: %d\n", edt->GetSaved()->getid());
    }


    delete edt;

    ui->lineEditSearch->selectAll();
    ui->lineEditSearch->setFocus();


    QRAD_OUT();

}

void Managervisit::doSair()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Sair?","Deseja sair desta pesquisa?",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
           reject();
}
void Managervisit::resizeEvent(QResizeEvent * event)
{
//    debug_message("resizeEvent widht:%d resizemode: %d\n",
  //                ui->tableViewSearch->width(),
    //              ui->tableViewSearch->horizontalHeader()->sectionResizeMode());

//    LoadTableView();

    if( this->isVisible())
    {
        ui->tableViewSearch->setColumnWidth(1, 0.10 * ui->tableViewSearch->width());
        ui->tableViewSearch->setColumnWidth(2, 0.10 * ui->tableViewSearch->width());
        ui->tableViewSearch->setColumnWidth(3, 0.30 * ui->tableViewSearch->width());
        ui->tableViewSearch->setColumnWidth(4, 0.30 * ui->tableViewSearch->width());
        ui->tableViewSearch->setColumnWidth(5, 0.09 * ui->tableViewSearch->width());
        ui->tableViewSearch->setColumnWidth(6, 0.09 * ui->tableViewSearch->width());
        //ui->tableViewSearch->horizontalHeader()->setSectionResizeMode(3);
    }
  //  else
  //      ui->tableViewSearch->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


//    ui->tableViewSearch->addSearchColumn(3);
 //   ui->tableViewSearch->addSearchColumn(4);
  //  ui->tableViewSearch->addSearchColumnFilter(3);
   // ui->tableViewSearch->addSearchColumnFilter(4);

}
