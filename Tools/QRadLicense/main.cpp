#include <QtCore/QCoreApplication>
#include "clicense.h"
#include <QString>
#
int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);


    /// trmlicense cnpj credential
    if( argc != 4)
    {
        fprintf(stdout,"\nusage: $ trmlicense cnpj credential ddmmyyyy\n");
        fprintf(stdout,"   ex: $ trmlicense 12345678901234 BABACA 12012012\n\n");
        fprintf(stdout,"@copyright M²Smart Solutions, 2012.\n\n");
        return 0;
    }

    Clicense *plicense = new Clicense();
    QString strAux  = plicense->DecryptClientIdentification(QString(argv[2]), QString(argv[1]));
    fprintf(stdout, "\nInfo...: %s\n", strAux.toLatin1().data());
    strAux = plicense->GeneratelicenseString( strAux, QDate::fromString(QString(argv[3]),"ddMMyyyy") );
    fprintf(stdout,"licenca: %s\n", strAux.toLatin1().data());
    fprintf(stdout,"\n@copyright M²Smart Solutions, 2012.\n\n");

    delete plicense;
    return 1;
 //   return a.exec();
}
