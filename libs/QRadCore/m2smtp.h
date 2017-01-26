#ifndef M2SMTP_H
#define M2SMTP_H

#include <QApplication>
#include <QMessageBox>
#include <QtDebug>
#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QLocale>
#include <QList>
#include <QFileInfo>
#include "SmtpMime"


class M2Smtp : public QObject
{
    Q_OBJECT
    
public:
    M2Smtp( const QString &server,
            const QString &login,
            const QString &pass,
            const QString &port,
            const QString &from,
            const QList<QString> to,
            const bool popupMessage,
            const QList<QString> &bcc,
            const QString &subject,
            const QString &body,
            const QList<QString> file, const bool html = false);
    ~M2Smtp();
    
    void send();
    void getM2SmtpMsgError(int error, QString &);
    
public slots:
    void mailSent(QString text);
signals:
    void status( const int);
    void completed(bool, const int);

private slots:

    void stateChanged(QAbstractSocket::SocketState socketState);
    void errorReceived(QAbstractSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();
private:
    EmailAddress* stringToEmail(const QString &str);
    void setM2SmtpMsgError(QString errorString);

    QString message;
    QTextStream *t;
    QTcpSocket *socket;
    QString server;
    QString loginpass;
    QString from;
    QList<QString> rcpt;
    int rcptIndex;
    QString response;
    QString errorMsgSocket;
    int errorSocket;
    enum states{Rcpt,Mail,Data,Init,Body,Quit,Close,Auth};
    int state;
};
#endif /* M2SMTP_H */
