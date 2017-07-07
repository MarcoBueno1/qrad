#include "ccrypt.h"
#include <stdio.h>
#include <string.h>
#include <QDebug>

CCrypt::CCrypt()
{
}

CCrypt::~CCrypt()
{
}
//
// @hexstr2bin
//
int CCrypt::hexstr2bin( unsigned char *bin,
                        int num,
                        const unsigned char *hexstr,
                        int size )
{
   int i, j;
   unsigned char byte[3];

   if ((size % 2) != 0)
   {
      return -1;
   }

   if ((size / 2) > num)
   {
      return -1;
   }

   j=0; byte[2]=0;
   for (i=0; i<size; i+=2)
   {
      byte[0] = hexstr[i];
      byte[1] = hexstr[i+1];

      if (sscanf( (const char *)byte, "%02X", (unsigned int *)&bin[j++]) != 1)
      {
         return -1;
      }
   }

   return i;
}

//
// @hexstr2bin
//
int  CCrypt::bin2hexstr(char *hexstr,
                            int size,
                            unsigned char *bin,
                            int num )
{
        int i;
        char byte[3];
        char *p=hexstr;

  if (num > (size / 2))
  {
                return -1;
        }

  for (i=0; i<num; i++)
  {
    if (snprintf(byte, 3, "%02X", bin[i]) != 2)
    {
                        return -1;
                }
                memcpy(p, byte, 2);
                p+=2;
        }

   return i;
}


QString CCrypt::Encrypt( QString strSource,
                         QString strKey,
                         bool bConvertIn,
                         bool bConvertOut )
{
  return Process( strSource, strKey, DES_ENCRYPT, bConvertIn, bConvertOut );
}

QString CCrypt::Decrypt( QString strSource,
                         QString strKey,
                         bool bConvertIn,
                         bool bConvertOut )
{
  return Process( strSource, strKey, DES_DECRYPT, bConvertIn, bConvertOut );
}


QString CCrypt::Process( QString strSource,
                         QString strKey,
                         int iDesAction,
                         bool bConvertIn,
                         bool bConvertOut )
{
 int iLen;
 char szKey[16+1];
 char *szResultado;
 char inMessage[16+1];
 char outMessage[1000];
 char szMessage[1000];

 des_cblock key1,key2;
 des_key_schedule ks1,ks2;
 des_cblock ivec;

 iLen = strSource.length();

 if( bConvertIn)
 {
     if( -1 == hexstr2bin((unsigned char *)szMessage,
                1000,
                (const unsigned char *)strSource.toLatin1().data(),
                iLen))
     {
         //qDebug() << "Problema na conversao1";
     }

     if( -1 == hexstr2bin((unsigned char *)szKey,
                16+1,
                (const unsigned char *)strKey.toLatin1().data(),
                strKey.length()))
     {
         //qDebug() << "Problema na conversao2";
         //qDebug() << "chave:" << strKey;
         //qDebug() << "len:" << strKey.length();
     }

     iLen = (int) iLen/2;
 } else
 {
     memcpy(szMessage,
            strSource.toLatin1().data(),
            iLen );
     memcpy(szKey,
            strKey.toLatin1().data(),
            strKey.length());
 }


 szResultado = (char *) calloc (iLen *2, sizeof (char));

 memset (ivec, 0, sizeof (ivec));
 //memset (szResultado,0,sizeof (szResultado));

 memcpy (key1, szKey, 8);
 memcpy (key2, &szKey[8], 8);

 des_set_key(&key1, &ks1);
 des_set_key(&key2,&ks2);

 for (int iIndex = 0; iIndex < iLen; iIndex+=8)
 {
   memset (ivec, 0, sizeof (ivec));
   memcpy (inMessage, &szMessage[iIndex], 8);
   inMessage[8] = 0;



   des_ede3_cbc_encrypt((const unsigned char *)inMessage,
                        (unsigned char *)outMessage,
                        8L,
                        &ks1,
                        &ks2,
                        &ks1,
                        &ivec,
                        iDesAction);

   memcpy (&szResultado[iIndex], outMessage, 8);
 }

 if( bConvertOut )
 {
   bin2hexstr(szMessage,
              1000,
              (unsigned char *)szResultado,
              iLen);
   szMessage[iLen*2] = 0;
 }
 else
 {
   memcpy( szMessage,
           szResultado,
           iLen );
   szMessage[iLen] = 0;
 }

 free (szResultado);

 return QString(szMessage);
}

QString CCrypt::toHex( QString strSrc )
{
  char *szDest;
  QString strReturn;

  szDest = (char *)calloc( 1, strSrc.length() *2 +1);

  if ( -1  == bin2hexstr(szDest, strSrc.length() *2 ,(unsigned char *) strSrc.toLatin1().data(), strSrc.length()))
  {
       free( szDest );
       return "";
  }
  strReturn = QString( szDest );
  free(szDest);
  return strReturn;
}

QString CCrypt::toBin( QString strScr )
{
  char *szDest;
  QString strReturn;

  szDest = (char *)calloc( 1,strScr.length() );

  if ( -1  == hexstr2bin((unsigned char *)szDest, strScr.length(),(const unsigned char *)strScr.toLatin1().data() , strScr.length()))
  {
       free( szDest );
       return "";
  }
  strReturn = QString( szDest );
  free(szDest);
  return strReturn;
}
