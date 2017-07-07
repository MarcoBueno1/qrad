#ifndef QRadSECURE_H
#define QRadSECURE_H

#include <QString>
#include <QByteArray>
#include <QDebug>

#include "ssltypes.h"

class QRadSecure {
public:
    QRadSecure();
    ~QRadSecure();

    bool        initialize(void);
    void        terminate(void);
    QByteArray  md5(QByteArray content);
    QString     strMd5(QByteArray content);
    QString     strMd5(QString content);
    QString     bin2hex(QByteArray content);
    QByteArray  encrypt(QByteArray content);
    QByteArray  decrypt(QByteArray encryptedContent);
    QByteArray  privateEncrypt(QByteArray content);
    QByteArray  publicDecrypt(QByteArray content);
    qint64      blockSize(void);
    void        cpykey(char* cBuffer, const int* iBuffer, int size);

    static QString publicModulus();
    static QString publicExponent();    
private:
    TSSLFunctions* m_sslFunctions;
    bool           m_initialized;
    RSA*           m_pubKey;
    RSA*           m_privKey;
};

#endif // QRadSECURE_H
