#include "reportlauncher.h"
#include "ui_reportlauncher.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QSqlRecord>
#include <QSqlField>
#include <QDesktopServices>
#include <QUrl>
#include "pdfwrapper.h"


ReportLauncher::ReportLauncher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportLauncher)
{
    ui->setupUi(this);

    ui->dateTimeEditUntil->setDate(QDate::currentDate().addDays(-3));

    ui->dateTimeEditUntil->setDate(QDate::currentDate());

    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(OkPressed()));
    connect(ui->pushButtonCancelar, SIGNAL(clicked()), this, SLOT(CancelPressed()));

    m_LeftHead.append("Blink. @2016");
    m_LeftHead.append("Manaus - AM");
    m_LeftHead.append("Watsapp +55 (92) 98415-1066");


    m_report = NULL;
    m_model = NULL ;
}

ReportLauncher::~ReportLauncher()
{
    if( m_report != NULL )
        delete m_report;

    if( m_model != NULL )
        delete m_model;

    delete ui;
}
void ReportLauncher::Exec(QString action)
{

    m_report = report::findByMenu(action, true);

    if( !m_report )
    {
        QMessageBox::warning(this, "Oops!", QString::fromUtf8("Relatório %1 não encontrado!").arg(action));
        return;
    }

    setWindowTitle(m_report->getName());

    m_model = new QSqlQueryModel;

    m_model->setQuery(m_report->getSqlCombo());

    if( m_report->getDateMode() == dtNoDate)
        ui->groupBoxPeriod->setVisible(false);

     exec();

}

void ReportLauncher::OkPressed()
{
    Buildreport();
}
void ReportLauncher::CancelPressed()
{
  reject();
}

void ReportLauncher::Buildreport()
{
    QList< FieldFormat *> headers;
    QList<QStringList *> lines;

     int i;
     QSqlQueryModel *model  = new QSqlQueryModel;
     model->setQuery(m_report->getSql());

     QStringList *line;
     int nColumns = model->columnCount();
     for( i = 0; i < nColumns; i++ )
     {
        QString strAux = model->headerData(i, Qt::Horizontal).toString();
        FieldFormat *f = (FieldFormat *)malloc(sizeof(FieldFormat));
        strcpy(f->Name,strAux.toLatin1().data());
        //if( m_percents.count() == nColumns )
        //   f->Percent = m_percents.at(i).toDouble();
       // else
           f->Percent = 100/nColumns;

       // if( m_aligns.count() == nColumns )
       //    f->Align   = m_aligns.at(i).toInt();
       // else
           f->Align   = ALIGN_CENTER;

        headers.append(f);
     }


     for(  i = 0; i < model->rowCount(); i++ )
     {
       QSqlRecord rec = model->record(i);
       line = new QStringList;
       for( int j = 0; j < nColumns; j++ )
       {
         line->append(rec.field(j).value().toString() )    ;
       }
       lines.append(line);

     }

     QString strAux =  QString("%1%2").arg(m_report->getName()).arg(".pdf");
     strAux.replace(" ", "_");
     if( 0 == pdfwrapper::Build( strAux , m_LeftHead, m_report->getName(), headers, lines ))
          QDesktopServices::openUrl(QUrl(strAux, QUrl::TolerantMode));
}

