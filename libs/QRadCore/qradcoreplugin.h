#ifndef _QRadPLUGIN_H_
#define _QRadPLUGIN_H_

#include <QtCore>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStringList>
#include <QPluginLoader>
#include <QLibrary>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomNode>
#include <QDomAttr>

#include "qradplugininterface.h"
#include "qradplugincontainer.h"
#include "qradpluginaction.h"
#include "qradsplashscreen.h"

//#include "qradconfig.h"
//#include "qradsecurefile.h"
//#include "ecfchecker.h"

class QRadPluginMenu
{
public:
    QRadPluginMenu(QString path, QString title, QString action, QString hotKey, QString perm, QString icon="");

    void setPath(const QString &path)       { m_path = path; }
    void setTitle(const QString &title)     { m_title = title; }
    void setAction(const QString &action)   { m_action = action; }
    void setHotKey(const QString &hotkey)   { m_hotKey = hotkey; }
    void setPermission(const QString &perm) { m_permission = perm; }
    void setIcon(const QString &icon) { m_icon = icon; }

    QString path()          const { return m_path; }
    QString title()         const { return m_title; }
    QString action()        const { return m_action; }
    QString hotKey()        const { return m_hotKey; }
    QString permission()    const { return m_permission; }
    QString icon()    const { return m_icon; }
private:
    QString m_path;
    QString m_title;
    QString m_action;
    QString m_hotKey;
    QString m_permission;
    QString m_icon;
};

class QRadPluginDescriptor
{
public:
    QRadPluginDescriptor(QString name, QString library, bool enabled);
    ~QRadPluginDescriptor();

    void setName(const QString &name) { m_name = name; }
    void setLibrary(const QString &fileName) { m_library = fileName; }
    void setEnabled(bool value) { m_enabled = value; }

    QString name()      { return m_name;        }
    QString library()   { return m_library;     }
    bool    enabled()   { return m_enabled;     }
    QList<QRadPluginMenu*> menuList() { return *m_menuList; }

    void appendMenu(QRadPluginMenu *menu);
private:
    QString m_name;
    QString m_library;
    bool    m_enabled;

    QList<QRadPluginMenu*> *m_menuList;
};

class QRadPluginLoader
{
public:
    QRadPluginLoader(QMainWindow* parent, QRadSplashScreen *splash=NULL);
    ~QRadPluginLoader();

    bool Load(const QString& xmlPluginList);
    bool loadMenu(QRadPluginInterface *plugin, QRadPluginMenu *menu);
private:
    bool parse(const QString &xmlContent);
    bool parsePluginNode(const QDomNode &node);
    bool loadPlugin(QRadPluginDescriptor *desc);
    QMenu* findMenu(QWidget* parent, QString menuTitle);
private:
    QList<QRadPluginDescriptor *>    *m_pluginList;
    QMap<QString, QPluginLoader*>   *m_loaderMap;
    QMainWindow                     *m_mainWindow;
    QRadSplashScreen                 *m_splash;
};

#endif // _QRadPLUGIN_H_
