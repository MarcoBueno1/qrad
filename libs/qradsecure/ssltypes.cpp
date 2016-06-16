#include "qraddebug.h"
#include "ssltypes.h"
#include <QDebug>

#define LOAD_FUNCTION(ptr, type, name)   \
    functions->ptr = (type)DSM_GetProcAddress(functions->hInstance, name); \
    if (NULL == functions->ptr)  { debug_message("[SSLTYPES] Lib: %s DSM_GetProcAddress(%s) = false L:%d", OPENSSL_LIB_NAME, name, __LINE__); \
    DSM_FreeLibrary(functions->hInstance); return false; }

bool SSLFunctions_Initialize(TSSLFunctions* functions)
{
    if (NULL == functions)
        return false;

    functions->hInstance = DSM_LoadLibrary(OPENSSL_LIB_NAME);

    if (!functions->hInstance)
    {
        qDebug() << "Retornando false para:" << OPENSSL_LIB_NAME;
        return false;
    }


    LOAD_FUNCTION(BN_hex2bn, BN_HEX2BN, "BN_hex2bn");
    LOAD_FUNCTION(BN_bin2bn, BN_BIN2BN, "BN_bin2bn");
    LOAD_FUNCTION(BN_bn2hex, BN_BN2HEX, "BN_bn2hex");
    LOAD_FUNCTION(BN_free,   BN_FREE,   "BN_free");

    LOAD_FUNCTION(RSA_public_encrypt, RSA_PUBLIC_ENCRYPT, "RSA_public_encrypt");
    LOAD_FUNCTION(RSA_private_decrypt, RSA_PRIVATE_DECRYPT, "RSA_private_decrypt");
    LOAD_FUNCTION(RSA_private_encrypt, RSA_PRIVATE_ENCRYPT, "RSA_private_encrypt");
    LOAD_FUNCTION(RSA_public_decrypt, RSA_PUBLIC_DECRYPT, "RSA_public_decrypt");

    LOAD_FUNCTION(RSA_new, RSA_NEW, "RSA_new");
    LOAD_FUNCTION(RSA_free, RSA_FREE, "RSA_free");
    LOAD_FUNCTION(RSA_size, RSA_SIZE, "RSA_size");

    LOAD_FUNCTION(MD5_init,     MD5_INIT, "MD5_Init");
    LOAD_FUNCTION(MD5_update,   MD5_UPDATE, "MD5_Update");
    LOAD_FUNCTION(MD5_final,    MD5_FINAL, "MD5_Final");

    LOAD_FUNCTION(ERR_print_errors_fp, ERR_PRINT_ERRORS_FP, "ERR_print_errors_fp");

    return true;
}

void SSLFunctions_Terminate(TSSLFunctions* functions)
{
    if ((NULL == functions) || (NULL == functions->hInstance))
        return;

    DSM_FreeLibrary(functions->hInstance);
}
