#ifndef QRADLICUP_H
#define QRADLICUP_H

#include <QSqlQueryModel>
#include <QSqlDatabase>

class QRadLicUp 
{
public:
    QRadLicUp();
    ~QRadLicUp();

    bool SyncLicence();

private:
    void conectlocal();
    void conectremote();

    bool m_isNetworkStore;
    QSqlQueryModel *m_model;
    QSqlQueryModel *m_Remotemodel;
    QSqlDatabase    m_localdb;
    QSqlDatabase    m_db;
    QSqlDatabase    m_networkdb;
    QString m_lastInfo;

};

#endif // QRADLICUP
