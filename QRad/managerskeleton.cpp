#include "managerskeleton.h"
#include <QString>
#include <QFile>
#include "field.h"

#define MANAGER_SKELETON_UI "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"\
"<ui version=\"4.0\">\n"\
" <class>Manager%1</class>\n"\
" <widget class=\"QDialog\" name=\"Manager%1\">\n"\
"  <property name=\"geometry\">\n"\
"   <rect>\n"\
"    <x>0</x>\n"\
"    <y>0</y>\n"\
"    <width>800</width>\n"\
"    <height>600</height>\n"\
"   </rect>\n"\
"  </property>\n"\
"  <property name=\"windowTitle\">\n"\
"   <string>%2</string>\n"\
"  </property>\n"\
"  <widget class=\"QTabWidget\" name=\"tabWidget\">\n"\
"   <property name=\"geometry\">\n"\
"    <rect>\n"\
"     <x>0</x>\n"\
"     <y>0</y>\n"\
"     <width>800</width>\n"\
"     <height>516</height>\n"\
"    </rect>\n"\
"   </property>\n"\
"   <property name=\"font\">\n"\
"    <font>\n"\
"     <family>Verdana</family>\n"\
"     <pointsize>8</pointsize>\n"\
"    </font>\n"\
"   </property>\n"\
"   <property name=\"focusPolicy\">\n"\
"    <enum>Qt::NoFocus</enum>\n"\
"   </property>\n"\
"   <property name=\"currentIndex\">\n"\
"    <number>0</number>\n"\
"   </property>\n"\
"   <widget class=\"QWidget\" name=\"tab\">\n"\
"    <attribute name=\"title\">\n"\
"     <string>%2</string>\n"\
"    </attribute>\n"\
"    <layout class=\"QGridLayout\" name=\"gridLayout_2\">\n"\
"     <property name=\"leftMargin\">\n"\
"      <number>0</number>\n"\
"     </property>\n"\
"     <property name=\"topMargin\">\n"\
"      <number>0</number>\n"\
"     </property>\n"\
"     <property name=\"rightMargin\">\n"\
"      <number>0</number>\n"\
"     </property>\n"\
"     <property name=\"bottomMargin\">\n"\
"      <number>0</number>\n"\
"     </property>\n"\
"     <property name=\"spacing\">\n"\
"      <number>0</number>\n"\
"     </property>\n"\
"     <item row=\"1\" column=\"0\">\n"\
"      <widget class=\"QGroupBox\" name=\"groupBoxItens\">\n"\
"       <property name=\"font\">\n"\
"        <font>\n"\
"         <family>Verdana</family>\n"\
"         <pointsize>9</pointsize>\n"\
"         <weight>50</weight>\n"\
"         <bold>false</bold>\n"\
"        </font>\n"\
"       </property>\n"\
"       <property name=\"title\">\n"\
"        <string>%2s</string>\n"\
"       </property>\n"\
"       <property name=\"alignment\">\n"\
"        <set>Qt::AlignCenter</set>\n"\
"       </property>\n"\
"       <layout class=\"QHBoxLayout\" name=\"horizontalLayout_4\">\n"\
"        <item>\n"\
"         <widget class=\"QRadTableViewSearch\" name=\"tableViewSearch\">\n"\
"          <property name=\"font\">\n"\
"           <font>\n"\
"            <family>Verdana</family>\n"\
"            <pointsize>8</pointsize>\n"\
"           </font>\n"\
"          </property>\n"\
"          <property name=\"focusPolicy\">\n"\
"           <enum>Qt::NoFocus</enum>\n"\
"          </property>\n"\
"          <property name=\"styleSheet\">\n"\
"           <string notr=\"true\">selection-background-color: rgba(102, 176, 239, 75%); selection-color: rgb(255, 255, 255);</string>\n"\
"          </property>\n"\
"          <property name=\"verticalScrollBarPolicy\">\n"\
"           <enum>Qt::ScrollBarAlwaysOn</enum>\n"\
"          </property>\n"\
"          <property name=\"selectionBehavior\">\n"\
"           <enum>QAbstractItemView::SelectRows</enum>\n"\
"          </property>\n"\
"          <property name=\"sortingEnabled\">\n"\
"           <bool>true</bool>\n"\
"          </property>\n"\
"          <attribute name=\"horizontalHeaderMinimumSectionSize\">\n"\
"           <number>20</number>\n"\
"          </attribute>\n"\
"          <attribute name=\"verticalHeaderVisible\">\n"\
"           <bool>false</bool>\n"\
"          </attribute>\n"\
"          <attribute name=\"verticalHeaderMinimumSectionSize\">\n"\
"           <number>18</number>\n"\
"          </attribute>\n"\
"         </widget>\n"\
"        </item>\n"\
"       </layout>\n"\
"      </widget>\n"\
"     </item>\n"\
"     <item row=\"0\" column=\"0\">\n"\
"      <widget class=\"QGroupBox\" name=\"groupBox_2\">\n"\
"       <property name=\"minimumSize\">\n"\
"        <size>\n"\
"         <width>0</width>\n"\
"         <height>60</height>\n"\
"        </size>\n"\
"       </property>\n"\
"       <property name=\"maximumSize\">\n"\
"        <size>\n"\
"         <width>16777215</width>\n"\
"         <height>60</height>\n"\
"        </size>\n"\
"       </property>\n"\
"       <property name=\"font\">\n"\
"        <font>\n"\
"         <family>Verdana</family>\n"\
"         <pointsize>9</pointsize>\n"\
"         <weight>75</weight>\n"\
"         <bold>true</bold>\n"\
"        </font>\n"\
"       </property>\n"\
"       <property name=\"title\">\n"\
"        <string/>\n"\
"       </property>\n"\
"       <property name=\"alignment\">\n"\
"        <set>Qt::AlignHCenter|Qt::AlignTop</set>\n"\
"       </property>\n"\
"       <layout class=\"QHBoxLayout\" name=\"horizontalLayout\">\n"\
"        <property name=\"spacing\">\n"\
"         <number>6</number>\n"\
"        </property>\n"\
"        <property name=\"leftMargin\">\n"\
"         <number>9</number>\n"\
"        </property>\n"\
"        <property name=\"topMargin\">\n"\
"         <number>0</number>\n"\
"        </property>\n"\
"        <property name=\"rightMargin\">\n"\
"         <number>9</number>\n"\
"        </property>\n"\
"        <property name=\"bottomMargin\">\n"\
"         <number>9</number>\n"\
"        </property>\n"\
"        <item>\n"\
"         <widget class=\"QRadLineEditFakeEnter\" name=\"lineEditSearch\">\n"\
"          <property name=\"minimumSize\">\n"\
"           <size>\n"\
"            <width>0</width>\n"\
"            <height>34</height>\n"\
"           </size>\n"\
"          </property>\n"\
"          <property name=\"maximumSize\">\n"\
"           <size>\n"\
"            <width>16777215</width>\n"\
"            <height>34</height>\n"\
"           </size>\n"\
"          </property>\n"\
"          <property name=\"font\">\n"\
"           <font>\n"\
"            <family>Verdana</family>\n"\
"            <pointsize>18</pointsize>\n"\
"           </font>\n"\
"          </property>\n"\
"         </widget>\n"\
"        </item>\n"\
"       </layout>\n"\
"      </widget>\n"\
"     </item>\n"\
"    </layout>\n"\
"   </widget>\n"\
"  </widget>\n"\
"    <widget class=\"QPushButton\" name=\"PshBtnEditar\">\n"\
"     <property name=\"geometry\">\n"\
"      <rect>\n"\
"       <x>620</x>\n"\
"       <y>560</y>\n"\
"       <width>85</width>\n"\
"       <height>27</height>\n"\
"      </rect>\n"\
"     </property>\n"\
"     <property name=\"text\">\n"\
"      <string>&amp;Editar</string>\n"\
"     </property>\n"\
"    </widget>\n"\
"    <widget class=\"QPushButton\" name=\"PshBtnNovo\">\n"\
"     <property name=\"geometry\">\n"\
"      <rect>\n"\
"       <x>533</x>\n"\
"       <y>560</y>\n"\
"       <width>85</width>\n"\
"       <height>27</height>\n"\
"      </rect>\n"\
"     </property>\n"\
"     <property name=\"text\">\n"\
"      <string>&amp;Novo</string>\n"\
"     </property>\n"\
"    </widget>\n"\
"    <widget class=\"QPushButton\" name=\"PshBtnSair\">\n"\
"     <property name=\"geometry\">\n"\
"      <rect>\n"\
"       <x>707</x>\n"\
"       <y>560</y>\n"\
"       <width>85</width>\n"\
"       <height>27</height>\n"\
"      </rect>\n"\
"     </property>\n"\
"     <property name=\"text\">\n"\
"      <string>&amp;Sair</string>\n"\
"     </property>\n"\
"    </widget>\n"\
"   </widget>\n"\
" <customwidgets>\n"\
"  <customwidget>\n"\
"   <class>QRadLineEditFakeEnter</class>\n"\
"   <extends>QLineEdit</extends>\n"\
"   <header location=\"global\">qradlineeditfakeenter.h</header>\n"\
"  </customwidget>\n"\
"  <customwidget>\n"\
"   <class>QRadTableViewSearch</class>\n"\
"   <extends>QTableView</extends>\n"\
"   <header>qradtableviewsearch.h</header>\n"\
"  </customwidget>\n"\
" </customwidgets>\n"\
" <resources/>\n"\
" <connections/>\n"\
"</ui>\n"




#define CPP_SKEL "#include \"manager%1.h\"\n"\
"#include \"ui_manager%1.h\"\n"\
"#include \"edit%1.h\"\n"\
"#include \"column2delegate.h\"\n"\
"#include <QMessageBox>\n"\
"#include <QDebug>\n"\
"\n"\
"#define BN_DEFAULT_COLUMN_SEARCH %3\n"\
"#define SQL_ITEMS \"select %2,id from %1 order by %6\" \n"\
"\n"\
"Manager%1::Manager%1(QWidget *parent) :\n"\
"    QDialog(parent),\n"\
"    ui(new Ui::Manager%1)\n"\
"{\n"\
"    ui->setupUi(this);\n"\
"\n"\
"    m_keyinterval = NULL;\n"\
"    m_Model = new QSqlQueryModel;\n"\
"\n"\
"    ui->tableViewSearch->setStyleSheet(\"QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};\");\n"\
"\n"\
"    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));\n"\
"    connect(ui->tableViewSearch, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));\n"\
"    connect(ui->tableViewSearch, SIGNAL(notFound()),this,SLOT(notFound()));\n"\
"    connect(ui->tableViewSearch,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));\n"\
"    connect(ui->tableViewSearch,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));\n"\
"\n"\
"    connect(ui->PshBtnEditar, SIGNAL(clicked()), this, SLOT(doEditar()));\n"\
"    connect(ui->PshBtnNovo, SIGNAL(clicked()), this, SLOT(doNovo()));\n"\
"    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));\n"\
"\n"\
"    DoRefresh();\n"\
"}\n"\
"\n"\
"Manager%1::~Manager%1()\n"\
"{\n"\
"     if( m_keyinterval )\n"\
"     {\n"\
"       m_keyinterval->stop();\n"\
"       delete m_keyinterval;\n"\
"     }\n"\
"    delete m_Model;\n"\
"\n"\
"    delete ui;\n"\
"}\n"\
"\n"\
"void Manager%1::KeyPressTimeout()\n"\
"{\n"\
"//    if( ui->lineEditSearch->text().trimmed().isEmpty() )\n"\
"//        ui->tableViewSearch->selectRow(0);\n"\
"//    else\n"\
"        ui->tableViewSearch->Search(ui->lineEditSearch->text());\n"\
"}\n"\
"\n"\
"void Manager%1::StartTimer( QString )\n"\
"{\n"\
"    if( ui->lineEditSearch->text().trimmed().length() == 1 )\n"\
"        ui->tableViewSearch->selectRow(0);\n"\
"\n"\
"    if( m_keyinterval )\n"\
"    {\n"\
"        m_keyinterval->stop();\n"\
"        delete m_keyinterval;\n"\
"    }\n"\
"    m_keyinterval = new QTimer;\n"\
"    connect(m_keyinterval, SIGNAL(timeout()), this, SLOT(KeyPressTimeout()));\n"\
"    m_keyinterval->setSingleShot(true);\n"\
"    m_keyinterval->setInterval(200);\n"\
"    m_keyinterval->start();\n"\
"}\n"\
"void Manager%1::Found(QModelIndex)\n"\
"{\n"\
"    ui->tableViewSearch->SetNoEmptySearch( true );\n"\
"\n"\
"    ShowCurrentInformations();\n"\
"    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);\n"\
"\n"\
"    ui->tableViewSearch->SetNoEmptySearch( false);\n"\
"}\n"\
"void Manager%1::notFound()\n"\
"{\n"\
"   ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);\n"\
"  // ui->tableViewSearch->selectRow(0);\n"\
"}\n"\
"void Manager%1::TableClicked(QModelIndex currentIndex)\n"\
"{\n"\
"    qDebug() << \"TableClicked\";\n"\
"    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);\n"\
"    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());\n"\
"    ui->lineEditSearch->selectAll();\n"\
"\n"\
"    ShowCurrentInformations();\n"\
"}\n"\
"void Manager%1::CurrentChanged(QModelIndex currentIndex)\n"\
"{\n"\
"    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);\n"\
"    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());\n"\
"    ui->lineEditSearch->selectAll();\n"\
"\n"\
"    ShowCurrentInformations( );\n"\
"}\n"\
"\n"\
"void Manager%1::ShowCurrentInformations( void )\n"\
"{\n"\
"    QString strTemp;\n"\
"\n"\
"    if( m_Model->rowCount() )\n"\
"    {\n"\
"         strTemp.sprintf(\" Itens ( %d de %d ) \",\n"\
"                         ui->tableViewSearch->currentIndex().row() + 1,\n"\
"                         m_Model->rowCount() );\n"\
"         ui->groupBoxItens->setTitle(strTemp);\n"\
"    }\n"\
"}\n"\
"void Manager%1::LoadTableView()\n"\
"{\n"\
"    QApplication::processEvents();\n"\
"\n"\
"    m_Model->setQuery(SQL_ITEMS);\n"\
"\n"\
"    QApplication::processEvents();\n"\
"    ui->tableViewSearch->setModel( m_Model);\n"\
"    ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);\n"\
"\n"\
"    QApplication::processEvents();\n"\
"}\n"\
"\n"\
"void Manager%1::DoRefresh()\n"\
"{\n"\
"    ui->tableViewSearch->SetNoEmptySearch( true );\n"\
"\n"\
"    QApplication::processEvents();\n"\
"\n"\
"    LoadTableView();\n"\
"\n"\
"    ui->lineEditSearch->setFocus();\n"\
"    ConfigureTable();\n"\
"\n"\
"    QModelIndex index;\n"\
"    index = ui->tableViewSearch->currentIndex();\n"\
"\n"\
"    ui->lineEditSearch->setText(index.sibling(index.row(), BN_DEFAULT_COLUMN_SEARCH).data().toString());\n"\
"\n"\
"    ui->tableViewSearch->SetNoEmptySearch( false );\n"\
"\n"\
"    refreshTable();\n"\
"}\n"\
"\n"\
"void Manager%1::refreshTable()\n"\
"{\n"\
"    if(ui->lineEditSearch->text() == \"\")\n"\
"    {\n"\
"        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);\n"\
"        ui->tableViewSearch->Search(\"\");\n"\
"        ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);\n"\
"    }\n"\
"}\n"\
"\n"\
"void Manager%1::ConfigureTable()\n"\
"{\n"\
"  %4\n"\
"\n"\
"   // m_Model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8(\"Conuna1\"));\n"\
"    //m_Model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8(\"Conuna2\"));\n"\
"   // m_Model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8(\"Conuna3\"));\n"\
"   // m_Model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8(\"Conuna3\"));\n"\
"  //  m_Model->setHeaderData(5, Qt::Horizontal, QString::fromUtf8(\"Conuna4\"));\n"\
"\n"\
"   // ui->tableViewSearch->setColumnWidth(0, 0.06 * ui->tableViewSearch->width());\n"\
"    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf(\"id\"));\n"\
"%5\n"\
"\n"\
"}\n"\
"\n"\
"void Manager%1::keyPressEvent(QKeyEvent *event)\n"\
"{\n"\
"    qDebug() << \"KeyPressEvent\";\n"\
"  //  if(event->key() != Qt::Key_Escape)\n"\
"    switch(event->key())\n"\
"    {\n"\
"      case Qt::Key_PageUp:\n"\
"      case Qt::Key_Up:\n"\
"      case Qt::Key_PageDown:\n"\
"      case Qt::Key_Down:\n"\
"      case  Qt::Key_F12:\n"\
"      case Qt::Key_Delete:\n"\
"                           ui->tableViewSearch->keyPressEvent(event);\n"\
"                           break;\n"\
"      case Qt::Key_Escape:\n"\
"  //                         ui->tableViewSearch->keyPressEvent(event);\n"\
"                           doSair();\n"\
"                           break;\n"\
"      default:\n"\
"        break;\n"\
"    }\n"\
"\n"\
"\n"\
"}\n"\
"void Manager%1::MatchNewest(%1 *newest )\n"\
"{\n"\
"    DoRefresh();\n"\
"    for( int j = 0; j < newest->attributes().count(); j++ )\n"\
"    {\n"\
"        if( ui->tableViewSearch->model()->headerData(BN_DEFAULT_COLUMN_SEARCH,Qt::Horizontal).toString() == newest->attributes().at(j)->fieldName())\n"\
"        {\n"\
"            ui->lineEditSearch->setText(newest->attributes().at(j)->value().toString());\n"\
"            ui->tableViewSearch->Search(newest->attributes().at(j)->value().toString());\n"\
"        }\n"\
"    }\n"\
"}\n"\
"void Manager%1::doEditar()\n"\
"{\n"\
"    Edit%1 *edt = new Edit%1;\n"\
"\n"\
"    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();\n"\
"\n"\
"    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf(\"id\")).data().toInt();\n"\
"\n"\
"    %1 *sa = %1::findByid(nId);\n"\
"    edt->SetModel(sa);\n"\
"    if( edt->exec() == QDialog::Accepted )\n"\
"    {\n"\
"        MatchNewest(edt->GetSaved());\n"\
"    }\n"\
"    delete edt;\n"\
"\n"\
"}\n"\
"\n"\
"void Manager%1::doNovo()\n"\
"{\n"\
"    Edit%1 *edt = new Edit%1;\n"\
"\n"\
"    if( edt->exec() == QDialog::Accepted )\n"\
"    {\n"\
"        MatchNewest(edt->GetSaved());\n"\
"    }\n"\
"    delete edt;\n"\
"}\n"\
"\n"\
"void Manager%1::doSair()\n"\
"{\n"\
"    if( QMessageBox::Yes ==  QMessageBox::question(this, \"Sair?\",\"Deseja sair desta pesquisa?\",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))\n"\
"           reject();\n"\
"}\n"


/* %4
"    ui->tableViewSearch->addSearchColumn(0);\n"\
"    ui->tableViewSearch->addSearchColumn(1);\n"\
"    ui->tableViewSearch->addSearchColumn(2);\n"\
"    ui->tableViewSearch->addSearchColumn(3);\n"\
"    ui->tableViewSearch->addSearchColumn(4);\n"\
"    ui->tableViewSearch->addSearchColumn(5);\n"\
*/


#define H_SKEL "#ifndef MANAGER%1_H\n"\
"#define MANAGER%1_H\n"\
"\n"\
"#include <QTimer>\n"\
"#include <QDialog>\n"\
"#include <QKeyEvent>\n"\
"#include <QSqlQueryModel>\n"\
"#include \"edit%1.h\"\n"\
"\n"\
"namespace Ui {\n"\
"class Manager%1;\n"\
"}\n"\
"\n"\
"class Manager%1 : public QDialog\n"\
"{\n"\
"    Q_OBJECT\n"\
"\n"\
"public:\n"\
"    explicit Manager%1(QWidget *parent = 0);\n"\
"    ~Manager%1();\n"\
"\n"\
"public slots:\n"\
"    void notFound();\n"\
"    void Found( QModelIndex );\n"\
"    void StartTimer( QString );\n"\
"    void TableClicked( QModelIndex );\n"\
"    void CurrentChanged( QModelIndex );\n"\
"    void KeyPressTimeout();\n"\
"    void doEditar();\n"\
"    void doSair();\n"\
"    void doNovo();\n"\
"\n"\
"private:\n"\
"    Ui::Manager%1 *ui;\n"\
"\n"\
"    QTimer *m_keyinterval;\n"\
"    QSqlQueryModel  *m_Model;\n"\
"\n"\
"    void ShowCurrentInformations( void );\n"\
"    void LoadTableView();\n"\
"    void DoRefresh();\n"\
"    void refreshTable();\n"\
"    void ConfigureTable();\n"\
"    void keyPressEvent(QKeyEvent *event);\n"\
"    void MatchNewest(%1 *newest );\n"\
"\n"\
"};\n"\
"\n"\
"#endif // MANAGER%1_H\n"




ManagerSkeleton::ManagerSkeleton()
{

}

ManagerSkeleton::~ManagerSkeleton()
{

}

void ManagerSkeleton::Build(skeleton *skel)
{
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
	
    BuildUI( skel);
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
    BuildH( skel);
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
    BuildCPP( skel);
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
}

void ManagerSkeleton::BuildH(skeleton * skel)
{
    QList<CTable *>*pTables = skel->GetTables();

    for( int i = 0; pTables && (i < pTables->count()); i++   )
    {
        QString Name = pTables->at(i)->getName();

        QFile *file = new QFile(PathMaker::Path()+"manager" + Name + ".h");

        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "Impossivel abrir:"<<"manager" + Name + ".h"<<" para escrita" ;
            delete file;
            return;
        }

        QString strHead = QString(H_SKEL).arg(Name);

        file->write(strHead.toUtf8());

        file->flush();
        file->close();
    }

}

QString ManagerSkeleton::getDelegateFor(int nCol, CType *tp)
{
    if(tp->getType() == "QString")
    {
        return QString("     ui->tableViewSearch->setItemDelegateForColumn(%1, new ColumnCenter);\n").arg(nCol);
    }
    if( tp->getType() == "int")
    {
        return QString("     ui->tableViewSearch->setItemDelegateForColumn(%1, new ColumnCenter);\n").arg(nCol);
    }
    if( tp->getType() == "bool")
    {
        return QString("     ui->tableViewSearch->setItemDelegateForColumn(%1, new ColumnBool);\n").arg(nCol);
    }
    if( tp->getType() == "QDate")
    {
        return QString("     ui->tableViewSearch->setItemDelegateForColumn(%1, new ColumnDate);\n").arg(nCol);
    }
    if( tp->getType() == "QTime")
    {
        return QString("     ui->tableViewSearch->setItemDelegateForColumn(%1, new ColumnTime);\n").arg(nCol);
    }
    return QString("     ui->tableViewSearch->setItemDelegateForColumn(%1, new ColumnCenter);\n").arg(nCol);
}

void ManagerSkeleton::BuildCPP(skeleton * skel)
{
    QList<CTable *>*pTables = skel->GetTables();
    for( int i = 0; pTables && (i < pTables->count()); i++   )
    {
        QString Name = pTables->at(i)->getName();

        QFile *file = new QFile(PathMaker::Path()+"manager" + Name + ".cpp");

        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "Impossivel abrir:"<<"manager" + Name + ".cpp"<<" para escrita" ;
            delete file;
            return;
        }

        int nCount = 0;
        bool bHasSearch = false;
        int nMasterSearch = -1;
        QString strSelect;
        QString strDelegates;
        QString strMasterSearch;
        QString strAddSearchColumn;

        QList<CField*> *fields = pTables->at(i)->getFields();

        for( int j = 0; fields && (j < fields->count()); j++ )
        {
            if( fields->at(j)->getType()->getTpSearch() == tpSearchMaster )
            {
                nMasterSearch = nCount;
                strMasterSearch.sprintf("%d", nMasterSearch);
                if( !strSelect.isEmpty() )
                    strSelect += ", ";
                strSelect += fields->at(j)->getField();
                nCount++;
                strDelegates += getDelegateFor(nMasterSearch,fields->at(j)->getType());
            }
            else if(fields->at(j)->getType()->getTpSearch() == tpSearch )
            {
                bHasSearch = true;
                if( !strSelect.isEmpty() )
                    strSelect += ", ";
                strSelect += fields->at(j)->getField();
                strAddSearchColumn += QString("    ui->tableViewSearch->addSearchColumn(%1);\n").arg(nCount);
                strAddSearchColumn += QString("    ui->tableViewSearch->addSearchColumnFilter(%1);\n").arg(nCount);
                strDelegates += getDelegateFor(nCount, fields->at(j)->getType());
                nCount++;
            }

            /// Delegates
            ///

        }
        if(nMasterSearch > -1)
        {
            strAddSearchColumn.insert(0, QString("    ui->tableViewSearch->addSearchColumn(%1);\n").arg(nMasterSearch));
            strAddSearchColumn.insert(0, QString("    ui->tableViewSearch->addSearchColumnFilter(%1);\n").arg(nMasterSearch));
        }
        else
        {
            strMasterSearch.sprintf("%d", 0);
            if( !strSelect.isEmpty() )
                strSelect += ", ";
            strSelect += fields->at(0)->getField();
            strAddSearchColumn.insert(0, QString("    ui->tableViewSearch->addSearchColumn(%1);\n").arg(0));

            qDebug() << "WARNING: No Master Search .. please add at least one parameter as :searchmaster or your manager will not work!!!";
        }

        if( !bHasSearch)
        {
            qDebug() << "WARNING: No Master Search .. please add at least one parameter as :search or your manager will not make sense!!!";
        }


        QString strHead = QString(CPP_SKEL).arg(Name).arg(strSelect).arg(strMasterSearch).arg(strAddSearchColumn).arg(strDelegates)
                .arg(fields->at(nMasterSearch==-1?0:nMasterSearch)->getField());

        file->write(strHead.toUtf8());

        file->flush();
        file->close();
    }
}

void ManagerSkeleton::BuildUI(skeleton *skel)
{
    QList<CTable *>*pTables = skel->GetTables();
    for( int i = 0; pTables && (i < pTables->count()); i++   )
    {
       QString Name = pTables->at(i)->getName();

       QFile *file = new QFile(PathMaker::Path()+"manager" + Name + ".ui");

       if(!file->open(QIODevice::WriteOnly))
       {
           qDebug() << "Impossivel abrir:"<<"Manager" + Name + ".ui"+".h"<<" para escrita" ;
           delete file;
           return;
       }

       QString strHead = QString(MANAGER_SKELETON_UI).arg(Name).arg(pTables->at(i)->getCaption());

       file->write(strHead.toUtf8());

       file->flush();
       file->close();
    }
}


