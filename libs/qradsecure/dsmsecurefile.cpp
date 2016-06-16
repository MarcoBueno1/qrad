#include "qraddebug.h"
#include "dsmsecurefile.h"

DSMSecureFile::DSMSecureFile()
        : QFile()
{
    m_secure = new DSMSecure();
    m_fd = NULL;
}

DSMSecureFile::DSMSecureFile(const QString& name)
        : QFile(name)
{
    m_secure = new DSMSecure();
    m_fd = NULL;
}

bool DSMSecureFile::open(int fd, OpenMode flags)
{
    Q_UNUSED(fd);

    if (!m_secure->initialize())
        return false;

    return open(flags);
}

bool DSMSecureFile::open(FILE *f, OpenMode flags)
{
    Q_UNUSED(flags);

    if (!m_secure->initialize())
        return false;

    m_fd = f;

    return true;
}

bool DSMSecureFile::open(OpenMode flags)
{
    char mode[4];

    if (!m_secure->initialize())
        return false;

    if (flags == QIODevice::ReadWrite)       sprintf(mode, "rb+");
    else if (flags == QIODevice::ReadOnly)   sprintf(mode, "rb");
    else if (flags == QIODevice::WriteOnly)  sprintf(mode, "wb");
    else                                     sprintf(mode, "ab");

    m_fd = fopen(fileName().toUtf8(), mode);

    if (NULL == m_fd)
        return false;

    return true;
}

void DSMSecureFile::close()
{
    m_secure->terminate();

    fclose(m_fd);
    m_fd = NULL;
}

qint64 DSMSecureFile::write(const QByteArray& data)
{
    return writeData(data.data(), data.size());
}

qint64 DSMSecureFile::write(const char *data, qint64 len)
{
    return writeData(data, len);
}

qint64 DSMSecureFile::writeData(const char* data, qint64 len)
{
    QByteArray plainData(data, len);
    QByteArray encryptedData;
    qint64 bytesWritten = 0;
    
    encryptedData.clear();

    encryptedData.append(m_secure->encrypt(plainData));

    bytesWritten = fwrite(encryptedData.data(), 1, encryptedData.size(), m_fd);

    if (bytesWritten <= 0)
        return 0;

    m_decryptedPosition += len;

    return len;
}

QByteArray DSMSecureFile::readAll()
{
    debug_message("[DSMSecureFile] -->readAll L:%d\n", __LINE__ );

    QByteArray decryptedContent;
    char * content = NULL;

    if (!isOpen())
    {
        debug_message("[DSMSecureFile] -->readAll warning, não aberto L:%d\n", __LINE__ );

        open(QIODevice::ReadOnly);
        if(m_fd == NULL )
        {
           debug_message("[DSMSecureFile] <--readAll Erro, %s não pode ser aberto L:%d\n", fileName().toUtf8().data(),__LINE__ );
           return "";
        }
    }

    fseek(m_fd, 0, SEEK_END);
    qint64 fileSize = ftell(m_fd);
    fseek(m_fd, 0, SEEK_SET);

    content = new char[fileSize];

    if (fread(content, 1, fileSize, m_fd) <= 0)
    {
        delete content;
        debug_message("[DSMSecureFile] <--readAll erro L:%d\n", __LINE__ );
        return decryptedContent;
    }

    decryptedContent = m_secure->decrypt(QByteArray(content, fileSize));
    delete content;

    debug_message("[DSMSecureFile] <--readAll L:%d\n", __LINE__ );
    return decryptedContent;
}

QByteArray DSMSecureFile::read(qint64 maxlen)
{
    qint64 bytesRead;
    char *buffer = new char[maxlen];

    bytesRead = readData(buffer, maxlen);

    QByteArray content(buffer, bytesRead);

    return content;
}

QByteArray DSMSecureFile::readLine(qint64 maxlen)
{
    return QFile::readLine(maxlen);
}

qint64 DSMSecureFile::read(char *data, qint64 maxlen)
{
    return readData(data, maxlen);
}

qint64 DSMSecureFile::readLineData(char *data, qint64 maxlen)
{
    return QFile::readLineData(data, maxlen);
}

qint64 DSMSecureFile::readData(char* data, qint64 maxlen)
{
    QByteArray decryptedData = readAll();
    qint64 decryptedSize = decryptedData.size();

    qint64 sizeToRead = ((m_decryptedPosition + maxlen) > decryptedSize) ? decryptedSize - m_decryptedPosition : maxlen;

    if (sizeToRead < 0)
    {
        return 0;
    }

    memcpy(data, decryptedData.data() + m_decryptedPosition, sizeToRead);

    return sizeToRead;
}

qint64 DSMSecureFile::size() const
{/*
    QByteArray decryptedContent;
    char * content = NULL;

    if (!isOpen())
    {
        return 0;
    }

    qint64 fileSize = 0;

    fseek(m_fd, 0, SEEK_END);
    fileSize = ftell(m_fd);
    fseek(m_fd, 0, SEEK_SET);

    content = new char[fileSize];

    if (fread(content, 1, fileSize, m_fd) <= 0)
    {
        delete content;
        return 0;
    }

    decryptedContent = m_secure->decrypt(QByteArray(content, fileSize));
    delete content;

    return decryptedContent.size();
    */

    return QFile::size();
}

bool DSMSecureFile::getChar(char *c)
{
    return QFile::getChar(c);
}

bool DSMSecureFile::isReadable() const
{
    return QFile::isReadable();
}

bool DSMSecureFile::atEnd()
{
    return (size() >= m_decryptedPosition);
}

bool DSMSecureFile::canReadLine() const
{
    return false;
}

bool DSMSecureFile::isSequential() const
{
    return QFile::isSequential();
}

bool DSMSecureFile::isWritable() const
{
    return true;
}

QIODevice::OpenMode DSMSecureFile::openMode() const
{
    return QFile::openMode();
}

qint64 DSMSecureFile::pos() const
{
    return m_decryptedPosition;
}

bool DSMSecureFile::seek(qint64 offset)
{
    if (offset >= size())
        return false;

    m_decryptedPosition = offset;

    return true;
}

bool DSMSecureFile::isOpen() const
{
    return (NULL != m_fd);
}

bool DSMSecureFile::flush()
{
    if (!isOpen())
        return false;

    fflush(m_fd);

    return true;
}
