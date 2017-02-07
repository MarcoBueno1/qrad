#include "m2smtp.h"


#define UNCHECKEDANEX "uncheckedfile" // use this define in files unchecked ( anex in email )

EmailAddress* M2Smtp::stringToEmail(const QString &str)
{
    int p1 = str.indexOf("<");
    int p2 = str.indexOf(">");

    if (p1 == -1)
    {
        // no name, only email address
        return new EmailAddress(str);
    }
    else
    {
        return new EmailAddress(str.mid(p1 + 1, p2 - p1 - 1), str.left(p1));
    }

}


M2Smtp::M2Smtp(const QString &server,
            const QString &login,
            const QString &pass,
            const QString &port,
            const QString &from,
            const QList<QString> to,
            const bool popupMessage,
            const QList<QString> &bcc,
            const QString &subject,
            const QString &body,
            const QList<QString> file, const bool html )
{

    EmailAddress *sender = stringToEmail(from);

    SmtpClient smtp (server, port.toInt(), SmtpClient::SslConnection);

    MimeMessage message;

    message.setSender(sender);
    message.setSubject(subject);

    for (int i = 0; i < to.size(); ++i)
         message.addRecipient(stringToEmail(to.at(i)));

    MimeHtml content;

    if(html)
        content.setHtml(body);
    else
        content.setText(body);

    message.addPart(&content);
    QMessageBox okMessage (0);

    for (int i = 0; i < file.count(); ++i)
    {
        message.addPart(new MimeAttachment(new QFile(file.at(i))));
    }

    if (!smtp.connectToHost() && popupMessage)
    {
        okMessage.setText(QString::fromUtf8("email: Sem Conexão"));
        okMessage.exec();
        return;
    }

    if (!smtp.login(login, pass)&& popupMessage)
    {
        okMessage.setText(QString::fromUtf8("email: Erro de Autenticação"));
        okMessage.exec();
        return;
    }

    if (!smtp.sendMail(message) && popupMessage)
    {
        okMessage.setText(QString::fromUtf8("email: Erro de Envio"));
        okMessage.exec();
        return;
    }
//    else if( popupMessage )
    {
        okMessage.setText(QString::fromUtf8("Email Enviado!"));
        okMessage.exec();
    }

    smtp.quit();


/*
    QString strText;

    if(html)
    {
        strText.append("--000e0cdfc5ccf3ea1f04c844bb85\r\n");
        strText.append("Content-Type: text/plain; charset=UTF8\n\r\n");
    }
    strText.append(body);

    if(to.count() > 0){
//        message.append( "To: ");
        for(int i = 0; i < to.count() ; i++){
            //QScopedPointer<Smtp> smtp(new Smtp(login, pass, server, port.toInt()));
            Smtp *smtp = new Smtp(login, pass, server, port.toInt());
            if( popupMessage && (i == to.count()-1)) /// apresenta popup apenas uma vez.
                connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));


            smtp->sendMail(from, to.at(i) , subject, body);

//            message.append("<" + to.at(i) + ">" + ",");
//            rcpt.append("<" + to.at(i) + ">");
//            rcpt.insert(i, to.at(i));
        }
        message.append("\n");
    }
*/


/*    (void)bcc;

    socket = new QTcpSocket( this );

    connect( socket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
    connect( socket, SIGNAL( connected() ), this, SLOT( connected() ) );
    connect( socket, SIGNAL(error(QAbstractSocket::SocketError)), this, 
             SLOT(errorReceived(QAbstractSocket::SocketError)));
    connect( socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, 
             SLOT(stateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(disconnected()), this, 
            SLOT(disconnected()));;

    //Generate date
    QTime myTime = QDateTime::currentDateTime().toUTC().time();
    QLocale myLocal = QLocale(QLocale::C);
    QString date = myLocal.toString(QDate::currentDate(), "ddd, dd MMM yyyy ");
    date += myLocal.toString(myTime, "hh:mm:ss");
    date += " +0000 (UTC)";

    message.clear();

    this->loginpass = QByteArray().append(QChar::Null)
                      .append(login)
                      .append(QChar::Null)
                      .append(pass)
                      .toBase64();

//    rcpt = bcc;

    message.append("MIME-Version: 1.0 \n");

    message.append("Date: " + date + "\n");
    message.append("Subject: "  + subject + "\n");

    message.append("From: " + from + "\n");

    if(to.count() > 0){
        message.append( "To: ");
        for(int i = 0; i < to.count() ; i++){
            message.append("<" + to.at(i) + ">" + ",");
            rcpt.append("<" + to.at(i) + ">");
//            rcpt.insert(i, to.at(i));
        }
        message.append("\n");
    }

    rcptIndex = 0;

    message.append("Content-Type: multipart/mixed; boundary=000e0cdfc5ccf3ea2404c844bb87\r\n");
    message.append("\r\n");

    message.append("--000e0cdfc5ccf3ea2404c844bb87\r\n");
    message.append("Content-Type: multipart/related; boundary=000e0cdfc5ccf3ea2104c844bb86\r\n");
    message.append("\r\n");

    message.append("--000e0cdfc5ccf3ea2104c844bb86\r\n");
    message.append("Content-Type: multipart/alternative; boundary=000e0cdfc5ccf3ea1f04c844bb85\r\n");
    message.append("\r\n");

    if(html)
    {
        message.append("--000e0cdfc5ccf3ea1f04c844bb85\r\n");
        message.append("Content-Type: text/plain; charset=UTF8\n\r\n");
    }

    message.append("Por Favor carregue suas imagens ou instale um cliente de email de leitura de html.");
    message.append("----------------------------------------------------------------------------------");
    message.append("\n\r\n");
    message.append("--000e0cdfc5ccf3ea1f04c844bb85\r\n");

    QFile fileLogo(":/png/logom2.png");

    if(!fileLogo.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error ao abrir a figura da logo marca m2smart";
    }

    QString strLogo = fileLogo.readAll().toBase64();
    fileLogo.close();

    message.append("Content-Type: text/html; charset=UTF8; format=flowed\r\n");
    message.append("\r\n");
    message.append(body);
    message.append("<div>");
    message.append("<p></p>");
    message.append("<img src=\"cid:marca_m2smart@m2smart.com.br\"/>");
    message.append("<p><a href=\"http://www.m2smart.com.br\">www.m2smart.com.br</a></p>");
    message.append("<p>--------</p>");
    message.append("<p>Enviada via DSM<p></div>");

    message.append("\r\n\r\n");

    message.append("--000e0cdfc5ccf3ea1f04c844bb85--\r\n");
    message.append("--000e0cdfc5ccf3ea2104c844bb86\r\n");

    message.append("Content-Type: image/png; name = \":/png/logom2.png\"\r\n");
    message.append("Content-Transfer-Encoding: base64\r\n");
    message.append("Content-ID: <marca_m2smart@m2smart.com.br>\r\n");
    message.append("Content-Disposition: inline; filename = \":/png/logom2.png\"\n\r\n");
    message += strLogo + "\r\n";

    message.append("--000e0cdfc5ccf3ea2104c844bb86--\r\n");
    message.append("--000e0cdfc5ccf3ea2404c844bb87");

    for(int i = 0 ; i < file.count() ; i++)
    {
        if(!file.at(i).contains(UNCHECKEDANEX))
        {
            QFileInfo fileInfo(file.at(i));

            QFile fileMail(file.at(i));

            if (!fileMail.open(QIODevice::ReadOnly))
            {
                qDebug() << "Error ao abrir o arquivo!";
            }

            QString str = fileMail.readAll().toBase64();
            fileMail.close();

            message.append("\r\n");
            message.append(QString("Content-Type: application/octet-stream; name=\"%1\"\r\n").arg(fileInfo.fileName()) );
            message.append(QString("Content-Disposition: attachment; filename=\"%1\"\r\n").arg(fileInfo.fileName()));
            message.append("Content-Transfer-Encoding: base64\n\r\n");
            message += str + "\r\n";
            message.append("--000e0cdfc5ccf3ea2404c844bb87");
        }
    }

    message.append("--");

    message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
    message.replace( QString::fromLatin1( "\r\n.\r\n" ),
                     QString::fromLatin1( "\r\n..\r\n" ));

    this->from = from;
    
    //Add to adress
    
    this->server = server;
*/
}

void M2Smtp::send()
{
/*    state = Init;
    emit status( 1 ); //Connecting
    socket->connectToHost( server, 587);
    if(socket->waitForConnected ( 10000 ))
    {
        qDebug("constructor");
        t = new QTextStream( socket );
        emit status( 2 ); //Connected
    }
*/
}
M2Smtp::~M2Smtp()
{
  //  delete t;
 //   delete socket;
}

/* This signal is emitted whenever QAbstractSocket's state changes. The socketState parameter is the new state. */
void M2Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{

//    qDebug() <<"stateChanged " << socketState;
}

void M2Smtp::errorReceived(QAbstractSocket::SocketError socketError)
{
/*    qDebug() << "error " << socketError;
    this->errorSocket = socketError;
    setM2SmtpMsgError(socket->errorString());
    emit completed(false, socketError);
    status( -1 );
    */
}

void M2Smtp::disconnected()
{
/*    qDebug() <<"disconneted";
    qDebug() << "error "  << socket->errorString();
    if (socket->error() == QAbstractSocket::UnconnectedState)
    {
        qDebug() << "cai";
    }
    */
}

/* This signal is emitted after connectToHost() has been called and a connection has been successfully established. */
void M2Smtp::connected()
{
//    qDebug() << "connected";
}

void M2Smtp::readyRead()
{
    /*

    qDebug() <<"readyRead";
    // SMTP is line-oriented

    QString responseLine;
    do
    {
        responseLine = socket->readLine();
        response += responseLine;
    }
    while ( socket->canReadLine() && responseLine[3] != ' ' );

    responseLine.truncate( 3 );


    if ( state == Init && responseLine[0] == '2' )
    {
        // banner was okay, let's go on
        *t << "HELO there\r\n";
        t->flush();

        state = Auth;
        emit status( 3 ); //Sending
    }
    else if ( state == Auth && responseLine[0] == '2' )
    {
        // HELO was okay, let's go on
        *t << "AUTH PLAIN " << loginpass << "\r\n";
        t->flush();

        state = Mail;
        emit status( 4 ); //Sending
    }
    else if ( state == Mail && responseLine[0] == '2' )
    {
        // HELO response was okay (well, it has to be)

        *t << "MAIL FROM: " << from << "\r\n";
        t->flush();
        state = Rcpt;
        emit status( 5 ); //Sending
    }
    else if ( state == Rcpt && responseLine[0] == '2' )
    {
        QString adress = rcpt[rcptIndex];
        *t << "RCPT TO: " << adress << "\r\n"; //r
        rcptIndex++;
        t->flush();
        emit status( 6 ); //Sending
        if (rcptIndex >= rcpt.size())
            state = Data;
    }
    else if ( state == Data && responseLine[0] == '2' )
    {

        *t << "DATA\r\n";
        t->flush();
        state = Body;
        emit status( 7 ); //Sending
    }
    else if ( state == Body && responseLine[0] == '3' )
    {
        *t << message << "\r\n.\r\n";
        t->flush();
        state = Quit;
    }
    else if ( state == Quit && responseLine == "250" )
    {
        qDebug() << __LINE__ << __FUNCTION__;
        *t << "QUIT\r\n";
        t->flush();
        // here, we just close.
        state = Close;

        QMessageBox::information(0,
                                 QString::fromUtf8("Enviado"),
                                 QString::fromUtf8("Sua mensagem foi enviada com sucesso!"));

        emit completed(true,this->errorSocket);
        emit status( 8 ); //Sending
        socket->disconnectFromHost();
    }
    else if ( state == Close )
    {
        deleteLater();
        return;
    }
    else
    {
        // something broke.
        QMessageBox::warning( 0, tr( "M2 E-mail" ), tr( "Unexpected reply from SMTP server:\n\n" ) + response );
        emit status(-1);
        state = Close;
    }

    response = "";
    */
}

void M2Smtp::getM2SmtpMsgError(const int error, QString &message)
{
/*    if ( QAbstractSocket::HostNotFoundError == error )
    {
        message = this->errorMsgSocket;
        qDebug() << "MESSAGE: " << message;
    }
    */
}

void M2Smtp::setM2SmtpMsgError(QString errorString)
{
   // this->errorMsgSocket = errorString;
}

void M2Smtp::mailSent(QString text)
{
    if(text == "Message sent")
        QMessageBox::information( 0, QString::fromUtf8( "Informação" ), tr( "e-mail Enviado!\n\n" ) );

}
