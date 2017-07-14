#include "qraddebug.h"
#include "qradcoreplugin.h"
#include "qradshared.h"
#include "qradlog.h"
#include "qradconfig.h"
//#include "cashiermodel.h"
//#include "configurationmodel.h"

/*************************************************************************************************
                                            QRadPluginMenu
**************************************************************************************************/

QRadPluginMenu::QRadPluginMenu(QString path, QString title, QString action, QString hotKey, QString perm, QString icon)
{
    setPath(path);
    setTitle(title);
    setAction(action);
    setHotKey(hotKey);
    setPermission(perm);
    setIcon(icon);

}

/*************************************************************************************************
                                            QRadPluginDescriptor
**************************************************************************************************/

QRadPluginDescriptor::QRadPluginDescriptor(QString name, QString library, bool enabled)
{
    m_menuList = new QList<QRadPluginMenu*>();

    setName(name);
    setLibrary(library);
    setEnabled(enabled);
}

QRadPluginDescriptor::~QRadPluginDescriptor()
{
    foreach (QRadPluginMenu *menu, *m_menuList)
        delete menu;

    m_menuList->clear();

    delete m_menuList;
}

void QRadPluginDescriptor::appendMenu(QRadPluginMenu *menu)
{
    m_menuList->append(menu);
}

/*************************************************************************************************
                                            QRadPlugin
**************************************************************************************************/

QRadPluginLoader::QRadPluginLoader( QMainWindow* parent, QRadSplashScreen *splash )
{
    m_pluginList = new QList<QRadPluginDescriptor *>();

    m_loaderMap  = new QMap<QString, QPluginLoader*>();

    m_splash = splash;

    m_mainWindow = parent;
}

QRadPluginLoader::~QRadPluginLoader()
{
    foreach (QString key, m_loaderMap->keys())
    {
        QPluginLoader *loader = m_loaderMap->value(key);

        if (NULL != loader)
        {
            debug_message("Unloading %s...\t", key.toUtf8().data());
            loader->unload();
            m_loaderMap->insert(key, NULL);

            debug_message("Done.\n");
        }
    }// foreach

    m_loaderMap->clear();

    delete m_loaderMap;
    delete m_pluginList;
}

bool QRadPluginLoader::Load( const QString& xmlPluginList )
{
    debug_message("[QRadPluginLoader]-->Load(%s)\n", xmlPluginList.toUtf8().data());
    QFile file( xmlPluginList );


    /// Add default report smart plugin ( it create its menus onLoad according database config )
    QRadPluginDescriptor *descrep = new QRadPluginDescriptor(
            QString::fromUtf8("qradreportplugin"),
            QString::fromUtf8("./qradreportplugin"),
            true);
    m_pluginList->append(descrep);
    ///


    if( file.open(QIODevice::ReadOnly) )
    {
        QString content = QString::fromUtf8(file.readAll());


        if (!parse(content))
        {
            debug_message("[QRadPluginLoader]<--Load false L:%d\n",__LINE__);
            return false;
        }
    }
    else
    {
        debug_message("[QRadPluginLoader]::Load() Fail L:%d\n",__LINE__);
        QRad_LOG_ERROR(QString("Failed to read file {%1}").arg(xmlPluginList));
    }


    foreach (QRadPluginDescriptor *desc, *m_pluginList)
    {
        if (!desc->enabled())
        {
            debug_message("%s is disabled.\n", desc->name().toUtf8().data());
            QRad_LOG_ERROR(QString("{%1} is disabled").arg(desc->name()));
            continue;
        } // if

        loadPlugin(desc);
    } // foreach

    debug_message("[QRadPluginLoader]<--Load true L:%d\n",__LINE__);
    return true;
}

bool QRadPluginLoader::loadPlugin(QRadPluginDescriptor *desc)
{
    QString libName;
    QStringList aux;
    QTime time;

    if (desc->library().contains("/"))
    {
        aux = desc->library().split('/');

        libName = LIBRARY(aux.at(aux.count() - 1));

        for (int i=aux.count() - 2; i >= 0; i--)
        {
            libName = aux.at(i) + "/" + libName;
        }
    }
    else
    {
        libName = LIBRARY(desc->library());
    }

    time.start();

    debug_message("Loading New: %s...\t", libName.toUtf8().data()); fflush(stdout);

    QPluginLoader* loader = new QPluginLoader(libName);

    if ( !loader->load() )
    {
        QRad_LOG_ERROR(QString("Failed: %1").arg(loader->errorString()));

        debug_message("Failed: %s\n", loader->errorString().toUtf8().data()); fflush(stdout);
        return false;
    }

    QRadPluginInterface* plugin = qobject_cast<QRadPluginInterface*>(loader->instance());

    if ( NULL == plugin )
    {
        loader->unload();
        delete loader;
        debug_message("Failed: %s\n", loader->errorString().toUtf8().data()); fflush(stdout);
        return false;
    }

    plugin->setDescriptor(desc);

    QRadPluginContainer *container = QRadPluginContainer::getInstance();

    plugin->setParent(dynamic_cast<QWidget*>(m_mainWindow));
    plugin->setSplash(m_splash);
    plugin->onLoad(container);

    m_loaderMap->insert(desc->name(), loader);
    container->insert(desc->name(), plugin);

    debug_message("Done [%dms].\n",time.elapsed()); fflush(stdout);

    foreach (QRadPluginMenu *menu, desc->menuList())
        loadMenu(plugin, menu);

    return true;
}

bool QRadPluginLoader::loadMenu(QRadPluginInterface *plugin, QRadPluginMenu *menu)
{
    if (NULL == m_mainWindow)
        return false;

    QWidget* currentMenu = m_mainWindow->menuBar();
    QStringList menus = menu->path().split('/');

    for ( int i = 0; i < menus.count(); i++ )
    {
        currentMenu = findMenu(currentMenu, menus.at( i ) );
    }

    QMenu* lastMenu = qobject_cast<QMenu*>( currentMenu );

    if ( 0 == lastMenu )
    {
        return false;
    }

    QRadPluginAction* action = new QRadPluginAction( menu->title() );
    if( !menu->icon().isEmpty())
    {
        debug_message("Icone: %s para %s\n", menu->icon().toLatin1().data(), menu->title().toLatin1().data());
        action->setIcon(QIcon(menu->icon()));
    }
    if( !menu->hotKey().isEmpty())
        action->setShortcut(QKeySequence(menu->hotKey()));




#if (DEVELOPMENT_VERSION == 1)
    action->setEnabled(true);
#else
    action->setEnabled(QRadConfig::GetUserProfile(menu->permission()));
#endif

    action->setActionDescriptor(menu->action());
    lastMenu->addAction( action );
    action->connect( action, SIGNAL(triggered(QString)), plugin, SLOT(Process(QString)) );

    return true;
}

QMenu* QRadPluginLoader::findMenu(QWidget* parent, QString menuTitle)
{
    QMenu* menu = 0;

    for ( int i=0; i < parent->children().count(); i++ )
    {
        menu = qobject_cast<QMenu*>( parent->children().at( i ) );

        if ( ( 0 != menu ) && ( menuTitle == menu->title().replace( '&', "" ) ) )
        {
            menuTitle = menu->title();
            break;
        }
        else
        {
            menu = 0;
        }
    }

    // if menu does not exists, it's created

    if ( 0 == menu )
    {
        QMenuBar* bar = qobject_cast<QMenuBar*>(parent);

        if ( 0 != bar )
        {
            return bar->addMenu(menuTitle);
        }

        QMenu* parentMenu = qobject_cast<QMenu*>(parent);

        if (0 != parentMenu)
        {
            return parentMenu->addMenu(menuTitle);
        }
    }

    return menu;
}

bool QRadPluginLoader::parse(const QString &xmlContent)
{

    QString error;
    int eLine, eColumn;


    QDomDocument doc;

    if (!doc.setContent(xmlContent, &error, &eLine, &eColumn))
    {
        debug_message("[QRadPluginLoader] <--Parse false L:%d\n", __LINE__);
        QRad_LOG_ERROR(QString("Could not load xml contente (line=%1, column=%2): %3")
                      .arg(eLine).arg(eColumn).arg(error.toUtf8().data()));

        return false;
    }

    QDomNodeList pluginNodes = doc.elementsByTagName("plugin");

    if (pluginNodes.count() == 0)
    {
        debug_message("[QRadPluginLoader] <--Parse false L:%d\n", __LINE__);
        QRad_LOG_ERROR(QString::fromUtf8("There is no plugin to load!"));
        fflush(stderr);
        return false;
    }

    debug_message("[QRadPluginLoader] Found %d plugins! Loading...\n", pluginNodes.count());

    for (int i = 0; i < pluginNodes.count(); i++)
        parsePluginNode(pluginNodes.at(i));

    return true;
}

bool QRadPluginLoader::parsePluginNode(const QDomNode &node)
{
    if (!node.attributes().contains("name"))
    {
        debug_message("Failed parsing plugin node: there is no \"name\" attribute!\n");
        return false;
    }

    QString name = node.attributes().namedItem("name").nodeValue();

    if (!node.attributes().contains("library"))
    {
        debug_message("Failed parsing plugin node (%s): there is no \"library\" attribute!\n", name.toUtf8().data());
        return false;
    }

    bool enabled = false;
//    bool forceNFCe = true;

    if (!node.attributes().contains("enabled"))
    {
        debug_message("Warning: while parsin plugin (%s). there is no \"enabled\" attribute. Assuming true!\n", name.toUtf8().data());
        enabled = true;
    }
    else
    {
        enabled = (node.attributes().namedItem("enabled").nodeValue() == "true") ? true : false;
    }

    QRadPluginDescriptor *desc = new QRadPluginDescriptor(
            node.attributes().namedItem("name").nodeValue(),
            node.attributes().namedItem("library").nodeValue(),
            enabled);

    for (int i = 0; i < node.childNodes().count(); i++)
    {
        QDomNode menuNode = node.childNodes().at(i);

        if (menuNode.nodeName() != "menu")
            continue;

        if (!(menuNode.attributes().contains("path")        &&
              menuNode.attributes().contains("title")       &&
              menuNode.attributes().contains("action")      &&
              menuNode.attributes().contains("permission")))
            continue;

        desc->appendMenu(new QRadPluginMenu( QString::fromUtf8(menuNode.attributes().namedItem("path").nodeValue().toUtf8().data()),
                                             QString::fromUtf8(menuNode.attributes().namedItem("title").nodeValue().toUtf8().data()),
                                             QString::fromUtf8(menuNode.attributes().namedItem("action").nodeValue().toUtf8().data()),
                                             QString::fromUtf8(menuNode.attributes().namedItem("hotkey").nodeValue().toUtf8().data()),
                                             QString::fromUtf8(menuNode.attributes().namedItem("permission").nodeValue().toUtf8().data()),
                                             menuNode.attributes().contains("icon")?QString::fromUtf8(menuNode.attributes().namedItem("icon").nodeValue().toUtf8().data()):""));
    } // for

    m_pluginList->append(desc);


    return true;
}
