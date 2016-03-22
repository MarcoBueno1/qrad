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

QRadPluginMenu::QRadPluginMenu(QString path, QString title, QString action, QString hotKey, QString perm)
{
    setPath(path);
    setTitle(title);
    setAction(action);
    setHotKey(hotKey);
    setPermission(perm);
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

    if (!file.open(QIODevice::ReadOnly))
    {
        debug_message("[QRadPluginLoader]::Load() Fail L:%d\n",__LINE__);
        QRad_LOG_ERROR(QString("Failed to read file {%1}").arg(xmlPluginList));
        return false;
    }

    QString content = QString::fromUtf8(file.readAll());


    if (!parse(content))
    {
        debug_message("[QRadPluginLoader]<--Load false L:%d\n",__LINE__);
        return false;
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



#if (DEVELOPMENT_VERSION == 1)
    action->setEnabled(true);
#else
    action->setEnabled(QRadConfig::GetUserProfile(menu->permission()));
#endif

/*    if (!menu->hotKey().isEmpty())
    {
        if ( (menu->action() == ACTION_SHOW_PAF_CASHIER) ||
             (menu->action() == ACTION_MNG_ACCOUNT_PAF) ||
             (menu->action() == ACTION_MNG_AGREEMENT_PAF) ||
             (menu->action() == ACTION_SHOW_CASHIER) ||
             (menu->action() == ACTION_SHOW_PAF_PAYMENTWAYREG) ||
             (menu->action() == ACTION_SHOW_PAF_PAYMENTWAYAUXREG) )
        {
            bool isEnabled = true;

            if (!ECFChecker::load())
            {
                isEnabled = false;
            }
            isEnabled &= ECFChecker::enabled();

            if (isEnabled)
            {
                action->setShortcut(QKeySequence(menu->hotKey()));
            }
        }
        else
        {
            action->setShortcut(QKeySequence(menu->hotKey()));
        }
    }
*/

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

/*
    if(( name == "paf-cashiernfce")  && !enabled)
    {
        // check if current machine already has hd serial into QRad_cashier
        CashierModel *c = CashierModel::findByHDSerial(M2phi::GetHDSerial());
        if(c)
        {
            enabled = true;
            forceNFCe = true;
            delete c;
            debug_message("enable=true for 'paf-cashiernfce' plugin ( checked logic..)!\n");
        }
        else
        {
            CashierModelList *l = CashierModel::findAll();
            if( !l )
            {
                if( QMessageBox::Yes == QMessageBox::question(m_mainWindow,QString::fromUtf8("Atenção"),
                                      QString::fromUtf8("Esta máquina será utilizada como caixa?"),
                                      QMessageBox::Yes | QMessageBox::No ))
                {

                    if( QRadConfig::UserCanCancelSale(true))
                    {
                        enabled = true;
                        forceNFCe = true;
                        delete c;
                        debug_message("enable=true for 'paf-cashiernfce' plugin ( by user..)!\n");
                    }
                    else
                    {
                        QMessageBox::information(m_mainWindow,QString::fromUtf8("Oops"),
                                                              QString::fromUtf8("Você não tem permissão para habilitar a funcionalidade de caixa nesta máquina!"));
                    }

                }

            }


        }
    }

    //for nfe and nfce assume database configuration
    else if(( name == "nfegenerator20") &&
            ConfigurationModel::getInstanceNoSelect()->getUseNFE() )
    {
        enabled = true;
        debug_message("enable=true for 'nfegenrator20' plugin ( checked via Database..)!\n");
    }
    else if(( name == "paf-cashier") &&
            forceNFCe )
    {
        enabled = false;
        debug_message("enable=false for 'paf-cashier' plugin ( paf-cashiernfce is checked via Database..)!\n");
    }
    else if(( name == "paf-cashiernc") &&
            forceNFCe )
    {
        enabled = false;
        debug_message("enable=false for 'paf-cashiernc' plugin ( paf-cashiernfce is checked via Database..)!\n");
    }
*/
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

        desc->appendMenu(new QRadPluginMenu(QString::fromUtf8(menuNode.attributes().namedItem("path").nodeValue().toUtf8().data()),
                                           QString::fromUtf8(menuNode.attributes().namedItem("title").nodeValue().toUtf8().data()),
                                           QString::fromUtf8(menuNode.attributes().namedItem("action").nodeValue().toUtf8().data()),
                                           QString::fromUtf8(menuNode.attributes().namedItem("hotkey").nodeValue().toUtf8().data()),
                                           QString::fromUtf8(menuNode.attributes().namedItem("permission").nodeValue().toUtf8().data())));
    } // for

    m_pluginList->append(desc);

    return true;
}
