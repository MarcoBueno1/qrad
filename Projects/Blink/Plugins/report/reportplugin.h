#ifndef ReportPLUGIN_H
#define ReportPLUGIN_H

#include <QtCore>

#include "qradplugininterface.h"
#include "qradplugincontainer.h"
#include <QStringList>

class ReportPlugin : public QRadPluginInterface
{
    Q_OBJECT;
    Q_INTERFACES( QRadPluginInterface );
    Q_PLUGIN_METADATA(IID "Report" FILE "report.json")

public:
     ReportPlugin();
    ~ReportPlugin();

    void onLoad(QRadPluginContainer*);


private slots:
    void Process( const QString& action );
    void setParam(QString, QVariant);
    QVariant getParam(QString);

private:
    QRadPluginContainer *m_container;
    QString m_Title;
    QString m_strSQL;
    QStringList m_LeftHead;
    QStringList m_percents;
    QStringList m_aligns;
};

#endif // Report_H
