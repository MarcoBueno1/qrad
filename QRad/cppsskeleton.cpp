#include "cppsskeleton.h"
#include <QFile>


#define CPP_BODY "#include \"edit%1.h\"\n"\
"#include \"ui_edit%1.h\"\n"\
"#include <QCompleter>\n"\
"#include <QMessageBox>\n"\
"#include <QModelIndex>\n"\
"#include <QAbstractItemModel>\n"\
"#include <QVariant>\n"\
"%8\n"\
"\n"\
"Edit%1::Edit%1(QWidget *parent) :\n"\
"    QDialog(parent),\n"\
"    ui(new Ui::Edit%1)\n"\
"{\n"\
"    ui->setupUi(this);\n"\
"    \n"\
"    m_mod = NULL;\n"\
"    m_lastMod = NULL;\n"\
"    %4\n"\
"    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));\n"\
"    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));\n"\
"}\n"\
"\n"\
"Edit%1::~Edit%1()\n"\
"{\n"\
"    delete ui;\n"\
"}\n"\
"void Edit%1::showEvent(QShowEvent *event)\n"\
"{\n"\
"    Q_UNUSED(event);\n"\
"    %5\n"\
"    %6\n"\
"}\n"\
"void Edit%1::keyPressEvent(QKeyEvent *e)\n"\
"{\n"\
"    if(e->key() != Qt::Key_Escape)\n"\
"            QDialog::keyPressEvent(e);\n"\
"        else {Cancel();}\n"\
"}\n"\
"\n"\
"void Edit%1::SetModel(%1* mod)\n"\
"{\n"\
"   m_mod = mod;\n"\
"   Load();\n"\
"}\n"\
"\n"\
"\n"\
"void Edit%1::Save()\n"\
"{\n"\
"\n"\
"%2"\
"    \n"\
"}\n"\
"\n"\
"void Edit%1::Load()\n"\
"{\n"\
"%3\n"\
"}\n\n"\
"void Edit%1::Cancel()\n"\
"{\n"\
"    if( QMessageBox::Yes ==  QMessageBox::question(this, \"Cancelar?\",\"Deseja cancelar esta edição?\",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))\n"\
"           reject();\n"\
"}\n"\
"%1* Edit%1::GetSaved()\n"\
"{\n"\
"   return m_lastMod;\n"\
"\n"\
"}\n"\
"%7\n"



#define BN_PREPARE_CMB_EVNT "disconnect(ui->CmbBx%1, SIGNAL(OnEdit()), ui->CmbBx%1, SLOT(doEdit()));\n"



#define BN_CONNECT_EVNT     "connect(ui->CmbBx%1, SIGNAL(OnEdit()), this, SLOT(doEditCmbBx%1()));\n"

#define BN_EVNT_CMB_EDT "void Edit%1::doEditCmbBx%2()\n"\
"{\n"\
"    Edit%4* Edt =  new Edit%4;\n"\
"    QString strName;"\
"    if( Edt->exec() == QDialog::Accepted )\n"\
"    {\n"\
"      %4* mod = Edt->GetSaved();\n"\
"      if( mod )\n"\
"      {\n"\
"           strName = mod->get%3();"\
"           delete mod;\n"\
"      }\n"\
"    }\n"\
"    emit ui->CmbBx%2->OnUpdate(strName);\n"\
"    delete Edt;\n"\
"}\n"

#define BN_INCLUE "#include \"edit%1.h\"\n"\
                  "#include <QCompleter>\n"
cppsskeleton::cppsskeleton()
{
    m_IgnoreIfExist = false;
}

cppsskeleton::~cppsskeleton()
{

}
void cppsskeleton::Build( skeleton *Skel )
{
    if(!Skel->GetTables() || !Skel->GetTables()->at(0)->getFields())
    {
        qDebug() << "Warning: No table or no fields, we cant proceed ... \n";
        return;
    }

    QList<CTable *>* pTables = Skel->GetTables();

    for( int i = 0; i < pTables->count(); i++ )
    {
        QFile *file = new QFile(PathMaker::Path()+"edit" + pTables->at(i)->getName() + ".cpp");

        if( m_IgnoreIfExist && file->exists() )
        {
            qDebug() << "Already exist, Ignoring " << "Edit" << pTables->at(i)->getName() << ".cpp" ;
            return;
        }

        if(!file->open(QIODevice::WriteOnly))
        {
            qDebug() << "Impossivel abrir:"<<"Edit" << pTables->at(i)->getName()<< ".cpp" <<" para escrita" ;
            delete file;
            return;
        }

        QList<CField*>  *fields = pTables->at(i)->getFields();
    //    QList<QPair<bool, CType *> > modes = Skel->GetModes();


        QString  strInitCombos;
        QString  strModel = "    " + Skel->GetName() +"* mod =  m_mod;\n";
        strModel += "    if( m_mod == NULL)\n";
        strModel += QString("        ") + QString("mod = new ") + pTables->at(i)->getName() + QString(";\n\n");

        QString strDisconnect;
        QString strConnect;
        QString strCmbEdit;
        QString strIncludes;


        for( int j = 1; j< fields->count();j++)  /// skip primary key
        {
            if( fields->at(j)->getMode() == fHide ) // hide
                 continue;

            if( fields->at(j)->getMode() == fNoModel) // hide
                 continue;

            if( fields->at(j)->getType()->getType() == "QString" )
            {
                strModel += "    mod->set" + fields->at(j)->getCaption() + "(ui->LnEdt" +  fields->at(j)->getCaption() +"->text());\n";
            }
            if( fields->at(j)->getType()->getType() == "int" )
            {
                switch(fields->at(j)->getType()->geTpMode())
                {
                    case tpMulti:
                    case tpMultiEdtFull:
                    {
        //                strModel += "    mod->set" + fields.at(i).first->getCaption() + "(ui->CmbBx" +  fields.at(i).first->getCaption() +
        //                        "->model()->index.sibling("+"ui->CmbBx" +  fields.at(i).first->getCaption() +"->model()->index.row(),0).data().toInt());\n";

    //                    QString tmp = QString("\n        int row = ;\n").arg(fields.at(i).first->getCaption());
     //                   strModel += tmp;
                        strModel += QString("    mod->set%1(ui->CmbBx%1->model()->data(ui->CmbBx%1->model()->index(ui->CmbBx%1->currentIndex(), 0)).toInt());\n\n").arg(fields->at(j)->getCaption());

                        strInitCombos += QString("    ui->CmbBx%1->setTable(\"%2\");\n").arg(fields->at(j)->getCaption()).arg(fields->at(j)->getType()->getTable());
                        strInitCombos += QString("    ui->CmbBx%1->setField(\"%2\");\n").arg(fields->at(j)->getCaption()).arg(fields->at(j)->getType()->getField());
                        strInitCombos += QString("    ui->CmbBx%1->setCanAdd(true);\n").arg(fields->at(j)->getCaption());
                        strInitCombos += QString("    ui->CmbBx%1->setUserName(\"QRad\");\n").arg(fields->at(j)->getCaption());
                        strInitCombos += QString("    ui->CmbBx%1->completer()->setFilterMode(Qt::MatchContains );\n").arg(fields->at(j)->getCaption());
                        if( fields->at(j)->getType()->geTpMode() == tpMultiEdtFull )
                        {
                            if( strDisconnect.isEmpty())
                            {
                                strDisconnect += QString(BN_PREPARE_CMB_EVNT).arg(fields->at(j)->getCaption());
                            }
                            strConnect += QString(BN_CONNECT_EVNT).arg(fields->at(j)->getCaption());
                            strCmbEdit += QString(BN_EVNT_CMB_EDT).arg(pTables->at(i)->getName()).arg(fields->at(j)->getCaption()).arg(fields->at(j)->getType()->getField()).arg(fields->at(j)->getType()->getTable());
                            strIncludes += QString(BN_INCLUE).arg(fields->at(j)->getType()->getTable());

                        }
                    }
                    break;
                    default:
                        strModel += "    mod->set" + fields->at(j)->getCaption() + "(ui->SpnBx" +  fields->at(j)->getCaption() +"->value());\n";
                        break;
                }
            }
            if( fields->at(j)->getType()->getType() == "double" )
            {
                strModel += "    mod->set" + fields->at(j)->getCaption() + "(ui->DblSpnBx" +  fields->at(j)->getCaption() +"->value());\n";
            }
            if( fields->at(j)->getType()->getType() == "bool" )
            {
                strModel += "    mod->set" + fields->at(j)->getCaption() + "(ui->ChkBx" +  fields->at(j)->getCaption() +"->isChecked());\n";
            }
            if( fields->at(j)->getType()->getType() == "QDate" )
            {
                strModel += "    mod->set" + fields->at(j)->getCaption() + "(ui->DtEdt" +  fields->at(j)->getCaption() +"->date());\n";
            }
            if( fields->at(j)->getType()->getType() == "QTime" )
            {
                strModel += "    mod->set" + fields->at(j)->getCaption() + "(ui->TmEdt" +  fields->at(j)->getCaption() +"->time());\n";
            }

        }

        strModel += "    bool bRet = mod->Save();\n";
        strModel += "    if( m_lastMod )\n";
        strModel += "       delete m_lastMod;\n";
        strModel += "    m_lastMod = mod;\n";
        strModel += "    m_mod = NULL;\n";
    //    strModel += "    if( m_mod == NULL)\n";
    //    strModel += "       delete mod;\n";
        strModel += "    if( bRet )\n";
        strModel += "    {\n";
        strModel += "       QMessageBox::information(this, \"Sucesso!\",\"Informações foram salvas com sucesso!\");\n";
        strModel += "       accept();\n";
        strModel += "    }\n";
        strModel += "    else\n";
        strModel += "       QMessageBox::warning(this, \"Oops\",\"Não foi possivel salvar\");\n";



        QString strLoad  = "    if( m_mod == NULL)\n";
        strLoad         += "      return;\n";


        for( int j = 1; j< fields->count();j++)  /// skip primary key
        {
            if( fields->at(j)->getMode() == fHide ) // hide
                 continue;

            if( fields->at(j)->getMode() == fNoModel) // hide
                 continue;

            if( fields->at(j)->getType()->getType() == "QString" )
            {
                strLoad += "    ui->LnEdt" +  fields->at(j)->getCaption() +"->setText(m_mod->get" + fields->at(j)->getCaption() + "());\n";
            }
            if( fields->at(j)->getType()->getType() == "int" )
            {
                switch(fields->at(j)->getType()->geTpMode())
                {
                    case tpMulti:
                    case tpMultiEdtFull:
                              strLoad += "    ui->CmbBx" + fields->at(j)->getCaption() +"->setCurrentId(m_mod->get" + fields->at(j)->getCaption() + "());\n";
                              break;
                    default:
                              strLoad += "    ui->SpnBx" +  fields->at(j)->getCaption() +"->setValue(m_mod->get" + fields->at(j)->getCaption() + "());\n";
                              break;
                }
             }
            if( fields->at(j)->getType()->getType() == "double" )
            {
                strLoad += "    ui->DblSpnBx" +  fields->at(j)->getCaption() +"->setValue(m_mod->get" + fields->at(j)->getCaption() + "());\n";
            }
            if( fields->at(j)->getType()->getType() == "bool" )
            {
                strLoad += "    ui->ChkBx" +  fields->at(j)->getCaption() +"->setChecked(m_mod->get" + fields->at(j)->getCaption() + "());\n";
            }
            if( fields->at(j)->getType()->getType() == "QDate" )
            {
                strLoad += "    ui->DtEdt" +  fields->at(j)->getCaption() +"->setDate(m_mod->get" + fields->at(j)->getCaption() + "());\n";
            }
            if( fields->at(j)->getType()->getType() == "QTime" )
            {
                strLoad += "    ui->TmEdt" +  fields->at(j)->getCaption() +"->setTime(m_mod->get" + fields->at(j)->getCaption() + "());\n";
            }

        }


        QString strBody = QString(CPP_BODY).arg(pTables->at(i)->getName())
                .arg(strModel).arg(strLoad).arg(strInitCombos).arg(strDisconnect)
                .arg(strConnect).arg(strCmbEdit).arg(strIncludes);

        file->write(strBody.toUtf8());
        file->flush();
        file->close();
        delete file;
    }
}

void cppsskeleton::IgnoreIfExist(bool bIgnore)
{
    m_IgnoreIfExist = bIgnore;
}

