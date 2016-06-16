#ifndef SSLTYPES_H
#define SSLTYPES_H

#include <string.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/rsa.h>
#include <openssl/bn.h>
#include <openssl/md5.h>

#include <dsmsystem.h>

#ifndef __cplusplus
#include <c.h> // definitions of bool, true and false
#endif

#ifdef _WIN32
    #define OPENSSL_LIB_NAME    "libeay32.dll"
#elif  __linux__ // linux
    #define OPENSSL_LIB_NAME    "libssl.so"
#else
    #define OPENSSL_LIB_NAME    "libssl.dylib"
#endif

typedef int (*BN_HEX2BN)(BIGNUM**, const char*);   // BN_hex2bn
typedef BIGNUM* (*BN_BIN2BN)(const unsigned char *s, int len, BIGNUM *ret);
typedef char* (*BN_BN2HEX)(const BIGNUM *a);
typedef void (*BN_FREE)(BIGNUM*);
typedef int (*RSA_PUBLIC_ENCRYPT)(int, unsigned char*, unsigned char*, RSA*, int);
typedef int (*RSA_PRIVATE_DECRYPT)(int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
typedef int (*RSA_PRIVATE_ENCRYPT)(int, unsigned char*, unsigned char*, RSA*, int);
typedef int (*RSA_PUBLIC_DECRYPT)(int flen, unsigned char *from, unsigned char *to, RSA *rsa, int padding);
typedef int (*MD5_INIT)(MD5_CTX *c);
typedef int (*MD5_UPDATE)(MD5_CTX*, const void*, unsigned long);
typedef int (*MD5_FINAL)(unsigned char *, MD5_CTX*);
typedef int (*SSL_LIBRARY_INIT)(void);
typedef void (*SSL_LOAD_ERROR_STRINGS)(void);
typedef RSA* (*RSA_NEW)(void);
typedef void (*RSA_FREE)(RSA*);
typedef int (*RSA_SIZE)(RSA*);
typedef void (*ERR_PRINT_ERRORS_FP)(FILE *fp);

typedef struct TSSLFunctions_tag
{
    HINSTANCE               hInstance;

    BN_HEX2BN               BN_hex2bn;
    BN_BIN2BN               BN_bin2bn;
    BN_BN2HEX               BN_bn2hex;
    BN_FREE                 BN_free;

    RSA_PUBLIC_ENCRYPT      RSA_public_encrypt;
    RSA_PRIVATE_DECRYPT     RSA_private_decrypt;
    RSA_PRIVATE_ENCRYPT     RSA_private_encrypt;
    RSA_PUBLIC_DECRYPT      RSA_public_decrypt;

    RSA_NEW                 RSA_new;
    RSA_FREE                RSA_free;
    RSA_SIZE                RSA_size;

    MD5_INIT                MD5_init;
    MD5_UPDATE              MD5_update;
    MD5_FINAL               MD5_final;

    ERR_PRINT_ERRORS_FP      ERR_print_errors_fp;
}TSSLFunctions;

bool SSLFunctions_Initialize(TSSLFunctions* functions);

void SSLFunctions_Terminate(TSSLFunctions* functions);

#endif // SSLTYPES_H
