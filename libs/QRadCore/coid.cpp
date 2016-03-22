#include "coid.h"
#include "pgsqlasync.h"
#include <QImage>
#include <QFile>

#define TEMP_PATH "tempfile.jpg"

COID::COID()
{

}

int COID::setImage(QString strFullPath, QSqlDatabase db)
{
    int oid = -1;
    if( db.driverName().contains("QPSQL"))
    {
        oid = PGSQLAsync::Send(strFullPath,db.hostName(),db.databaseName(),db.userName(),db.password());

    }
    else // lite
    {

    }

    return oid;
}

int COID::setImage(QPixmap pixmap, QSqlDatabase db)
{
    if( db.driverName().contains("QPSQL"))
    {

    }
    else
    {

    }

    return 1;
}

QString COID::getImage( int oid, QSqlDatabase db )
{
    QString strPath = TEMP_PATH;

    QFile::remove(strPath);

    if( db.driverName().contains("QPSQL"))
    {
        PGSQLAsync::Receive(oid,strPath,db.hostName(),db.databaseName(),db.userName(),db.password());
    }
    else
    {

    }

    return strPath;
}

QPixmap COID::getImagePix( int oid, QSqlDatabase db )
{
    if( db.driverName().contains("QPSQL"))
    {

    }
    else
    {

    }

    QImage OIDImage;
    OIDImage.load("");


    return QPixmap::fromImage(OIDImage);
}
