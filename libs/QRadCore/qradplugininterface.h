#ifndef QRadPLUGININTERFACE_H
#define QRadPLUGININTERFACE_H

//#include <QtCore>
//#include <QWidget>
#include <QStringList>
#include "qradsplashscreen.h"

//#include "version.h"

class QRadPluginContainer;
class QRadPluginDescriptor;

class QRadPluginInterface : public QObject
{
    Q_OBJECT
   public:
      QRadPluginInterface();
      virtual ~QRadPluginInterface();

      virtual void onLoad(QRadPluginContainer* container)   = 0;
      void setParent(QWidget *parent){m_parent = parent;}

      void appendAction(QString action);
      void setDescriptor(QRadPluginDescriptor *desc);
      QRadPluginDescriptor* getDescriptor();

      QStringList* actionList();
      void setSplash( QRadSplashScreen *splash) { m_splash = splash;}
      QWidget *getParent() {return m_parent;}

    public slots:
        virtual void Process( const QString& action ) = 0;
        virtual void setParam(QString paramName, QVariant paramValue) = 0;
        virtual QVariant getParam(QString paramName) = 0;

    protected:
        QWidget* m_parent;
        QRadSplashScreen *m_splash;

    private:
        QStringList* m_actionList;
        QRadPluginDescriptor *m_descriptor;
};

Q_DECLARE_INTERFACE( QRadPluginInterface, "QRad.QRadPluginInterface" )

#endif // QRadPLUGININTERFACE_H

