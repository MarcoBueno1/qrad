#ifndef QRadSPLASHSCREEN_H
#define QRadSPLASHSCREEN_H

#include <QApplication>
#include <QSplashScreen>
#include <QPainter>
#include <QTimer>
//#include <QThread>
#include <QBitmap>

class QRadSplashScreen : public QSplashScreen
{
    Q_OBJECT
public:
    QRadSplashScreen(const QPixmap &pix, const Qt::WindowFlags &flags);

    void setMessage(const QString& message);
    void show();
    void finish(QWidget *w);
    void loadCustomImage();

protected:
    void drawContents(QPainter *painter);

private:
    QString m_message;
    QBitmap m_customMask;
    bool    m_hasCustomImage;
};

#endif // QRadSPLASHSCREEN_H
