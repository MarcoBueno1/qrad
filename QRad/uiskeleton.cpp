#include <QFile>
#include "uiskeleton.h"


#define QRAD_LINE_EDIT_SEARCH            "<widget class=\"QRadLineEditFakeEnter\" name=\"lineEditSearch\">\n"\
"<property name=\"minimumSize\">\n"\
" <size>\n"\
"  <width>0</width>\n"\
"  <height>34</height>\n"\
" </size>\n"\
"</property>\n"\
"<property name=\"maximumSize\">\n"\
" <size>\n"\
"  <width>16777215</width>\n"\
"  <height>34</height>\n"\
" </size>\n"\
"</property>\n"\
"<property name=\"font\">\n"\
" <font>\n"\
"  <family>Verdana</family>\n"\
"  <pointsize>18</pointsize>\n"\
" </font>\n"\
"</property>\n"\
"</widget>\n"

#define QRAD_TABLEVIEW_SEARCH            "<widget class=\"QRadTableViewSearch\" name=\"tableViewSearch\">\n"\
"<property name=\"font\">\n"\
" <font>\n"\
"  <family>Verdana</family>\n"\
"  <pointsize>8</pointsize>\n"\
" </font>\n"\
"</property>\n"\
"<property name=\"focusPolicy\">\n"\
" <enum>Qt::NoFocus</enum>\n"\
"</property>\n"\
"<property name=\"styleSheet\">\n"\
" <string notr=\"true\">selection-background-color: rgba(102, 176, 239, 75%); selection-color: rgb(255, 255, 255);</string>\n"\
"</property>\n"\
"<property name=\"verticalScrollBarPolicy\">\n"\
" <enum>Qt::ScrollBarAlwaysOn</enum>\n"\
"</property>\n"\
"<property name=\"selectionBehavior\">\n"\
" <enum>QAbstractItemView::SelectRows</enum>\n"\
"</property>\n"\
"<property name=\"sortingEnabled\">\n"\
" <bool>true</bool>\n"\
"</property>\n"\
"<attribute name=\"horizontalHeaderMinimumSectionSize\">\n"\
" <number>20</number>\n"\
"</attribute>\n"\
"<attribute name=\"verticalHeaderVisible\">\n"\
" <bool>false</bool>\n"\
"</attribute>\n"\
"<attribute name=\"verticalHeaderMinimumSectionSize\">\n"\
" <number>18</number>\n"\
"</attribute>\n"\
"</widget>\n"\
"</item>\n"\
"</layout>\n"\
"</widget>\n"


#define QRAD_CUSTOM_WIDGETS  "<customwidgets>\n"\
"<customwidget>\n"\
" <class>QRadTableViewSearch</class>\n"\
" <extends>QTableView</extends>\n"\
" <header>QRadtableviewsearch.h</header>\n"\
"</customwidget>\n"\
"</customwidgets>\n"\


#define QRAD_EMPTY_UI "<ui version=\"4.0\">\n"\
"<class>Edit%1</class>\n"\
"<widget class=\"QDialog\" name=\"Edit%1\">\n"\
" <property name=\"geometry\">\n"\
"  <rect>\n"\
"   <x>0</x>\n"\
"   <y>0</y>\n"\
"   <width>800</width>\n"\
"   <height>600</height>\n"\
"  </rect>\n"\
" </property>\n"\
" <property name=\"windowTitle\">\n"\
"  <string>Edit</string>\n"\
" </property>\n"\
    "    <widget class=\"QFrame\" name=\"frame\">\n"\
    "     <property name=\"geometry\">\n"\
    "      <rect>\n"\
    "       <x>10</x>\n"\
    "       <y>50</y>\n"\
    "       <width>781</width>\n"\
    "       <height>500</height>\n"\
    "      </rect>\n"\
    "     </property>\n"\
    "     <property name=\"frameShape\">\n"\
    "      <enum>QFrame::WinPanel</enum>\n"\
    "     </property>\n"\
    "     <property name=\"frameShadow\">\n"\
    "      <enum>QFrame::Raised</enum>\n"\
    "     </property>\n"\
" %2"\
    "    </widget>\n"\
"<widget class=\"QFrame\" name=\"topmenu\">\n"\
" <property name=\"geometry\">\n"\
"  <rect>\n"\
"   <x>1</x>\n"\
"   <y>1</y>\n"\
"   <width>801</width>\n"\
"   <height>42</height>\n"\
"  </rect>\n"\
" </property>\n"\
" <property name=\"minimumSize\">\n"\
"  <size>\n"\
"   <width>0</width>\n"\
"   <height>42</height>\n"\
"  </size>\n"\
" </property>\n"\
" <property name=\"maximumSize\">\n"\
"  <size>\n"\
"   <width>16777215</width>\n"\
"   <height>42</height>\n"\
"  </size>\n"\
" </property>\n"\
" <property name=\"styleSheet\">\n"\
"  <string notr=\"true\">background-color: rgb(50, 0, 133);</string>\n"\
" </property>\n"\
" <property name=\"frameShadow\">\n"\
"  <enum>QFrame::Raised</enum>\n"\
" </property>\n"\
" <widget class=\"QLabel\" name=\"labelStatus\">\n"\
"  <property name=\"enabled\">\n"\
"   <bool>false</bool>\n"\
"  </property>\n"\
"  <property name=\"geometry\">\n"\
"   <rect>\n"\
"    <x>4</x>\n"\
"    <y>0</y>\n"\
"    <width>541</width>\n"\
"    <height>39</height>\n"\
"   </rect>\n"\
"  </property>\n"\
"  <property name=\"palette\">\n"\
"   <palette>\n"\
"    <active>\n"\
"     <colorrole role=\"WindowText\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Button\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Text\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"ButtonText\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Base\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Window\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"    </active>\n"\
"    <inactive>\n"\
"     <colorrole role=\"WindowText\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Button\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Text\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"ButtonText\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Base\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Window\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"    </inactive>\n"\
"    <disabled>\n"\
"     <colorrole role=\"WindowText\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Button\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Text\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"ButtonText\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>255</red>\n"\
"        <green>255</green>\n"\
"        <blue>255</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Base\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"     <colorrole role=\"Window\">\n"\
"      <brush brushstyle=\"SolidPattern\">\n"\
"       <color alpha=\"255\">\n"\
"        <red>50</red>\n"\
"        <green>0</green>\n"\
"        <blue>133</blue>\n"\
"       </color>\n"\
"      </brush>\n"\
"     </colorrole>\n"\
"    </disabled>\n"\
"   </palette>\n"\
"  </property>\n"\
"  <property name=\"font\">\n"\
"   <font>\n"\
"    <family>Verdana</family>\n"\
"    <pointsize>18</pointsize>\n"\
"    <weight>75</weight>\n"\
"    <bold>true</bold>\n"\
"   </font>\n"\
"  </property>\n"\
"  <property name=\"styleSheet\">\n"\
"   <string/>\n"\
"  </property>\n"\
"  <property name=\"frameShape\">\n"\
"   <enum>QFrame::NoFrame</enum>\n"\
"  </property>\n"\
"  <property name=\"frameShadow\">\n"\
"   <enum>QFrame::Plain</enum>\n"\
"  </property>\n"\
"  <property name=\"lineWidth\">\n"\
"   <number>1</number>\n"\
"  </property>\n"\
"  <property name=\"text\">\n"\
"   <string>Cadastro de %3</string>\n"\
"  </property>\n"\
"  <property name=\"textFormat\">\n"\
"   <enum>Qt::AutoText</enum>\n"\
"  </property>\n"\
"  <property name=\"alignment\">\n"\
"   <set>Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter</set>\n"\
"  </property>\n"\
"  <property name=\"wordWrap\">\n"\
"   <bool>false</bool>\n"\
"  </property>\n"\
"  <property name=\"textInteractionFlags\">\n"\
"   <set>Qt::NoTextInteraction</set>\n"\
"  </property>\n"\
" </widget>\n"\
"</widget>\n"\
    "%4\n"\
"</widget>\n"\
"    <customwidgets>\n"\
"     <customwidget>\n"\
"      <class>SComboBox</class>\n"\
"      <extends>QComboBox</extends>\n"\
"      <header location=\"global\">scombobox.h</header>\n"\
"     </customwidget>\n"\
"    </customwidgets>\n"\
"<resources/>\n"\
"<connections/>\n"\
"</ui>\n"


#define QRAD_SMART_COMBO   "<widget class=\"SComboBox\" name=\"CmbBx%1\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>%2</x>\n"\
"  <y>%3</y>\n"\
"  <width>120</width>\n"\
"  <height>33</height>\n"\
" </rect>\n"\
"</property>\n"\
    "    <property name=\"editable\">\n"\
    "     <bool>true</bool>\n"\
    "    </property>\n"\
"    <property name=\"sizeAdjustPolicy\">\n"\
"     <enum>QComboBox::AdjustToContents</enum>\n"\
"    </property>\n"\
"%4"\
"</widget>\n"\


/*



*/

#define BN_SMART_COMBO_ITEM    "<item>\n"\
"<property name=\"text\">\n"\
" <string>%1</string>\n"\
"</property>\n"\
"</item>\n"



// x = 100
// y = 50
#define QRAD_LINEEDIT "<widget class=\"QLineEdit\" name=\"LnEdt%1\">\n"\
" <property name=\"geometry\">\n"\
"  <rect>\n"\
"   <x>%2</x>\n"\
"   <y>%3</y>\n"\
"   <width>113</width>\n"\
"   <height>27</height>\n"\
"  </rect>\n"\
" </property>\n"\
"</widget>\n"

// x = 31
// y = 55
#define QRAD_LABEL   "<widget class=\"QLabel\" name=\"Lbl%1\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>%2</x>\n"\
"  <y>%3</y>\n"\
"  <width>59</width>\n"\
"  <height>17</height>\n"\
" </rect>\n"\
"</property>\n"\
"<property name=\"text\">\n"\
" <string>%1</string>\n"\
"</property>\n"\
"</widget>\n"

///// Passo 1:
///// X deve ser sempre igual
///// Y deve ser += 35

///// Passo 2:
///// X += 242
///// Y deve ser += 35


#define QRAD_DATE_EDIT   "<widget class=\"QDateEdit\" name=\"DtEdt%1\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>%2</x>\n"\
"  <y>%3</y>\n"\
"  <width>110</width>\n"\
"  <height>27</height>\n"\
" </rect>\n"\
"</property>\n"\
"</widget>\n"

#define QRAD_TIME_EDIT   "<widget class=\"QTimeEdit\" name=\"TmEdt%1\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>%2</x>\n"\
"  <y>%3</y>\n"\
"  <width>118</width>\n"\
"  <height>27</height>\n"\
" </rect>\n"\
"</property>\n"\
"</widget>\n"

#define QRAD_SPIN_BOX   "<widget class=\"QSpinBox\" name=\"SpnBx%1\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>%2</x>\n"\
"  <y>%3</y>\n"\
"  <width>111</width>\n"\
"  <height>27</height>\n"\
" </rect>\n"\
"</property>\n"\
"<property name=\"maximum\">\n"\
" <number>999999999999</number>\n"\
"</property>\n"\
"</widget>\n"

#define QRAD_DOUBLE_SPIN_BOX  "<widget class=\"QDoubleSpinBox\" name=\"DblSpnBx%1\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>%2</x>\n"\
"  <y>%3</y>\n"\
"  <width>111</width>\n"\
"  <height>27</height>\n"\
" </rect>\n"\
"</property>\n"\
"<property name=\"maximum\">\n"\
" <double>999999999999.989999999990687</double>\n"\
"</property>\n"\
"</widget>\n"

#define QRAD_CHECK_BOX   "<widget class=\"QCheckBox\" name=\"ChkBx%1\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>%2</x>\n"\
"  <y>%3</y>\n"\
"  <width>110</width>\n"\
"  <height>22</height>\n"\
" </rect>\n"\
"</property>\n"\
"<property name=\"text\">\n"\
" <string>%1</string>\n"\
"</property>\n"\
"</widget>\n"

#define QRAD_BTN_CANCEL   "<widget class=\"QPushButton\" name=\"PshBtnCancel\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>622</x>\n"\
"  <y>568</y>\n"\
"  <width>85</width>\n"\
"  <height>27</height>\n"\
" </rect>\n"\
"</property>\n"\
"<property name=\"text\">\n"\
" <string>&amp;Cancelar</string>\n"\
"</property>\n"\
"</widget>\n"

#define QRAD_BTN_SAVE   "<widget class=\"QPushButton\" name=\"PshBtnSave\">\n"\
"<property name=\"geometry\">\n"\
" <rect>\n"\
"  <x>709</x>\n"\
"  <y>568</y>\n"\
"  <width>85</width>\n"\
"  <height>27</height>\n"\
" </rect>\n"\
"</property>\n"\
"<property name=\"text\">\n"\
" <string>&amp;Salvar</string>\n"\
"</property>\n"\
"</widget>\n"



uiskeleton::uiskeleton()
{
    m_IgnoreIfExist = false;
}

uiskeleton::~uiskeleton()
{

}

void uiskeleton::Build(skeleton *skel)
{
   QList<CTable*> *pTables = skel->GetTables();

	qDebug() <<  __FILE__ << " LINE:" << __LINE__ ;
   for( int i = 0; pTables && (i < pTables->count()); i++ )
   {
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
	   
       QString Name = pTables->at(i)->getName();
       QFile *file = new QFile(PathMaker::Path()+"edit" + Name + ".ui");

       if( m_IgnoreIfExist && file->exists() )
       {
           qDebug() << "Already exist, Ignoring " << "Edit" + Name + ".ui"+".h";
           return;
       }

       if(!file->open(QIODevice::WriteOnly))
       {
           qDebug() << "Impossivel abrir:"<<"Edit" + Name + ".ui"+".h"<<" para escrita" ;
           delete file;
           return;
       }


       QString strBody;
       int x,y;
       x = 30;
       y = 10;
       int Count = 1;
       QList<CField *> *pFields = pTables->at(i)->getFields();
       for( int j = 1; pFields && (j< pFields->count());j++)  /// skip primary key
       {
           if( pFields->at(j)->getMode() == fHide ) // hide
               continue;

           if( pFields->at(j)->getType()->getType() == "QString" )
           {
               strBody += QString(QRAD_LABEL).arg(pFields->at(j)->getCaption()).arg(x).arg(y);
               strBody += QString(QRAD_LINEEDIT).arg(pFields->at(j)->getCaption()).arg(x+70).arg(y-5);
           }
           if( pFields->at(j)->getType()->getType() == "int" )
           {
               strBody += QString(QRAD_LABEL).arg(pFields->at(j)->getCaption()).arg(x).arg(y);
               switch(pFields->at(j)->getType()->geTpMode())
               {
                   case tpMulti:
                   case tpMultiEdtFull:
                           {
                               QString items;
                               for( int k =0; k < pFields->at(j)->getType()->getDefaults().count(); k++ )
                                     items += QString(BN_SMART_COMBO_ITEM).arg(pFields->at(j)->getType()->getDefaults().at(k));

                               strBody += QString(QRAD_SMART_COMBO).arg(pFields->at(j)->getCaption()).arg(x+70).arg(y-5).arg(items);
                           }
                           break;
                   default:
                          strBody += QString(QRAD_SPIN_BOX).arg(pFields->at(j)->getCaption()).arg(x+70).arg(y-5);
                          break;
               }
           }
           if( pFields->at(j)->getType()->getType() == "double" )
           {
               strBody += QString(QRAD_LABEL).arg(pFields->at(j)->getCaption()).arg(x).arg(y);
               strBody += QString(QRAD_DOUBLE_SPIN_BOX).arg(pFields->at(j)->getCaption()).arg(x+70).arg(y-5);

           }
           if( pFields->at(j)->getType()->getType() == "bool" )
           {
               strBody += QString(QRAD_CHECK_BOX).arg(pFields->at(j)->getCaption()).arg(x+70).arg(y-5);
           }
           if( pFields->at(j)->getType()->getType() == "QDate" )
           {
               strBody += QString(QRAD_LABEL).arg(pFields->at(j)->getCaption()).arg(x).arg(y);
               strBody += QString(QRAD_DATE_EDIT).arg(pFields->at(j)->getCaption()).arg(x+70).arg(y-5);
           }
           if( pFields->at(j)->getType()->getType() == "QTime" )
           {
               strBody += QString(QRAD_LABEL).arg(pFields->at(j)->getCaption()).arg(x).arg(y);
               strBody += QString(QRAD_TIME_EDIT).arg(pFields->at(j)->getCaption()).arg(x+70).arg(y-5);
           }


           if( Count == 15 )
           {
               y = 10;
               x += 242;

           }
           else if( Count == 30 )
           {

               y = 10;
               x += 242;
           }
           else
           {
               y += 35;
           }
           if( Count > 45 )
           {
               qDebug() << "Warning, to mutch fields. Maximum is 45!!";
           }
           Count++;
       }

       QString strFooter = QString(QRAD_BTN_CANCEL);
       strFooter += QString(QRAD_BTN_SAVE);

       QString strHead = QString(QRAD_EMPTY_UI).arg(Name).arg(strBody).arg(pTables->at(i)->getCaption()).arg(strFooter);

       file->write(strHead.toUtf8());

       file->flush();
       file->close();
   }
	qDebug() <<  __FILE__ << " LINE:" << __LINE__ ;
}

void uiskeleton::IgnoreIfExist(bool bIgnore)
{
    m_IgnoreIfExist = bIgnore;
}
