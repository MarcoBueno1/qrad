#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <QFile>
#include "pgsqlasync.h"
#include "qraddebug.h"
#include "libpq-fe.h"

#ifndef __linux__
#define DSM_WAIT_TIME 25000
#endif


PGSQLAsync::PGSQLAsync()
{
}

unsigned int PGSQLAsync::Send( QString strFilePath,
                               QString strstrHostName,
                               QString strdatabasename,
                               QString strusername,
                               QString strpassword)
{
    char conninfo[256];
    PGconn     *conn;
    PGresult   *res;

    char       szfilename[2048];
    Oid         lobjOid;

    if( strstrHostName.contains("localhost") )
        strstrHostName = "127.0.0.1";



    QString strconnection;
    strconnection = "hostaddr = '" + strstrHostName +"' ";
    strconnection += " dbname = '" +strdatabasename + "' ";
    strconnection += " user = '" +strusername + "' ";
    strconnection += " password = '" +strpassword + "' ";


    strcpy(conninfo, strconnection.toUtf8().data());

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
     if (PQstatus(conn) != CONNECTION_OK)
     {
         debug_message("Connection to database failed: %s",
         PQerrorMessage(conn));
     }

     res = PQexec(conn, "begin");
     PQclear(res);

     strcpy(szfilename,strFilePath.toUtf8().data());

     printf("importing file %s\n", szfilename);

     lobjOid = lo_import(conn, szfilename);

     res = PQexec(conn, "end");

     PQclear(res);

     PQfinish(conn);

     return lobjOid;
}


void PGSQLAsync::Receive( unsigned int loid,
                          QString strFilePath,
                          QString strstrHostName,
                          QString strdatabasename,
                          QString strusername,
                          QString strpassword )
{
    char conninfo[256];
    PGconn     *conn;
    PGresult   *res;
    char       szfilename[2048];

	
	QFile::remove(strFilePath);


    if( strstrHostName.contains("localhost") )
        strstrHostName = "127.0.0.1";



    QString strconnection;
    strconnection = "hostaddr = '" + strstrHostName +"' ";
    strconnection += " dbname = '" + strdatabasename + "' ";
    strconnection += " user = '" +strusername+ "' ";
    strconnection += " password = '" +strpassword + "' ";


    strcpy(conninfo, strconnection.toUtf8().data());

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
     if (PQstatus(conn) != CONNECTION_OK)
     {
         debug_message("Connection to database failed: %s",
         PQerrorMessage(conn));
     }

     res = PQexec(conn, "begin");
     PQclear(res);

     strcpy(szfilename,strFilePath.toUtf8().data());

     printf("exporting file %s\n", szfilename);

     lo_export(conn, loid, szfilename);


     res = PQexec(conn, "end");

     PQclear(res);

     PQfinish(conn);

}
