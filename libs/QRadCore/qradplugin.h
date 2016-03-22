#ifndef qrad_H
#define qrad_H

#include <QtCore>

#include "qradplugininterface.h"
#include "qradplugincontainer.h"
//#include "qrad.h"

class QRadPlugin : public QRadPluginInterface
{
    Q_OBJECT;
    Q_INTERFACES( QRadPluginInterface );
    Q_PLUGIN_METADATA(IID "qradplugin" FILE "qrad.json")

public:
//    CalcDarePlugin();
    ~CalcDarePlugin();

    void onLoad(QRadPluginContainer*);

protected:
    void show();

private slots:
    void Process( const QString& action );
    void setParam(QString, QVariant);
    QVariant getParam(QString);

private:
//    CalcDare *m_calcDare;
    QRadPluginContainer *m_container;
};

#endif // qrad_H
