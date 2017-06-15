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

     PQflush(conn);
     PQclear(res);

     PQfinish(conn);

}
int PGSQLAsync::WaitChange( QString strtable,
                            int nTimeout,
                            QString strHostName,
                            QString strDatabaseName,
                            QString strUserName,
                            QString strPassword )
{
#ifdef _nao_compilar_
#ifndef __linux__
    ConfigurationModel *config = ConfigurationModel::getInstanceNoSelect();
    if( config && config->getDisableListen() )
    {
        int WaitTime = GetTickCount() + DSM_WAIT_TIME;
        while( WaitTime > GetTickCount() )
        {
            qApp->processEvents();
            Sleep(10);
        }
        return 0;
    }
#endif
#endif

    char conninfo[256];
    PGconn     *conn;
    PGresult   *res;
    PGnotify   *notify;
    struct timeval timeout;


    /* Initialize the timeout data structure. */
    timeout.tv_sec = nTimeout;
    timeout.tv_usec = 0;

    if( strHostName.contains("localhost") )
        strHostName = "127.0.0.1";



    QString strconnection;
    strconnection = "hostaddr = '" + strHostName +"' ";
    strconnection += " dbname = '" +strDatabaseName + "' ";
    strconnection += " user = '" +strUserName + "' ";
    strconnection += " password = '" +strPassword + "' ";


    strcpy(conninfo, strconnection.toUtf8().data());

    /* Make a connection to the database */

    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
     if (PQstatus(conn) != CONNECTION_OK)
     {
         debug_message("Connection to database failed: %s",
         PQerrorMessage(conn));
//         exit_nicely(conn);
     }

     /*
      * Issue LISTEN command to enable notifications from the rule's NOTIFY.
      */
     char strAux[300];
     sprintf( strAux, "LISTEN %s",  strtable.toUtf8().data());

     res = PQexec(conn, (const char *)strAux);
     if (PQresultStatus(res) != PGRES_COMMAND_OK)
     {
         debug_message("LISTEN command failed: %s", PQerrorMessage(conn));
         PQclear(res);
//         exit_nicely(conn);
     }

     /*
      * should PQclear PGresult whenever it is no longer needed to avoid memory
      * leaks
      */
     PQclear(res);

     /* Quit after four notifies are received. */
     do
     {
         /*
          * Sleep until something happens on the connection.  We use select(2)
          * to wait for input, but you could also use poll() or similar
          * facilities.
          */

         int         sock;
         fd_set      input_mask;

         sock = PQsocket(conn);

         if (sock < 0)
             break;              /* shouldn't happen */

         FD_ZERO(&input_mask);
         FD_SET(sock, &input_mask);

#ifdef __linux__
         if (select(sock + 1, &input_mask, NULL, NULL, nTimeout?&timeout:NULL) < 0)
         {
             debug_message( "select() failed: %s\n", strerror(errno));
//             exit_nicely(conn);
         }
#endif

         /* Now check for input */
         PQconsumeInput(conn);
         while ((notify = PQnotifies(conn)) != NULL)
         {
             debug_message(
                     "ASYNC NOTIFY of '%s' received from backend pid %d\n",
                     notify->relname, notify->be_pid);
             PQfreemem(notify);
         }
     }while(0);

#ifdef __linux__
     debug_message( "Done.\n");
#endif

     /* close the connection to the database and cleanup */
     PQfinish(conn);

     return 0;
}
