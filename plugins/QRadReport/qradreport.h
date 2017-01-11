#ifndef QRadReportPlugin_H
#define QRadReportPlugin_H

#include <QtCore>

#include "qradplugininterface.h"
#include "qradplugincontainer.h"
#include "qradcoreplugin.h"

class QRadReportPlugin : public QRadPluginInterface
{
    Q_OBJECT;
    Q_INTERFACES( QRadPluginInterface );
    Q_PLUGIN_METADATA(IID "qradreportplugin" FILE "qradreport.json")

public:
     QRadReportPlugin();
    ~QRadReportPlugin();

    void onLoad(QRadPluginContainer*);

protected:



private slots:
    void Process( const QString& action );
    void setParam(QString, QVariant);
    QVariant getParam(QString);

private:
    QRadPluginContainer *m_container;
};

#endif // QRadReportPlugin_H
