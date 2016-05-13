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

void QRadLicUp::conectlocal()
{
    if( !m_localdb.isOpen() || !m_localdb.isValid() )
    {
        m_localdb =QSqlDatabase::addDatabase( dsmFile::GetParamValue("banco/database"), "pqsqllocal");
        m_localdb.setDatabaseName(dsmFile::GetParamValue("banco/databasename"));
        m_localdb.setHostName(dsmFile::GetParamValue("banco/hostname"));
        m_localdb.setUserName(dsmFile::GetParamValue("banco/username"));
        m_localdb.setPassword(dsmFile::GetParamValue("banco/password"));
    }

    if (!m_localdb.open())
    {
        QMessageBox::information(m_splash, "Erro.", "Nao foi possivel conectar no banco de dados local");
    }

}

void QRadLicUp::conectremote()
{
    QString strRemoteServer;
    if( !m_db.isOpen() || !m_db.isValid() )
    {
        debug_message("\nENTREI NO IF PARA SETAR DADOS DO SERVIDOR REMOTO ........\n");


        m_db =QSqlDatabase::addDatabase( dsmFile::GetParamValue("licensedb/database"), "mysqlserverdream");
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
        QMessageBox::information(m_splash, "Erro.", "Nao foi possivel conectar no banco de dados remoto.");
//        debug_message("\nErro: %s\n (m_db.setHostName=%s, m_db.password()=%s)\n", m_db.lastError().text().toUtf8().data(),
 //                     m_db.hostName().toUtf8().data(), m_db.password().toUtf8().data());
    }

}


QRadLicUp::QRadLicUp()
{

    m_model = new QSqlQueryModel;
    m_Remotemodel = new QSqlQueryModel;
    m_progress = NULL;
    m_isNetworkStore = false;
}

QRadLicUp::~QRadLicUp()
{

    delete m_model;
    delete m_Remotemodel;
    if( NULL != m_progress )
      delete m_progress;
}

bool QRadLicUp::SyncLicence()
{
    Clicense objLicense;

    QString strSQL;
	

    m_model->setQuery("select * from license order by id limit 1");
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
            return false;
        }


        m_Remotemodel->setQuery( "select cnpj, lastlicense, appversionm2smart from license");
        if( m_Remotemodel->rowCount())
        {
            QString strIdentification =  objLicense.GetClientIdentification();
            QString strcnpj;
            QString strLastlicense;
            QString strDSMRelease;
            bool blocked;
            bool enabled;
            bool updateDsm;
            QString dsmRelease;
            QString dsmNewRelease;

            strcnpj = m_Remotemodel->record(0).field(0).value().toString();
            strLastlicense = m_Remotemodel->record(0).field(1).value().toString();
            strDSMRelease = dsmFile::GetParamValue("version/oldrelease");
            /// update license...


            qDebug() << "release:" << strDSMRelease;
            qDebug() << "local cnpj:" << strcnpj;

            strSQL = "select credential, lastlicense, license, enabled, blocked,enableautoupdate,dsmrelease,dsmnewrelease  from lnx_client where cnpj = '" + strcnpj + "'";

            m_Remotemodel->setQuery(strSQL, m_db);
            qDebug() << "Registros remotos: " << m_Remotemodel->rowCount();
            if( m_Remotemodel->rowCount() )
            {
                qDebug() << "Local: Credential:" <<strIdentification;
                qDebug() << "Remote: Credential:" <<m_Remotemodel->record(0).field(0).value().toString();
                qDebug() << "Local: license:" << strLastlicense;
                qDebug() << "Remote: license:" <<m_Remotemodel->record(0).field(2).value().toString();
                qDebug() << "Update DSM: " << m_Remotemodel->record(0).field(5).value().toBool();
                qDebug() << "New Update DSM: " << m_Remotemodel->record(0).field(7).value().toString();

                enabled = m_Remotemodel->record(0).field(3).value().toBool();
                blocked = m_Remotemodel->record(0).field(4).value().toBool();
                updateDsm = m_Remotemodel->record(0).field(5).value().toBool();
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

//                        strSQL = "update dsm_uselicnese set lastlicense = '" + strLastlicense + "'" ;

//                        m_Remotemodel->setQuery( strSQL );
                    }
                }
                strSQL = "update lnx_client set dsmrelease = '" + strDSMRelease +"' where cnpj='"+
                         strcnpj+"'";

                m_Remotemodel->setQuery( strSQL, m_db );

                m_model->setQuery("update license set sync=true");
                m_model->setQuery("update dsm_configuration set bp=true");

                if( blocked  || !enabled )
                {
                       strSQL = "update license set lastlicense = '' " ;
                       m_Remotemodel->setQuery( strSQL );
                       qDebug() << "IMPORTANTE: SUA LICENÇA FOI BLOQUEADA, POR FAVOR ENTRE EM CONTATO COM A M2SMART ( contato@m2smart.com.br )";
                }
                else
                {
                    if(updateDsm & dsmRelease != dsmNewRelease){

                        dsmFile::SetParamValue("version/newrelease",dsmNewRelease);

                        DSMPluginInterface *dsmUpdatePlugin;

                        dsmUpdatePlugin = DSMPluginContainer::getInstance()->plugin("dsmupdate");

                        if( NULL == dsmUpdatePlugin )
                        {
                            qDebug() << "Not found dsmupdate...: " << __LINE__;
                            return false;
                        }

                        dsmUpdatePlugin->setParam("updateDSM", updateDsm);
                        dsmUpdatePlugin->Process(ACTION_SHOW_DSMUPDATE);
                    }
                }

            }
            else // insert new store into remote db.
            {

                QSqlQuery *insert  = new QSqlQuery(m_db);

                strSQL = "INSERT INTO lnx_client (name, nickname, address, number, neighborhood, city, state, cep, credential, lastlicense, license, enabled, blocked, phone, email, contato, cnpj, ie, mobile, installdate, dsmrelease ) VALUES ( '%1', '%2', '%3', %4, '%5', '%6', %7, '%8', '%9', '%10', '%11', %12, %13, '%14', '%15', '%16', '%17', '%18', '%19', '%20', '%21' )";

                if( !insert->exec(strSQL.arg(m_model->record(0).field("companyname").value().toString())
                    .arg(m_model->record(0).field("fantasyname").value().toString())
                    .arg(m_model->record(0).field("address").value().toString())
                    .arg(m_model->record(0).field("nro").value().toInt())
                    .arg(m_model->record(0).field("neighborhood").value().toString())
                    .arg(m_model->record(0).field("city").value().toString())
                    .arg(m_model->record(0).field("uf").value().toInt())
                    .arg(m_model->record(0).field("cep").value().toString())
                    .arg(strIdentification)
                    .arg(m_model->record(0).field("lastlicense").value().toString())
                    .arg(m_model->record(0).field("lastlicense").value().toString())
                    .arg(true)
                    .arg(false)
                    .arg(m_model->record(0).field("phone").value().toString())
                    .arg(m_model->record(0).field("email").value().toString())
                    .arg("")
                    .arg(m_model->record(0).field("cnpj").value().toString())
                    .arg(m_model->record(0).field("ie").value().toString())
                    .arg(m_model->record(0).field("fax").value().toString())
                    .arg( QDate::currentDate().toString("yyyy-MM-dd"))
                    .arg( strDSMRelease ) ))
                {
                    qDebug() << insert->lastQuery();
                    QMessageBox::warning(m_splash, QString::fromUtf8("Atenção"), QString::fromUtf8("Não foi possivel atualizar base remota"));
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