#ifndef CCRYPT_H
#define CCRYPT_H

#include <stdio.h>
#include <QString>
#include <sys/types.h>
#include <inttypes.h>


#ifndef __WIN32
#include <linux/types.h>

typedef uint8_t __int64;
typedef uint8_t __int8;
typedef uint32_t __int32;

#endif

#define des_set_key DES_set_key
#define des_ecb3_encrypt DES_ecb3_encrypt
#define des_ecb_encrypt DES_ecb_encrypt
#define des_ede3_cbc_encrypt DES_ede3_cbc_encrypt



#ifdef  __cplusplus
extern "C" {
#endif

#define des_ecb2_encrypt(i,o,k1,k2,e) \
        des_ecb3_encrypt((i),(o),(k1),(k2),(k1),(e))

typedef unsigned char des_cblock[8];
typedef /* const */ unsigned char const_des_cblock[8];
#define DES_LONG unsigned int
typedef struct DES_ks
{
   union
        {
         des_cblock cblock;
         /* make sure things are correct size on machines with
          * 8 byte longs */
         DES_LONG deslong[2];
         } ks[16];
} des_key_schedule;

int des_set_key(const_des_cblock *key,des_key_schedule *schedule);

void des_ecb_encrypt(const_des_cblock *input,des_cblock *output,
                     des_key_schedule *ks,int enc);



void des_ecb3_encrypt(const_des_cblock *input, des_cblock *output,
                      des_key_schedule *ks1,des_key_schedule *ks2,
                      des_key_schedule *ks3, int enc);


void des_ede3_cbc_encrypt(const unsigned char *, unsigned char *,
                         long, des_key_schedule *,
                         des_key_schedule *, des_key_schedule *, des_cblock *,int);

#ifdef  __cplusplus
}
#endif

#define DES_ENCRYPT	1
#define DES_DECRYPT	0


class CCrypt
{
public:
    CCrypt();
    ~CCrypt();

     QString Encrypt( QString strSource,
                      QString strKey,
                      bool bConvertIn = true,
                      bool bConvertOut = true );

     QString Decrypt( QString strSource,
                      QString strKey,
                      bool bConvertIn = true,
                      bool bConvertOut = true );
     static QString toBin( QString strScr );
     static QString toHex( QString strSrc );

    private:


        static int  bin2hexstr(char *hexstr,
                        int size,
                        unsigned char *bin,
                        int num );

        static int hexstr2bin( unsigned char *bin,
                        int num,
                        const unsigned char *hexstr,
                        int size );

        QString Process( QString strSource,
                         QString strKey,
                         int iDesAction,
                         bool bConvertIn,
                         bool bConvertOut );


};

#endif // CCrypt_H
