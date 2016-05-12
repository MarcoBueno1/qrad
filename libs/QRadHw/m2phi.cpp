#include "m2phi.h"
#include <stdlib.h>
#include <fcntl.h>
#include "hdreg.h"
#include <stdio.h>
#include <QProcess>
#include <QNetworkInterface>
#include "dsmdebug.h"

#if defined(WIN32)
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

QString M2phi::GetHDSerial(void)
{
    static QString hdSerial = QString::fromUtf8(INVALID_HDSERIAL);

#ifdef __linux__
    if ((INVALID_HDSERIAL == hdSerial) || (hdSerial.isEmpty()))
    {
        hdSerial = ReadHDSerial();
//        if( hdSerial == INVALID_HDSERIAL)
//            hdSerial = GetMacAddress();
    }
#endif

    if( INVALID_HDSERIAL == hdSerial)
    {
        hdSerial = GetMacAddress();
        hdSerial.replace(":", "_");

    }

    hdSerial.replace("-", "_");
    hdSerial.truncate(20);

    debug_message("Serial:%s\n", hdSerial.toUtf8().data());
    return hdSerial;
}

QString M2phi::ReadHDSerial()
{
#ifdef __linux__
    struct hd_driveid id;
    int fd = open("/dev/hda", O_RDONLY);

    if ( fd < 0 )
    {
        fd = open("/dev/sda", O_RDONLY);
    }

    if (fd < 0)
    {
        perror("/dev/sda");
    }

    memset(&id,0,sizeof(struct hd_driveid));

    if(!ioctl(fd, HDIO_GET_IDENTITY, &id))
    {
        QString s1;
        // id.serial_no does not save space for string termination
        const int serialSize = sizeof(id.serial_no);
        char buf[serialSize + 1];
        memcpy(buf, id.serial_no, serialSize);
        buf[serialSize] = '\0';

        s1.sprintf("%s", buf);
        s1 = s1.trimmed();

        if ( s1.contains("-") )
            s1.replace("-","_");

        close(fd);

        return s1;
    }

    perror("ioctl");
    return  INVALID_HDSERIAL;
#else
    int nCount = 0;
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start("wmic path win32_physicalmedia get SerialNumber");
    if (!process.waitForStarted())
        return INVALID_HDSERIAL;
    if (!process.waitForFinished())
        return INVALID_HDSERIAL;

    //    qDebug() << process.readAll();
    QList<QByteArray> output = process.readAll().trimmed().split('\n');
    for( int iIndex =0; iIndex < output.count(); iIndex++ )
    {
        QString strLine = output.at(iIndex);
        if( strLine.contains("SerialNumber"))
            nCount++;
        else if( nCount == 1)
        {
//            QString ret = QByteArray::fromHex(strLine.toUtf8()).trimmed();
            QString ret = strLine.toUtf8().trimmed();

            debug_message("M2Phi Line:%d ret=%d ret:%s\n", __LINE__, nCount, ret.toUtf8().data());
            return ret;
        }
        debug_message("M2Phi Line:%d Text:%s\n", nCount, strLine.toUtf8().data());

    }

    return INVALID_HDSERIAL;
#endif
}


QString M2phi::GetMacAddress()
{
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            return netInterface.hardwareAddress();
    }
    return INVALID_HDSERIAL;
}

