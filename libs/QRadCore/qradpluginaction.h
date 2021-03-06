#ifndef QRadPLUGINACTION_H
#define QRadPLUGINACTION_H

#include <QString>
#include <QAction>

class QRadPluginAction : public QAction
{
    Q_OBJECT;
public:
    QRadPluginAction( const QString& text, QObject* parent = 0 );

    void setActionDescriptor( const QString& action );
    void setPermission( const QString &permission);
    void setEnabledEx();
private:
    QString m_action;
    QString m_permission;

private slots:
    void trigger();

signals:
    void triggered( QString action );
};

#endif // QRadPLUGINACTION_H
