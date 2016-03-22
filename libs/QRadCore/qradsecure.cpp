#include "qraddebug.h"
#include "qradsecure.h"
#include "securekey.h"

QRadSecure::QRadSecure()
{
    debug_message("[QRadSecure] -->QRadSecure()..\n");

    m_sslFunctions = new TSSLFunctions;

    m_initialized = initialize();

    debug_message("[QRadSecure] <--QRadSecure()..\n");
}

QRadSecure::~QRadSecure()
{
    debug_message("[QRadSecure] -->~QRadSecure()..\n");

    if (m_initialized)
        terminate();

    delete m_sslFunctions;
    debug_message("[QRadSecure] <--~QRadSecure()..\n");
}

void QRadSecure::cpykey(char *cBuffer, const int *iBuffer, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        cBuffer[i] = (char)(iBuffer[i] - 1000);
    }

    cBuffer[size] = '\0';
}

QString QRadSecure::publicExponent()
{
    QString output = "";

    int count = 0;

    while (count < PUBLIC_EXPONENT_SIZE)
    {
        int chr = g_PUBLIC_EXPONENT[count] - 1000;
        output += (char)chr;

        count++;
    }
    return output;
}

QString QRadSecure::publicModulus()
{
    QString output = "";
    int count = 0;

    while (count < MODULUS_SIZE)
    {
        int chr = g_MODULUS[count] - 1000;
        output += (char)chr;

        count++;
    }

    return output;
}

bool QRadSecure::initialize()
{
/*    if( m_initialized )
        return true;
        */

    if (!SSLFunctions_Initialize(m_sslFunctions))
    {
        qDebug() << "SSLFunctions_Initialize: Failed";
        return false;
    }

    m_pubKey    = m_sslFunctions->RSA_new();

    if (NULL == m_pubKey)
    {
        qDebug() << "pub RSA_new(): Failed";
        SSLFunctions_Terminate(m_sslFunctions);
        return false;
    }

    m_privKey   = m_sslFunctions->RSA_new();

    if (NULL == m_privKey)
    {
        qDebug() << "priv RSA_new(): Failed";
        SSLFunctions_Terminate(m_sslFunctions);
        return false;
    }

    m_pubKey->n = m_privKey->n = NULL;
    m_pubKey->e = m_privKey->e = NULL;
    m_pubKey->d = m_privKey->d = NULL;
    m_pubKey->p = m_privKey->p = NULL;
    m_pubKey->q = m_privKey->q = NULL;
    m_pubKey->dmp1 = m_privKey->dmp1 = NULL;
    m_pubKey->dmq1 = m_privKey->dmq1 = NULL;
    m_pubKey->iqmp = m_privKey->iqmp = NULL;

    char *modulus           = new char[MODULUS_SIZE + 1];
    char *public_exponent   = new char[PUBLIC_EXPONENT_SIZE + 1];
    char *private_exponent  = new char[PRIVATE_EXPONENT_SIZE + 1];

    cpykey(modulus, g_MODULUS, MODULUS_SIZE);
    cpykey(public_exponent, g_PUBLIC_EXPONENT, PUBLIC_EXPONENT_SIZE);
    cpykey(private_exponent, g_PRIVATE_EXPONENT, PRIVATE_EXPONENT_SIZE);

    modulus[MODULUS_SIZE] = '\0';
    public_exponent[PUBLIC_EXPONENT_SIZE] = '\0';
    private_exponent[PRIVATE_EXPONENT_SIZE] = '\0';

    m_sslFunctions->BN_hex2bn(&m_pubKey->n, modulus);
    m_sslFunctions->BN_hex2bn(&m_pubKey->e, public_exponent);

    m_sslFunctions->BN_hex2bn(&m_privKey->n, modulus);
    m_sslFunctions->BN_hex2bn(&m_privKey->e, public_exponent);
    m_sslFunctions->BN_hex2bn(&m_privKey->d, private_exponent);

    delete modulus;
    delete public_exponent;
    delete private_exponent;

    m_initialized = true;

    return true;
}

void QRadSecure::terminate()
{
    m_sslFunctions->RSA_free(m_pubKey);
    m_sslFunctions->RSA_free(m_privKey);
    SSLFunctions_Terminate(m_sslFunctions);

    m_initialized = false;
}

QByteArray  QRadSecure::privateEncrypt(QByteArray content)
{
    QByteArray encryptedContent;
    unsigned char *pContent = (unsigned char*)content.data();
    unsigned char *buffer = NULL;
    int bytesEncrypted = 0;
    int totalBytes = 0;
    int bufferSize = 0;

    bufferSize = m_sslFunctions->RSA_size(m_pubKey) * 3;

    encryptedContent.clear();

    buffer = new unsigned char[bufferSize];

    while (content.size() > totalBytes)
    {
        int inSize = (content.size() - totalBytes);

        inSize = (inSize >= bufferSize) ? bufferSize : inSize;

        bytesEncrypted = m_sslFunctions->RSA_private_encrypt( 128,
                                                              pContent + totalBytes,
                                                              buffer,
                                                              m_privKey,
                                                              RSA_NO_PADDING);
        if (bytesEncrypted < 0)
        {
            qDebug() << "Error during encryption: ";
            m_sslFunctions->ERR_print_errors_fp(stdout);
            fflush(stdout);
            break;
        }

        encryptedContent.append((char*)buffer, bytesEncrypted);

        totalBytes += inSize;
    }

    free(buffer);

    return encryptedContent;
}

QByteArray  QRadSecure::publicDecrypt(QByteArray encryptedContent)
{
    QByteArray decryptedContent;
    unsigned char *pEncrypted = (unsigned char*)encryptedContent.data();
    unsigned char *buffer = NULL;
    int bytesDecrypted = 0;
    int totalBytes = 0;
    int bufferSize = 0;

    decryptedContent.clear();

    bufferSize = m_sslFunctions->RSA_size(m_privKey);

    if (bufferSize <= 0)
    {
        return decryptedContent;
    }

    buffer = new unsigned char[bufferSize];

    while (encryptedContent.size() > totalBytes)
    {
        int inSize = encryptedContent.size() - totalBytes;

        inSize = (inSize > bufferSize) ? (bufferSize) : inSize;

        bytesDecrypted = m_sslFunctions->RSA_public_decrypt( bufferSize,
                                                             pEncrypted + totalBytes,
                                                             buffer,
                                                             m_pubKey,
                                                             RSA_PKCS1_PADDING);

        if (bytesDecrypted < 0)
            break;

        decryptedContent.append((char*)buffer, bytesDecrypted);

        totalBytes += inSize;
    }

    free(buffer);

    return decryptedContent;
}

QByteArray QRadSecure::encrypt(QByteArray content)
{
    QByteArray encryptedContent;
    unsigned char *pContent = (unsigned char*)content.data();
    unsigned char *buffer = NULL;
    int bytesEncrypted = 0;
    int totalBytes = 0;
    int bufferSize = 0;

    bufferSize = m_sslFunctions->RSA_size(m_pubKey);

    encryptedContent.clear();

    buffer = new unsigned char[bufferSize];

    while (content.size() > totalBytes)
    {
        int inSize = (content.size() - totalBytes);

        inSize = (inSize >= (bufferSize/2)) ? (bufferSize/2) : inSize;

        bytesEncrypted = m_sslFunctions->RSA_public_encrypt( inSize,
                                                             pContent + totalBytes,
                                                             buffer,
                                                             m_pubKey,
                                                             RSA_PKCS1_PADDING);
        if (bytesEncrypted < 0)
        {
            qDebug() << "Error during decryption: ";
            m_sslFunctions->ERR_print_errors_fp(stdout);
            fflush(stdout);
            break;
        }

        encryptedContent.append((char*)buffer, bytesEncrypted);

        totalBytes += inSize;
    }

    free(buffer);

    return encryptedContent;
}

QByteArray QRadSecure::decrypt(QByteArray encryptedContent)
{
    debug_message("[QRadSecure] -->decrypt()..\n");

    QByteArray decryptedContent;
    unsigned char *pEncrypted = (unsigned char*)encryptedContent.data();
    unsigned char *buffer = NULL;
    int bytesDecrypted = 0;
    int totalBytes = 0;
    int bufferSize = 0;

    decryptedContent.clear();

    bufferSize = m_sslFunctions->RSA_size(m_privKey);

    if (bufferSize <= 0)
    {
        return decryptedContent;
    }

    buffer = new unsigned char[bufferSize];

    while (encryptedContent.size() > totalBytes)
    {
        int inSize = encryptedContent.size() - totalBytes;

        inSize = (inSize > bufferSize) ? (bufferSize) : inSize;

        bytesDecrypted = m_sslFunctions->RSA_private_decrypt(bufferSize,
                                                             pEncrypted + totalBytes,
                                                             buffer,
                                                             m_privKey,
                                                             RSA_PKCS1_PADDING);

        if (bytesDecrypted < 0)
            break;

        decryptedContent.append((char*)buffer, bytesDecrypted);

        totalBytes += inSize;
    }

    free(buffer);

    return decryptedContent;
}

qint64 QRadSecure::blockSize()
{
    return m_sslFunctions->RSA_size(m_privKey);
}

QByteArray QRadSecure::md5(QByteArray content)
{
    MD5_CTX ctx;
    unsigned char result[16];

    memset(result, 0, sizeof(result));
    memset(&ctx, 0, sizeof(ctx));

    if (m_sslFunctions->MD5_init(&ctx) < 0)
    {
        return NULL;
    }

    m_sslFunctions->MD5_update(&ctx, content.data(), content.length());

    m_sslFunctions->MD5_final(result, &ctx);

    return QByteArray((const char*)result, sizeof(result));
}

QString QRadSecure::strMd5(QByteArray content)
{
    QString result = "";

    if (!m_initialized)
    {
        if (!initialize())
            return "";
    }

    result = bin2hex(md5(content));

    if (!m_initialized)
    {
        terminate();
    }

    return result;
}

QString QRadSecure::strMd5(QString content)
{
    return strMd5(content.toLatin1());
}

QString QRadSecure::bin2hex(QByteArray content)
{
    BIGNUM* num;
    char *hex;

    num = m_sslFunctions->BN_bin2bn((unsigned char*)content.data(), content.size(), NULL);

    hex = m_sslFunctions->BN_bn2hex(num);

    QString strResult = QString(hex);

    m_sslFunctions->BN_free(num);

    return strResult;
}
