#include "stableview.h"

#include <QSqlDriver>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

STableView::STableView(QWidget *parent) :
        QTableView(parent)
{

}

STableView::~STableView()
{

}

void STableView::setModel( QSqlTableModel  * model )
{	
	model->tableName();
	
    QStringList list = QSqlDatabase::database().driver()->tables(QSql::Tables);
    QSqlDriver *driver = QSqlDatabase::database().driver();
    for( int nI = 0; nI < list.count(); nI++ )
    {
        qDebug() << list.at(nI);
        QSqlRecord rec = driver->record(list.at(nI));
        for( int j = 0; j < rec.count(); j++ )
        {
            qDebug() << rec.fieldName(j) <<":" << rec.field(j).type();
        }
    }

	
}
