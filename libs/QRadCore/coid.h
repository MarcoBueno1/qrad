#ifndef COID_H
#define COID_H

#include <QString>
#include <QPixmap>
#include <QSqlDatabase>

class COID
{
public:
    COID();
public:
    static int setImage(QString strFullPath, QSqlDatabase db = QSqlDatabase::database());
    static int setImage(QPixmap pixmap, QSqlDatabase db = QSqlDatabase::database());
    static QString getImage( int oid, QSqlDatabase db = QSqlDatabase::database() );
    static QPixmap getImagePix( int oid, QSqlDatabase db = QSqlDatabase::database() );
};

#endif // COID_H
