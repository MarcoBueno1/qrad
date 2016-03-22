#ifndef QRadSECUREFILE_H
#define QRadSECUREFILE_H

#include <QFile>
#include <QString>

#include <stdio.h>
#include "qradsecure.h"

class QRadSecureFile : public QFile
{
public:
    QRadSecureFile();
    QRadSecureFile(const QString& name);

    bool open(int fd, OpenMode flags);
    bool open(FILE *f, OpenMode flags);
    bool open(OpenMode flags);

    void close();


    qint64 write(const QByteArray& data);
    qint64 write(const char* data, qint64 len);
    qint64 writeData(const char* data, qint64 len);

    QByteArray readAll();
    QByteArray read(qint64 maxlen);
    QByteArray readLine(qint64 maxlen);
    qint64 readLineData(char *data, qint64 maxlen);
    qint64 read(char *data, qint64 maxlen);
    qint64 readData(char *data, qint64 maxlen);

    bool getChar(char *c);

    bool isReadable() const;
    bool isWritable() const;
    bool isSequential() const;
    bool atEnd();
    bool canReadLine() const;

    qint64 pos() const;
    qint64 size() const;
    bool isOpen() const;
    bool seek(qint64 offset);
    bool flush();

    QIODevice::OpenMode openMode() const;

private:
    QRadSecure* m_secure;
    FILE*      m_fd;
    qint64     m_decryptedPosition;
};

#endif // QRadSECUREFILE_H
