#include "qradsplashscreen.h"

#include <QApplication>
#include <QStringList>
#include <QPixmap>
#include <QDebug>
#include "qraddebug.h"

QRadSplashScreen::QRadSplashScreen(const QPixmap& pix, const Qt::WindowFlags &flags)
    : QSplashScreen(pix, flags)
{
    m_message       = "";

    m_hasCustomImage = false;

    try
    {
        loadCustomImage();
    }
    catch( ...)
    {
        qDebug() << "Could not load custom pix.";
    }
}

void QRadSplashScreen::loadCustomImage()
{
/*    QString appName = QApplication::arguments().at(0);

    if (appName.contains("QRad-paf"))
        appName = "QRad-paf";
    else
        appName = "QRad-sg";

    QString customPicture = QRadConfig::GetParamValue(QString("%1/splashscreen").arg(appName));

    if (customPicture.isEmpty())
    {
        debug_message("[QRadSplashScreen] Loading default image...\n")
        return;
    }

    QFile pixFile(customPicture);

    if (!pixFile.exists())
    {
        debug_message("[QRadSplashScreen] File does not exists: %s\n", pixFile.fileName().toUtf8().data())
        return;
    }

    debug_message("[QRadSplashScreen] Loading Custom Image: %s\n", pixFile.fileName().toUtf8().data())

    QPixmap pix(pixFile.fileName());

    setPixmap(pix);

    m_customMask = pix.mask();

    m_hasCustomImage = true;
*/
}

void QRadSplashScreen::show()
{
    if (m_hasCustomImage)
    {
        debug_message("[QRadSplashScreen] Using custom mask, this mask will be ignored.\n");
        setMask(m_customMask);
    }

    QSplashScreen::show();
}

void QRadSplashScreen::finish(QWidget *w)
{
    QSplashScreen::finish(w);
}

void QRadSplashScreen::drawContents(QPainter *painter)
{
    if (m_message.length() == 0)
        return;

    painter->drawText(22,
                      rect().height() - 20,
                      m_message + "...");
}

void QRadSplashScreen::setMessage(const QString &message)
{
    m_message = message;

    this->showMessage(m_message, Qt::AlignLeft, Qt::white);
}
