#include "qradhead.h"
#include "uiskeleton.h"
#include "cppsskeleton.h"
#include "hskeleton.h"
#include "controllerskeleton.h"
#include "managerskeleton.h"
#include "projectskeleton.h"
#include "type.h"

QRadHead::QRadHead()
{

}

QRadHead::~QRadHead()
{
	for( int i=0; i< m_skels.count(); i++)
	{
		delete m_skels.at(i);
	}
}
void QRadHead::addSkel(skeleton *skel)
{
    m_skels.append(skel);
}
void QRadHead::Build()
{
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
	
	skeleton *skel;
	
	for( int i=0; i< m_skels.count(); i++)
	{
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
        skel = m_skels.at(i);
        skel->CreateFile();
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

        uiskeleton *ui = new uiskeleton;
        ui->Build(skel);
        delete ui;
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

        cppsskeleton *cpp = new cppsskeleton;
        cpp->Build(skel);
        delete cpp;
		
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

        hskeleton *h = new hskeleton;
        h->Build(skel);
        delete h;

	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

        ManagerSkeleton *mng =  new ManagerSkeleton;
        mng->Build(skel);
        delete mng;
		
	qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

        QList<CTable*>* pTables = skel->getTables();
        qDebug() << "pTables->count():" << pTables->count();
        for( int j = 0; pTables && (j < pTables->count());j ++ )
        {
            qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

            //// cria subtabelas para cada item multi
            QList<CField*> *fields = pTables->at(j)->getFields();
            for( int k = 0; fields && (k < fields->count()); k++ )
            {
                qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
                qDebug() << "type::"  <<  fields->at(k)->getType();
                qDebug() << "Table::"  <<  fields->at(k)->getType()->getTable();
                qDebug() << fields->at(k)->getType()->getTable();
                qDebug() << fields->at(k)->getType()->geTpMode();


                if( !fields->at(k)->getType()->getTable().isEmpty() &&
                        ((fields->at(k)->getType()->geTpMode() == tpMulti)
                        ||(fields->at(k)->getType()->geTpMode() == tpMultiEdtFull)))
                {
                    CTable *tbl  = new CTable ;
                    CField *fld  ;

					
                    skeleton *subskel =  new skeleton();
                    subskel->IgnoreIfExist(true);


                    subskel->setName(fields->at(k)->getType()->getTable());
                    tbl->setName(fields->at(k)->getType()->getTable());
                    subskel->addTable(tbl);


                    fld = new CField;
                    fld->setField("id", "int");
                    tbl->addField(fld);

                    fld  = new CField;
                    fld->setField(fields->at(k)->getType()->getField(), "QString");
                    tbl->addField(fld);

                    fld  = new CField;
                    fld->setField("tp", "int");
                    fld->setMode(fHide);
                    tbl->addField(fld);

                    subskel->CreateFile();

                    uiskeleton *subui = new uiskeleton;
                    subui->IgnoreIfExist(true);
                    subui->Build(subskel);
                    delete subui;

                    cppsskeleton *subcpp = new cppsskeleton;
                    subcpp->IgnoreIfExist(true);
                    subcpp->Build(subskel);
                    delete subcpp;

                    hskeleton *subh = new hskeleton;
                    subh->IgnoreIfExist(true);
                    subh->Build(subskel);
                    delete subh;

                    ManagerSkeleton *submng =  new ManagerSkeleton;
    //                subh->IgnoreIfExist(true);
                    submng->Build(subskel);
                    delete submng;

                    delete subskel;
                }
            }

            qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
        }
        qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
         projectskeleton *pro = new projectskeleton;
         qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
         pro->Build(skel);
         qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;
         delete pro;
         qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

    }
    qDebug() << "Build::"  << __FILE__ << " LINE:" << __LINE__ ;

	
}
int QRadHead::Run()
{
	/// do initial procedures ( if needed )
	
	Build();
	return 0;
}
