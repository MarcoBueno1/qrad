#include "qradlicup.h"
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include "clicense.h"
#include <QSqlRecord>
#include <QSqlField>
//#include "dsmsystem.h"
#include <QSqlQuery>
#include "qradfile.h"
#include "qraddebug.h"
#include "maincompany.h"

void QRadLicUp::conectlocal()
{
    if( !m_localdb.isOpen() || !m_localdb.isValid() )
    {
        m_localdb =QSqlDatabase::addDatabase( QRadFile::GetParamValue("banco/database"), "pqsqllocal");
        m_localdb.setDatabaseName(QRadFile::GetParamValue("banco/databasename"));
        m_localdb.setHostName(QRadFile::GetParamValue("banco/hostname"));
        m_localdb.setUserName(QRadFile::GetParamValue("banco/username"));
        m_localdb.setPassword(QRadFile::GetParamValue("banco/password"));
    }

    if (!m_localdb.open())
    {
        QMessageBox::information(NULL, "Erro.", "Nao foi possivel conectar no banco de dados local");
    }

}

void QRadLicUp::conectremote()
{
    QString strRemoteServer;
    if( !m_db.isOpen() || !m_db.isValid() )
    {
        debug_message("\nENTREI NO IF PARA SETAR DADOS DO SERVIDOR REMOTO (%s)........\n",
                      QRadFile::GetParamValue("licensedb/database").toLatin1().data());

        for( int i=0; i < QSqlDatabase::drivers().count(); i++)
          debug_message("\nDrivers disponivel: %s\n",QSqlDatabase::drivers().value(i).toLatin1().data());


        debug_message("\ndatdabase=%s\n",QRadFile::GetParamValue("licensedb/database").toLatin1().data());

        m_db =QSqlDatabase::addDatabase( QRadFile::GetParamValue("licensedb/database"), "mysqlserverdream");
        m_db.setDatabaseName(/*dsmFile::GetParamValue("licensedb/databasename")*/ "lnxclients");
        strRemoteServer = "mysql.b2lnx.com"; /*dsmFile::GetParamValue("licensedb/hostname")*/
        m_db.setHostName(strRemoteServer);
        if(strRemoteServer == QString("mysql.m2smart.com.br"))
            m_db.setHostName(QString("mysql.b2lnx.com"));

        m_db.setUserName("lnx"/*dsmFile::GetParamValue("licensedb/username")*/ );
        QString ps = "lnxfuckhacker";
        ps.truncate(3);
        ps+= "passwordy";
        ps+= "2";
        ps+= "0";
        ps+= "a";
        ps+= "z";

        ps.replace("az","15");
        ps.replace("y","+");
        m_db.setPassword(ps /*dsmFile::GetParamValue("licensedb/password")*/);
        m_db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=20");

    }

    if (!m_db.open())
    {
//        QMessageBox::information(NULL, "Erro.", "Nao foi possivel conectar no banco de dados remoto.");


//        debug_message("\nErro: %s\n (m_db.setHostName=%s, m_db.password()=%s)\n", m_db.lastError().text().toUtf8().data(),
 //                     m_db.hostName().toUtf8().data(), m_db.password().toUtf8().data());
//        debug_message("\nNAO CONSEGUI CONECTAR NO SERVIDOR REMOTO ........\n");
        debug_message("\n%s........\n" ,m_db.lastError().text().toLatin1().data());

    }

}


QRadLicUp::QRadLicUp()
{

    m_model = new QSqlQueryModel;
    m_Remotemodel = new QSqlQueryModel;
    //m_progress = NULL;
    m_isNetworkStore = false;
}

QRadLicUp::~QRadLicUp()
{

    delete m_model;
    delete m_Remotemodel;
    //if( NULL != m_progress )
//      delete m_progress;
}

bool QRadLicUp::SyncLicence()
{
    Clicense objLicense;

    QString strSQL;
	

    maincompany *company = maincompany::findByid(1);

//    m_model->setQuery("select * from maincompany order by id limit 1");
//    if( m_model->rowCount( ))
//    {
     //    bSync = m_model->record(0).field("sync").value().toBool();
//    }

 //   if( ( QDate::currentDate().day() > 25 ) || !bSync || !iRemaining ) @mbueno: provisoriamente comentado, para verificar o comportamento quando
                                                                     //           o DSM busca informação todos os dias no servidor...
    {

        conectremote();
        if( !m_db.isOpen() )
        {
            debug_message("\nNão consegiu conectar no servidor remoto, abortando........\n");
            return false;
        }


//        m_Remotemodel->setQuery( QString("select cnpj, lastlicense, appversionm2smart from license where cnpj = '%1'").arg(company->getcnpj()));
        if( company)
        {
            QString strIdentification =  objLicense.GetClientIdentification();
            QString strcnpj;
            QString strLastlicense;
            QString strDSMRelease;
            bool blocked;
            bool enabled;
            QString dsmRelease;
            QString dsmNewRelease;

            strcnpj = company->getcnpj();
            strLastlicense = company->getlastLicense();
            strDSMRelease = "1.0";//QRadFile::GetParamValue("version/oldrelease");
            /// update license...


            qDebug() << "release:" << strDSMRelease;
            qDebug() << "local cnpj:" << strcnpj;

            strSQL = "select credential, lastlicense, license, enabled, blocked,enableautoupdate,dsmrelease,dsmnewrelease  from lnx_client where cnpj = '" + strcnpj + "'";

            m_Remotemodel->setQuery(strSQL, m_db);
            qDebug() << "Registros remotos: " << m_Remotemodel->rowCount();
            if( m_Remotemodel->rowCount() )
            {
                debug_message("\nENCONTROU REGISTRO REMOTO........\n");

                qDebug() << "Local: Credential:" <<strIdentification;
                qDebug() << "Remote: Credential:" <<m_Remotemodel->record(0).field(0).value().toString();
                qDebug() << "Local: license:" << strLastlicense;
                qDebug() << "Remote: license:" <<m_Remotemodel->record(0).field(2).value().toString();
                qDebug() << "Update DSM: " << m_Remotemodel->record(0).field(5).value().toBool();
                qDebug() << "New Update DSM: " << m_Remotemodel->record(0).field(7).value().toString();

                enabled = m_Remotemodel->record(0).field(3).value().toBool();
                blocked = m_Remotemodel->record(0).field(4).value().toBool();
                //updateDsm = m_Remotemodel->record(0).field(5).value().toBool();
                dsmRelease = m_Remotemodel->record(0).field(6).value().toString();
                dsmNewRelease = m_Remotemodel->record(0).field(7).value().toString();

                if(strIdentification != m_Remotemodel->record(0).field(0).value().toString())
                {
                    strSQL = "update lnx_client set credential = '" + strIdentification +"' where cnpj='"+
                             strcnpj+"'";

                    qDebug() << "Registros remotos: " << m_Remotemodel->rowCount();

                    m_Remotemodel->setQuery( strSQL, m_db );

                }
                else if( !m_Remotemodel->record(0).field(2).value().toString().isEmpty() )
                {
                    if( objLicense.IsValidlicense( m_Remotemodel->record(0).field(2).value().toString()))
                    {
                        strSQL = "update lnx_client set lastlicense = '" + strLastlicense +"' where cnpj='"+
                                 strcnpj+"'";

                        strLastlicense = m_Remotemodel->record(0).field(2).value().toString();

                        m_Remotemodel->setQuery( strSQL, m_db );

//                        strSQL = "update maincompany set lastlicense = '" + strLastlicense + "'" ;

//                        m_Remotemodel->setQuery( strSQL );
                    }
                }
                strSQL = "update lnx_client set dsmrelease = '" + strDSMRelease +"' where cnpj='"+
                         strcnpj+"'";

                m_Remotemodel->setQuery( strSQL, m_db );

                m_model->setQuery("update maincompany set sync=true");
                //m_model->setQuery("update dsm_configuration set bp=true");

                if( blocked  || !enabled )
                {
                       strSQL = "update maincompany set lastlicense = '' " ;
                       m_Remotemodel->setQuery( strSQL );
                       qDebug() << "IMPORTANTE: SUA LICENÇA FOI BLOQUEADA, POR FAVOR ENTRE EM CONTATO COM A M2SMART ( contato@m2smart.com.br )";
                }

            }
            else // insert new store into remote db.
            {
                debug_message("\nTENTANDO INSERIR REGISTRO REMOTO........\n");


                QSqlQuery *insert  = new QSqlQuery(m_db);

                strSQL = "INSERT INTO lnx_client (name, nickname, address, number, neighborhood, city, state, cep, credential, lastlicense, license, enabled, blocked, phone, email, contato, cnpj, ie, mobile, installdate, dsmrelease ) VALUES ( '%1', '%2', '%3', %4, '%5', '%6', %7, '%8', '%9', '%10', '%11', %12, %13, '%14', '%15', '%16', '%17', '%18', '%19', '%20', '%21' )";

                if( !insert->exec(strSQL.arg(company->getname())
                    .arg(company->getfantasyname())
                    .arg("Av. Grande Otelo")
                    .arg("270")
                    .arg("Parque 10")
                    .arg("Manaus")
                    .arg(13)
                    .arg("69055050")
                    .arg(strIdentification)
                    .arg(company->getlicense())
                    .arg(company->getlastLicense())
                    .arg(true)
                    .arg(false)
                    .arg("1")
                    .arg("1")
                    .arg("1")
                    .arg(company->getcnpj())
                    .arg("1")
                    .arg("1")
                    .arg( QDate::currentDate().toString("yyyy-MM-dd"))
                    .arg( strDSMRelease ) ))
                {
                    debug_message("Lasterror=%s\n", insert->lastError().databaseText().toLatin1().data());
                    debug_message("%s\n",strSQL.toLatin1().data());

                    QMessageBox::warning(NULL, QString::fromUtf8("Atenção"), QString::fromUtf8("Não foi possivel atualizar base remota"));
                }
                else
                {
                    if( !strIdentification.trimmed().isEmpty() )
                        m_model->setQuery("update license set sync=true");

                }
                delete insert;

            }

        }
    }
   return true;
}
