#ifndef CLICENSE_H
#define CLICENSE_H
#include <QString>
#include <QDate>
#include "ccrypt.h"
#include "dsmerrors.h"

#define DSM_ERR_INVALID_LICENSE -1
#define DSM_ERR_DATE_FRAUD      -2
#define DSM_ERR_TOO_MUCH_USERS  -3

class Clicense
{
public:
    Clicense();
    ~Clicense();

    // CLIENT SIDE:
    // This is main method of license class, it returns true if:
    //     - strlicense is valid for current month or last month( when != "" )
    //     - Current stored strlicense is valid for current month or last month ( when the parameter is "" )
    //
    int IsValidlicense( QString strlicense = "" );

    // CLIENT SIDE:
    // Return the client identification string:
    //     - The result string must be sent to M2Smart ( and stored in M2Smart database )
    //     - This method must be called at installation time or when  dsm_cashier table changes ( insert/update/remove rows)
    //

    QString GetClientIdentification( );
    // CLIENT SIDE:
    int RemainingDays( );
	int TooMuchUsers();
    void GetUnlockKey( char *pUnlockKey );



    // M2SMART SIDE:
    // Decrypt string generated by GetClientIdentification
    //     - The DecryptClientIdentification return a clear string with all client serial numbers (Hard Disks).
    //
    QString DecryptClientIdentification( QString strIdentification, QString strCNPJ );

    // M2SMART SIDE:
    // GeneratelicenseString  - It constructs a valid license for month/year passed in "objDate"
    //     - The strIdentification must be previously returned by DecryptClientIdentification.
    //
    QString GeneratelicenseString( QString strIdentification, QDate objDate );


private:
    QString m_strlicense;
    CCrypt m_objCrypt;

    QString GetCNPJ();
    QString GetStoredKey();
    QString GetSerialNumbers();
    QString BuildKey( QDate ObjDate );
    QString GetMaxUsers( QString cnpj );
    void StoreMaxUsers( QString strMax );
    bool StoreNewlicense( QString strNewlicense);
    bool IsNewlicenseValid( QString strNewlicense );
    bool IsStoredStillValid();
    bool IsLocalDateWrong();
    QString BuildCorekey( QString strIdentification, QDate objDate );
};

#endif // CLICENSE_H
