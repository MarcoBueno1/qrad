#include "clicense.h"
#include <QDebug>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlField>
#include <QSqlRecord>
#include <QCryptographicHash>
#include "qraddebug.h"
#define _CLIENT_SIDE 1

#define QRADL_SELECT_MACHINES        "select id, machine, id from machine order by id"
#define QRADL_MAX_USERS              "select max from maincompany"
#define QRADL_SELECT_STOREDLICENSE   "select lastlicense from maincompany"
#define QRADL_SELECT_CNPJ            "select cnpj from maincompany"
#define QRADL_UPDATE_LICENSE         "update maincompany set lastlicense = "
#define QRADL_UPDATE_MAX_USERS       "update maincompany set max = "

/// FIX: Adjust next line to provide safe fraud verification.
#define QRADL_SELECT_SELL_DATE       "select data_entrada from visit order by date"
//

#define QRADL_CONNECTED_USERS        "SELECT usesysid, usename FROM pg_stat_activity"

#define MAX_MONTHS_LICENSE 24

const char g_UnlockKey[] = "@1@1@1@:\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf";


Clicense::Clicense()
{

}

Clicense::~Clicense()
{

}

int Clicense::IsValidlicense( QString strlicense )
{
    m_strlicense = strlicense;

    // step 1 --Test if strlicence is valid
    if( 0 == strlicense.length() )
    {
        // step 2 -- Test if stored already valid ..
        if( IsStoredStillValid() )
        {
            // step 3 -- Verify date Fraud ..
            if( IsLocalDateWrong())
            {
                return DSM_ERR_DATE_FRAUD;
            }
            int nUsers = TooMuchUsers();
            if( nUsers )
            {
                 debug_message("retornou no too much user=%d\n",nUsers);

                 return nUsers;
            }

            return DSM_SUCCESS;
        }

        return DSM_ERR_INVALID_LICENSE;
    }

    // step 4 -- Build New license string and compare with passed strlicense, if ok store it
    if( IsNewlicenseValid( strlicense ) )
    {
         StoreNewlicense( strlicense );
         // step 5 -- Verify date Fraud ..
         if( IsLocalDateWrong())
         {
                return DSM_ERR_DATE_FRAUD;
         }
         return DSM_SUCCESS;
    }


    return DSM_ERR_INVALID_LICENSE;
}


bool Clicense::IsStoredStillValid()
{
    QDate objDate;
    QString strGenratedKey;
    QString strStoredKey;

    objDate = QDate::currentDate();

   // strGenratedKey = BuildKey( objDate );
    strStoredKey   = GetStoredKey();

    if( 0 == strStoredKey.length())
    {
        return false;
    }


    for( int iIndex = 0; iIndex < MAX_MONTHS_LICENSE; iIndex++ )
    {
        strGenratedKey = BuildKey( objDate );
        if( strGenratedKey.length()
            && (strGenratedKey == strStoredKey ))
        {
            return true;
        }

        objDate = objDate.addMonths(1);
    }

/*    // check if last month is ok ( max 60 days whithout pay for licenses ..)
    objDate = QDate::currentDate().addMonths(-1);

    strGenratedKey = BuildKey( objDate.month(), objDate.year() );

    if( strGenratedKey.length()
        && strStoredKey.length()
        && (strGenratedKey == strStoredKey ))
    {
        return true;
    }
*/

    return false;
}
//
// B4603419C77797B2AEB10A0A16E6919D
//
bool Clicense::IsNewlicenseValid( QString strNewlicense )
{
    QDate objDate;
    QString strGenratedKey;

    if( 0 == strNewlicense.length())
    {
        return false;
    }
	
    if( strNewlicense.length() > 32 )
    {
        // has max users information
        QString strmax = strNewlicense.mid(32);
        StoreMaxUsers(strmax);
        strNewlicense.truncate(32);
    }

    objDate = QDate::currentDate();

    for( int iIndex = 0; iIndex < MAX_MONTHS_LICENSE; iIndex++ )
    {
        strGenratedKey = BuildKey( objDate );
        debug_message("strGenratedKey=%s strNewlicense=%s\n",
                      strGenratedKey.toLatin1().data(),
                      strNewlicense.toLatin1().data());
//        //qDebug() << "Buind=" << strGenratedKey;

        if( strGenratedKey.length()
            && ( strGenratedKey == strNewlicense ) )
        {
            return true;
        }
        //qDebug() << "Index=" << iIndex << "Buind=" << strGenratedKey;

        objDate = objDate.addMonths(1);
    }

    return false;
}

QString Clicense::BuildKey( QDate ObjDate )
{
    QString strCNPJ;
    QString strKey;
    QString strStoredKey;
    QString strHash;
    QString strDataStream;
    QString strCurrentDate;
    QString strHDSerialNumbers;
    QByteArray strByte;

    // step 1: get stored hdserialnumbers ..
    strHDSerialNumbers = GetSerialNumbers();
    if( 0 == strHDSerialNumbers.length() )
    {
        //qDebug() << "<--BuildKey 1";
        return strHDSerialNumbers;
    }

    // step 2: get CNPJ
    strCNPJ = GetCNPJ();
    if( 0 == strCNPJ.length() )
    {
        //qDebug() << "<--BuildKey 2";
        return strCNPJ;
    }

    // step 3: get current month / year( via param .. )
    //strCurrentDate.sprintf("%02d%02d",nMonth, nYear);

    // step 4: Build hash number
    strDataStream = strHDSerialNumbers + strCNPJ + strCurrentDate;

    strKey = BuildCorekey( strDataStream, ObjDate );
/*
   //qDebug() << "Trace= " << strDataStream;

    strByte = strDataStream.toLatin1();
    strHash = QCryptographicHash::hash( strByte, QCryptographicHash::Md5 ).toHex();

    // step 5: Build key
    strKey = m_objCrypt.Encrypt( strHash, strHash );
*/
/*
    if( 0 == strKey.length() )
    {
        //qDebug() << "<--BuildKey 3";
        return strKey;
    }
*/
    return strKey;
}

QString Clicense::GetStoredKey()
{
    QSqlQuery query;

    query.prepare( QRADL_SELECT_STOREDLICENSE );

    ;
    if( !query.exec() || (-1 == query.size()) )
    {
        return "";
    }
    query.first();

    return  query.value(query.record().indexOf("lastlicense")).toString();
}

int Clicense::TooMuchUsers()
{
     QSqlQuery query;
     QString strMax = GetMaxUsers( GetCNPJ() );
	 
	 strMax.remove(" ");
	 strMax.remove(";");
	 
     query.prepare( QRADL_CONNECTED_USERS );
     if( !query.exec() || (-1 == query.size()) )
     {
		return 0;
	 }
	 if( query.size() > strMax.toInt() )
		 return  query.size() - strMax.toInt() ;
	 
	 return 0;
}
//
// This information need to be stored cripted
//
void Clicense::StoreMaxUsers( QString strMax )
{
   QSqlQuery query;
   QString strSQL;

   strSQL = QString(QRADL_UPDATE_MAX_USERS) + "'" + strMax + "'";
   query.exec(strSQL);
   //qDebug() <<  strNewlicense;
}

QString Clicense::GetMaxUsers( QString cnpj )
{
    QSqlQuery query;
    QString strUsers = "";
    query.prepare( QRADL_MAX_USERS );
	
    if( !query.exec() || (-1 == query.size()) )
    {
		return strUsers;
    }
    strUsers =  query.value(query.record().indexOf("max")).toString();
	
    if( 0 == strUsers.length() )
	return strUsers;
	
    while( strUsers.length() % 8 )
	strUsers.append(" ");

   return DecryptClientIdentification( strUsers, cnpj );
}

QString Clicense::GetSerialNumbers()
{
    int nIndex;
    QSqlQuery query;
    QString strTemp;
    QString strNumbers = "";

    query.prepare( QRADL_SELECT_MACHINES );

    if( !query.exec() || (-1 == query.size()) )
    {
        //qDebug() << "retornou null na primeira";
        return strNumbers;
    }
    query.first();

    for( nIndex = 0; nIndex < query.size(); nIndex++ )
    {
        strTemp = query.value(query.record().indexOf("id")).toString();
        if( strTemp.length() )
            strNumbers.push_back(strTemp + ";");
        strTemp = query.value(query.record().indexOf("machine")).toString();
        if( strTemp.length() )
            strNumbers.push_back( strTemp + ";");
        query.next();
    }

    return strNumbers;
}

QString Clicense::GetCNPJ()
{
    QSqlQuery query;

    query.prepare( QRADL_SELECT_CNPJ );
    if( !query.exec() )
    {
        //qDebug() << "Retornou problema no cnpj";
        return "";
    }
    query.first();

    return query.value(query.record().indexOf("cnpj")).toString();
}

bool Clicense::StoreNewlicense( QString strNewlicense)
{
   QSqlQuery query;
   QString strSQL;

   strSQL = QString(QRADL_UPDATE_LICENSE) + "'" + strNewlicense + "'";
   query.exec(strSQL);
   //qDebug() <<  strNewlicense;

   return true;
}

QString Clicense::GetClientIdentification( )
{
    QString strCNPJ;
    QString strIdentification;
    QString strHDSerialNumbers;

    // step 2: get CNPJ
    strCNPJ = GetCNPJ();
    if( 0 == strCNPJ.length() )
    {
        return strCNPJ;
    }

    // step 1: get stored hdserialnumbers ..
    strHDSerialNumbers = GetMaxUsers(strCNPJ) + GetSerialNumbers();
    if( 0 == strHDSerialNumbers.length() )
    {
        return strHDSerialNumbers;
    }
	
    // step3: Encrypt data to transmit ..

    strIdentification = strHDSerialNumbers + strCNPJ;
    while( strIdentification.length() % 8 )
        strIdentification.append(" ");

    while( strCNPJ.length() % 8 )
        strCNPJ.append(" ");

    //qDebug() << "Antes da cripto" << strIdentification;
    return  m_objCrypt.Encrypt( strIdentification, strCNPJ, false ); // colocar false
}

QString Clicense::DecryptClientIdentification( QString strIdentification, QString strCNPJ )
{
#ifndef CLIENT_SIDE

    while( strCNPJ.length() % 8 )
        strCNPJ.append(" ");

    //qDebug() << "CNPJ:" << strCNPJ;
    //qDebug() << "Identificaçao:" << strIdentification;
    //qDebug() << "Tamanho id" << strIdentification.length();

    QString strId  =m_objCrypt.Decrypt( strIdentification, CCrypt::toHex(strCNPJ), true, false );

    while(strId.endsWith(" "))
    {
        strId.remove(strId.lastIndexOf(""),1);
    }

    return  strId;
#else
    return "";
#endif
}

QString Clicense::GeneratelicenseString( QString strIdentification, QDate objDate )
{
/*
    QString strKey;
    QString strHash;
    QByteArray strByte;
    QString strDataStream;
    QString strCurrentDate;

    strCurrentDate.sprintf("%02d%02d",objDate.month(), objDate.year());

    strDataStream = strIdentification + strCurrentDate;
    strByte = strDataStream.toLatin1();

    //qDebug() << "strDataStream:" << strDataStream;

    strHash = QCryptographicHash::hash( strByte, QCryptographicHash::Md5 ).toHex();

    //qDebug() << "Hash:" << strHash;

    strKey = m_objCrypt.Encrypt( strHash, strHash );

    //qDebug() << "strKey:" << strKey;

    return strKey;
*/
#ifndef CLIENT_SIDE
    return BuildCorekey( strIdentification, objDate );
#else
    return "";
#endif
}

bool Clicense::IsLocalDateWrong()
{
    QDate objDate;
    QSqlQuery query;

    query.prepare( QRADL_SELECT_SELL_DATE );
    if( !query.exec() )
    {
        //qDebug() << "Retornou problema na data";
        return false;
    }
    if( 0 == query.size() )
    {
        return false;
    }

    query.last();

    objDate = query.value(query.record().indexOf("data_entrada")).toDate();
    if( objDate > QDate::currentDate() )
    {
        //qDebug() << objDate.day() << objDate.month() << objDate.year();
        //qDebug() << QDate::currentDate().day() << QDate::currentDate().month() << QDate::currentDate().year();
        return true;
    }

    return false;
}

int Clicense::RemainingDays( )
{
    int iIndex;
    QDate ObjDate;
    QString strKey;
    QString strGenratedKey;

    strKey = GetStoredKey();

    if( strKey.length() == 0 )
    {
        return 0;
    }

    ObjDate = QDate::currentDate();

    //ObjDate.addMonths(-1);

    for( iIndex = 0; iIndex < MAX_MONTHS_LICENSE; iIndex++ )
    {
        strGenratedKey = BuildKey( ObjDate );

        if( strGenratedKey.length()
            && (strGenratedKey == strKey ))
        {
           break;
        }
        ObjDate = ObjDate.addMonths(1);
    }

    if( MAX_MONTHS_LICENSE == iIndex )
    {
        return 0;
    }

    return QDate::currentDate().daysTo(ObjDate) + ObjDate.daysInMonth() - ObjDate.day();
}


QString Clicense::BuildCorekey( QString strIdentification, QDate objDate )
{
    QString strKey;
    QString strHash;
    QByteArray strByte;
    QString strDataStream;
    QString strCurrentDate;

    strCurrentDate.sprintf("m%02d%02dm",objDate.month(), objDate.year());

    strDataStream = strIdentification + strCurrentDate;
    strByte = strDataStream.toLatin1();

    //qDebug() << "strDataStream:" << strDataStream;

    strHash = QCryptographicHash::hash( strByte, QCryptographicHash::Md5 ).toHex();
	
	///
	/// Extra step: to diferenciate from M2 old algorithm ( double hash )
	///
    strHash = QCryptographicHash::hash( strHash.toLatin1(), QCryptographicHash::Md5 ).toHex();
	///-----------------------------------------------------

    //qDebug() << "Hash:" << strHash;

    strKey = m_objCrypt.Encrypt( strHash, strHash );

    //qDebug() << "strKey:" << strKey;

    return strKey;
}

void Clicense::GetUnlockKey( char *pUnlockKey )
{
   memcpy( pUnlockKey, &g_UnlockKey[8], 16);
}
