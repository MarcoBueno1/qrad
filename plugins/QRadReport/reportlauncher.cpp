#include "reportlauncher.h"
#include "ui_reportlauncher.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QSqlRecord>
#include <QSqlField>
#include <QDesktopServices>
#include <QUrl>
#include "qradshared.h"
#include "pdfwrapper.h"
#include "qraddebug.h"

ReportLauncher::ReportLauncher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReportLauncher)
{
    ui->setupUi(this);

    ui->dateTimeEditUntil->setDate(QDate::currentDate().addDays(-3));

    ui->dateTimeEditUntil->setDate(QDate::currentDate());

    connect(ui->pushButtonOk, SIGNAL(clicked()), this, SLOT(OkPressed()));
    connect(ui->pushButtonCancelar, SIGNAL(clicked()), this, SLOT(CancelPressed()));

    m_LeftHead.append("Condomínio Garden Club");
    m_LeftHead.append("Parque 10 de Novembro");
    m_LeftHead.append("Manaus-AM, Brasil");


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



    if( m_report->getDateMode() == dtNoDate)
    {
        ui->groupBoxPeriod->setVisible(false);
    }
    if( m_report->getComboMode() == cmNoCombo)
    {
        ui->groupBoxJust->setVisible(false);
    }
    else
    {
        m_model = new QSqlQueryModel;
        m_model->setQuery(m_report->getSqlCombo());
        ui->comboBox->setModel(m_model);
        ui->comboBox->setModelColumn(1);
    }


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

void ReportLauncher::AutoSizeColumn(QSqlQueryModel * model)
{
    for(int i = 0; i < model->rowCount(); i++ )
        for(int j = 0; j < model->columnCount(); j++ )
        {
            QString Field = model->index(i,j).data().toString();
            qDebug() << "Field:" << Field ;
            if( m_percents.count() <= j )
                m_percents.append(QString("%1").arg(Field.size()));
            else
            {
                if( m_percents.at(j).toInt() < Field.size())
                    m_percents.replace(j, QString("%1").arg(Field.size()));
            }
        }

////
//      Dump Tamanhos Máximos
////
    qDebug() << "Dump Colunas";
    int TotalSizes = 0;
    for( int k = 0; k < m_percents.count(); k++ )
    {
        qDebug() << "Coluna:" << k << " Maximo: " << m_percents.at(k);
        TotalSizes +=m_percents.at(k).toInt();
    }
    double dwUnity = (double)100/TotalSizes;
    qDebug() << "dwUnity:" << dwUnity;

    for( int k = 0; k < m_percents.count(); k++ )
    {
        m_percents.replace(k, QString("%1").arg(dwUnity* m_percents.at(k).toInt()));
        qDebug() << "Coluna:" << k << " Percentual: " << m_percents.at(k);
    }



}

QString ReportLauncher::BuildSQL()
{
    int IdCombo = 0;
    QString sql = m_report->getSql();
    QString SqlWhere ;

    if( ui->groupBoxJust->isChecked() )
    {
        IdCombo = m_model->index(ui->comboBox->currentIndex(),0).data().toInt();
        SqlWhere = QString(m_report->getWhereCombo()).arg(IdCombo);
    }

    if( ui->groupBoxPeriod->isVisible() && ui->comboBox->isVisible())
           sql = QString(m_report->getSql()).arg(ui->dateTimeEditSince->date().toString(FMT_DATE_DB)).arg(ui->dateTimeEditUntil->date().toString(FMT_DATE_DB)).arg(SqlWhere);
    else if( ui->groupBoxPeriod->isVisible())
        sql = QString(m_report->getSql()).arg(ui->dateTimeEditSince->date().toString(FMT_DATE_DB)).arg(ui->dateTimeEditUntil->date().toString(FMT_DATE_DB));
    else if( ui->comboBox->isVisible() )
        sql = QString(m_report->getSql()).arg(SqlWhere);

    qDebug() << "Sql:" << sql;
    debug_message("SQL: %s\n",sql.toLatin1().data());
    return sql;
}

void ReportLauncher::Buildreport()
{
    QList< FieldFormat *> headers;
    QList<QStringList *> lines;

     int i;
     QSqlQueryModel *model  = new QSqlQueryModel;

     QString sql = BuildSQL();

     model->setQuery(sql);

     AutoSizeColumn(model);

     QStringList *line;
     int nColumns = model->columnCount();
     for( i = 0; i < nColumns; i++ )
     {
        QString strAux = model->headerData(i, Qt::Horizontal).toString();
        FieldFormat *f = (FieldFormat *)malloc(sizeof(FieldFormat));
        strcpy(f->Name,strAux.toLatin1().data());
        if( m_percents.count() == nColumns )
           f->Percent = m_percents.at(i).toDouble();
        else
           f->Percent = 100/nColumns;

//        if( m_aligns.count() == nColumns )
//           f->Align   = m_aligns.at(i).toInt();
//        else
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

     QString reportTitle = m_report->getName();
     int Titlesize = 20;
     if( ui->groupBoxJust->isChecked() )
     {
         reportTitle += " (Apenas "+m_model->index(ui->comboBox->currentIndex(),1).data().toString()+")";
         Titlesize = 12;
     }

     QString strAux =  QString("%1%2").arg(m_report->getName()).arg(".pdf");
     strAux.replace(" ", "_");
     if( 0 == pdfwrapper::Build( strAux , m_LeftHead, reportTitle , headers, lines, Titlesize ))
          QDesktopServices::openUrl(QUrl(strAux, QUrl::TolerantMode));
}

