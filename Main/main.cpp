#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QBitmap>
#include <QSqlDatabase>
#include "qradsplashscreen.h"
#include "qradcoreplugin.h"
#include "mainwindow.h"
#include "qraddebug.h"
#include "qradplugininterface.h"
#include "qradplugincontainer.h"


#include <QString>
#include <QList>
#include <QStringList>


//int nMandrakeCount = 0;

QList<QStringList *> m_AFN;

///
/// Apenas montagtem fixa de transicoes ( pode ser adaptado para alimentar elementos a partir da grid)
///
void BuildTransitions()
{
	QStringList *pTransition;
	
	pTransition = new QStringList;

	// novo formato, sem inserir o numero da entrada na lista ( apenas a matriz conforme se escreve) 
	//    inicio                    0                            1     
	pTransition->append("S"); pTransition->append("A,B"); pTransition->append("");      m_AFN.append(pTransition);pTransition = new QStringList;
	pTransition->append("B"); pTransition->append("X");   pTransition->append("A,B");   m_AFN.append(pTransition);pTransition = new QStringList;
	pTransition->append("A"); pTransition->append("B");   pTransition->append("A");     m_AFN.append(pTransition);pTransition = new QStringList;
	pTransition->append("X"); pTransition->append("");    pTransition->append("");      m_AFN.append(pTransition);//pTransition = new QStringList;
}

void DumpLine(QStringList * pTransition)
{
 QString strDebug;
 
 for( int j = 0; j< pTransition->count(); j++)
	 strDebug += pTransition->at(j) + "    ";
	 
 qDebug() << strDebug;
}

void DumpList()
{
 QString strDebug;
	
 for( int i = 0; i  < m_AFN.count(); i++ )
 {
	 QStringList *transition = m_AFN.at(i);
	 for( int j = 0; j< transition->count(); j++)
	     strDebug += transition->at(j) + "    ";
	 
	 qDebug() << strDebug;
	 strDebug ="";
 }
}

void Transform(QString current, int nTCurr, QStringList *pTransition, int niAfn )
{
  QStringList slCurrent = current.split(",");
  bool bLineChanged = false;
  for( int n = 0; n < slCurrent.count(); n++ )
  {
	  QString Charac = slCurrent.at(n);
	  for( int y =0/*nTCurr*//*+1*/; y < pTransition->count(); y++ )
	  {
		  //DumpLine(pTransition);
		  QString subCurrent =  pTransition->at(y);
		  if( subCurrent == current )
		  {
			  pTransition->replace(y, QString(subCurrent).remove(",")); // retira virgula do proximo (que foi encontrado).
			  bLineChanged = true;
		  }
		  else if( Charac == subCurrent )
		  {
			  pTransition->replace(y, QString(current).remove(",") );
			  bLineChanged = true;
		  }
		  else if( !subCurrent.isEmpty() && bLineChanged && !current.contains(subCurrent) && !subCurrent.contains(Charac)) // se nesta linha tem uma transicao simples, então adiciona a letra na frente
		  {
			  /// deve-se guardar este caso para gerar uma nova transicao,
			  /// tipico caso.....: B  X   -
			  //      ficaria.....: AB BX  -
			  //   teria que criar: BX -   X
			  
			  
			  QString newTransition = QString("%1%2").arg(Charac).arg(subCurrent);
			  
//			  qDebug() << "current....:" <<current;
//			  qDebug() << "Adicionando:" <<newTransition;
//			  qDebug() << "niAfn......:" <<niAfn;
			  
			  pTransition->replace(y, newTransition);
			  QStringList *pNewTransition = new QStringList;
			  
			  pNewTransition->append(newTransition);
			  pNewTransition->append(subCurrent);
			  pNewTransition->append(QString(current).remove(","));
			  
			  for( int ii = 0; ii < pTransition->count()-3; ii++ )
				  pNewTransition->append("");
//			  pNewTransition->append(subCurrent);
			  m_AFN.insert(niAfn+1, pNewTransition);
			  
//			  if( nMandrakeCount < 5 ) 
//				  nMandrakeCount++;
//			  else /// zebrovisky
//				  return;
			  
		  }
		  else
		  {
			 // qDebug() << "ignorado....:" <<subCurrent << " item do par:" << Charac << "  Linha indice:" << y;
			  
		  }
	  }
  }

  
  if( m_AFN.count() > (niAfn+1))
  {
	 /// recursividade ( )
//	qDebug() << "recursividade para:" <<niAfn+1;
//	qDebug() << "item......... ....:" <<current;
	QStringList *pTr = m_AFN.at(niAfn+1);
	Transform( current, 0, pTr, niAfn+1 );  
  }		  
  
}


void Search( QStringList *pTransition, int niAfn) // pasar o current para usar recursividade ( current estilo "A,B")
{
  for( int j=0; j < pTransition->count(); j++)
  {
	  QString current = pTransition->at(j);

	  if( current.contains(","))
	  {
	      pTransition->replace(j, QString(current).remove(",")); // retira virgula do corrente apenas.
          Transform(current, j, pTransition, niAfn);		  
	  }
  }

}

void RemoveInvalid()
{
	 for( int i=0; i < m_AFN.count(); i++ )
	 {
		QStringList *pTransition = m_AFN.at(i);
		QString strFirst = pTransition->at(0);
		bool Remove = true;
		for( int j = 1; j < pTransition->count(); j++ )
		{
			QString strCurrent = pTransition->at(j);
			if( strFirst != strCurrent )
				Remove = false;
		}
		if( Remove )
		{
			delete pTransition ;
			m_AFN.removeAt(i);
			i = 0;
		}
	 }
	
}

void Cleanup()
{
	 for( int i=0; i < m_AFN.count(); i++ )
	 {
		 delete m_AFN.at(i);
		 m_AFN.removeAt(i);
		 i =0;
	 }
}

void MAIN()
{	
	 BuildTransitions();
	 
	 qDebug() << "Matriz Inicial:";
	 DumpList();
	 
	 for( int i=0; i < m_AFN.count(); i++ )
	 {
		QStringList *pTransition = m_AFN.at(i);
		Search( pTransition, i );
	 }

	 RemoveInvalid();
	 qDebug() << " ";
	 qDebug() << "Matriz Processada:";
	 DumpList();
	 
	 Cleanup();
}


#if(0)


QString FindX(QString compos)
{
 for( int i = 0; i < m_AFN.count() ;i++ )
 {
	 QString value   = m_AFN.at(i)->at(2);
	 if(!value.isEmpty() && !compos.contains(value))
		 return value;
 }
  return "X";
}


void Swap()
{
//	 State = m_AFN.at(i)->at(0);
 int i;
 QString Entry = m_AFN.at(0)->at(1);
	
 for(  i = 0; i < m_AFN.count() ;i++ )
	 if( m_AFN.at(i)->at(1) != Entry )
		 break;
 if(i == m_AFN.count()) // apenas uma entrada
	 return ;
  	 
 for( int j = i; j < m_AFN.count() ; j++ )
 {
	 QString Value = m_AFN.at(j-i)->at(0);
	 m_AFN.at(j)->replace(0, Value);
 }
 
}

void MAIN()
{
 QString LastEntry = "0";	
	
 BuildTransitions();
 
 
 for( int i = 0; i < m_AFN.count() ;i++ )
 {
	 QString State      = m_AFN.at(i)->at(0);
	 QString Entry      = m_AFN.at(i)->at(1);
	 QString NewState   = m_AFN.at(i)->at(2); 
	 
	 if( LastEntry != Entry )
	 {
		 Swap();
	 }

	//if( Entry == "1" )				
	{
		qDebug() << "fora da logica" << " " << m_AFN.at(i)->at(0) << " " << m_AFN.at(i)->at(1) << "" << m_AFN.at(i)->at(2);
		
	}

	 
     if( NewState.contains(",") ) // ok,trata-se de uma transicao multipla, transforma-la em AFD
	 {
		 
			if( Entry == "1" )				
			{
				qDebug() << "entrou no contains" << " " << m_AFN.at(i)->at(0) << " " << m_AFN.at(i)->at(1) << "" << m_AFN.at(i)->at(2);
				
			}
		 
		QString strAux = FindX(NewState);
        QStringList strMultiTran = NewState.split(","); // quebra string em lista de strings, token para quebra = ','
		for( int j = 0; j < strMultiTran.count(); j++ )// processamento de cada transicao composta
		{
			for( int k = 0; k < m_AFN.count(); k++ ) // percorre a maquina procurando por transicoes para o estado corrente
			{
                if( Entry == "1" )				
				{
					qDebug() << "dentro da logica" << " " << m_AFN.at(k)->at(0) << " " << m_AFN.at(k)->at(1) << "" << m_AFN.at(k)->at(2);
					
				}
				
				QString CurrentStatus = m_AFN.at(k)->at(0);
				if( (CurrentStatus == strMultiTran.at(j)) && (Entry==m_AFN.at(k)->at(1))) /// encontrei uma transicao
				{
				   QString Mix = NewState;
				   Mix.remove(",");
				   m_AFN.at(k)->replace(0,Mix);
				   
				   QString TransCompos = m_AFN.at(k)->at(2);
				   if( TransCompos.contains(","))
				   {
					   TransCompos = TransCompos.remove(",");
				   }
				   else 
				   {
					  TransCompos =  CurrentStatus + m_AFN.at(k)->at(2);   /// soma.
				   }
				   
				   if( Mix == TransCompos ) // transicao nula eh a do bx
				   {
					   QString aux = Mix;
					   aux = aux.remove(CurrentStatus);
					   
					   Mix = aux;//Mix.remove(aux);
					   Mix += strAux;
				       m_AFN.at(k)->replace(0,Mix);
				       m_AFN.at(k)->replace(2,strAux);
					   
				   }
				   else
				   m_AFN.at(k)->replace(2,TransCompos);
//				   m_AFN.at(k)->replace(1, "10");
                   //AdjustOldMachine(Entry,  m_AFN.at(k)->at(2));					
				}
				else if( Entry == "1" )//// contains ??
				{
					qDebug() << "IF EFETIVO" << " " << m_AFN.at(k)->at(0) << " " << m_AFN.at(k)->at(1) << "" << m_AFN.at(k)->at(2);
					QString M = NewState;
					M = M.remove(",");
					m_AFN.at(k)->replace(2, M);
				}
				else
				{
					qDebug() << "IF EFETIVO eNTRY"  << Entry << " " << m_AFN.at(k)->at(0) << " " << m_AFN.at(k)->at(1) << "" << m_AFN.at(k)->at(2);
					
					QString strAux2 = m_AFN.at(k)->at(2);
					if(strAux2.contains(",") && (Entry==m_AFN.at(k)->at(1)))
					{
						strAux2.remove(",");
						m_AFN.at(k)->replace(2, strAux2);
					}
					//else if( CurrentStatus.contains(strAux2))
					//{
//						m_AFN.at(k)->replace(2, CurrentStatus);
					//}
				}
			}
		}
	 }
	 else /// aparentemente transicao normal.
	 {
	if( Entry == "1" )				
	{
		qDebug() << "fora da logica" << " " << m_AFN.at(i)->at(0) << " " << m_AFN.at(i)->at(1) << "" << m_AFN.at(i)->at(2);
		
	}
		 
	 }
	 
	 
 }
 
 DumpList();
 
 
 
 /*
 
 


 /// fazer este loop para cada entrada ..
 for( int a = 0; a < m_AFN.count(); a++)
 {
     QString Entry = m_AFN.at(a)->at(1);
     for( int i = 0; i < m_AFN.count(); i++ )
     {

         if( Entry != m_AFN.at(i)->at(1)) // só deixar passar se for a mesma entrda ...
             continue;

         /// para cada linha, verifica se existe
         /// transicao composta, caso exista, soma as transicoes para
         /// montar a nova maquina de estados
         QStringList *pTransition = new QStringList;

         QString State = m_AFN.at(i)->at(0);
         QString Transition = m_AFN.at(i)->at(2);
         QString TransitionTmp = m_AFN.at(i)->at(2);


         /// Adicionando Transicao na maquina AFD 
		 ///( verificar se precisa fazer if, pois "B" ou "A" podem estar já nesta primeira coluna ...)
         pTransition->append(State);  pTransition->append(Entry);  pTransition->append(TransitionTmp.remove(","));m_AFD.append(pTransition);pTransition = new QStringList;

         if( Transition.contains(",") ) // ok,trata-se de uma transicao multipla, transforma-la em AFD
         {
            QStringList strTransitions = Transition.split(","); // quebra string em lista de strings, token para quebra = ','

            State = TransitionTmp.remove(",");
            Transition= "";
            bool bInsert = false;
            qDebug() << "No Loop";
            for( int j = 0; j < strTransitions.count(); j++ )
            {
                for( int k = 0; k < m_AFN.count(); k++ ) // percorre a maquina procurando por transicoes para o estado corrente
                {
                    QString CurrentStatus = m_AFN.at(k)->at(0);
                    if( (CurrentStatus == strTransitions.at(j)) && (Entry==m_AFN.at(k)->at(1))) /// encontrei uma transicao
                    {
                        Transition +=  m_AFN.at(k)->at(2);   /// soma.
                        if( !strTransitions.contains(m_AFN.at(k)->at(2)) ) // tratar casos como o "BX" do exemplo ....
                        {
                            bInsert= true;
                            //// percorre os ja inseridos na maquina nova para ajustar
                            for( int n = 0; n<m_AFD.count(); n++)
                                if(( m_AFD.at(n)->at(0) == m_AFN.at(k)->at(2)) && (Entry==m_AFD.at(n)->at(1))) // achou um que ja tinha sido inserido, ajusta
                                {
                                    qDebug() << "Encontrei BX na maquina antiga";
                                        QString str = m_AFD.at(n)->at(0);
                                        str.insert(0,m_AFN.at(k)->at(0));
                                        m_AFD.at(n)->replace(0,str);
                                }

                            //// percorre os da maquina velha e ajusta
                            for( int m = 0; m<m_AFN.count(); m++)
                                if(( m_AFN.at(m)->at(0) == m_AFN.at(k)->at(2)) && (Entry==m_AFN.at(m)->at(1))) // achou um que ja tinha sido inserido, ajusta
                                {
                                    qDebug() << "Encontrei BX na maquina nova";
                                    QString str = m_AFN.at(m)->at(0);
									QString str2 = str;
									QString toAppend = m_AFN.at(m)->at(2);
									
                                    str2.insert(0,m_AFN.at(k)->at(0));
                                    m_AFN.at(m)->replace(2,m_AFN.at(m)->at(0));
                                    m_AFN.at(m)->replace(0,str2);
									
									/// insere a tansicao tipo AB -> BX
                                    pTransition->append(State);  pTransition->append(Entry);  pTransition->append(str2);m_AFD.append(pTransition);pTransition = new QStringList;									

                                    pTransition->append(str);  pTransition->append(Entry);  pTransition->append(toAppend);m_AFD.append(pTransition);pTransition = new QStringList;

    //                                    .insert(0,m_AFN.at(k)->at(0));
                                }


                        }
                        // remove transicao
                        QStringList *pTr = m_AFN.at(k);
                        delete pTr;
                        m_AFN.removeAt(k);
                        k--;
                        //continue;
                    }
                }
            }
            //CurrentState = CurrentTransition.remove(",");
            if( bInsert )
            {
                if( (State.length() > 1) && (State == TransitionTmp) )
                    ;
                else
				{

                pTransition->append(State);  pTransition->append(Entry);  pTransition->append(TransitionTmp);m_AFD.append(pTransition);pTransition = new QStringList;
				}
            }
           // delete m_AFD.at(i);
           // m_AFD.removeAt(i);
          //  i = 0;
         }
         else
         {
             delete m_AFN.at(i);
             m_AFN.removeAt(i);
			 if( i )i--;
             a = 0; ///
         }
     }
	 
	 /// Consideracao:
	 /// - A alimentacao das linhas x colunas deve ser na ordem dada pela funcao BuildTransitions
     ///   Obrigatoriamente na mesma ordem, caso a alimentacao seja fora de ordem ( ex: entradas "0" misturadas com entradas "1") vai dar problema
	 ///    o trecho abaixo leva isto em consideracao
	 /// Aqui a primeira coluna do AFN, vira o resultado da mesma coluna já calculado e ja inserido no AFS
	 
	 ////----------	 achar primeiro indice do AFD ( cuja "Entrada" eh a mesma que a ultima processada no trecho acima )

	 int Count;
	 for( Count=0; Count <  m_AFD.count(); Count++ )
		 if( m_AFD.at(Count)->at(1).toInt() == Entry.toInt() )
			 break;
     ////----------------------
	 qDebug() << "AFN Count:" << m_AFN.count() << "Entry.toInt() +1: " << Entry.toInt() +1;

	 /// Pegar a entrada cujo numero é Entry.toInt() +1 e fazer com que a coluna "S" dela seja igual a que foi processada agora ( pelo trecho acima )
	 for( int ii = 0; (ii < m_AFN.count()) ; ii++ )
	 {
//		 qDebug() << "loopsinho basico, " << "m_AFN.at(ii)->at(1).toInt():" << m_AFN.at(ii)->at(1).toInt();
		 
		 if(m_AFN.at(ii)->at(1).toInt() == (Entry.toInt() +1)) 
		 {
			/// substituir a coluna "S" original pelo respectivo resultado do processamento acima ( linha antes de 170 ).
			 if( Count < m_AFD.count() )			 
			 {
				 qDebug() << "Fez uma troca de:" << m_AFN.at(ii)->at(0) << " Para: " << QString(m_AFD.at(Count)->at(0).toUtf8());
	//			m_AFN.at(m)->replace(2,m_AFN.at(m)->at(0));
				 m_AFN.at(ii)->replace(0, QString(m_AFD.at(Count++)->at(0).toUtf8()));
			 }
		 }
		 else if( m_AFN.at(ii)->at(1).toInt() == Entry.toInt() ) // ja deveria ter sido removido ... 
		 {
			 qDebug() << "Matei um zumbi.. ,entry:" << Entry.toInt() <<  "dados( " << m_AFN.at(ii)->at(0) << "  " << m_AFN.at(ii)->at(1) <<  " " << m_AFN.at(ii)->at(2) ; 
			// delete m_AFN.at(ii);
			// m_AFN.removeAt(ii);
			// if( ii > 0 ) ii--;
		 }
	 }
	 /////------------------------------------------------------
	 
	 /// Aqui temos ja calculados todos os valores da primeira entrada
	 /// podemos então realimentar a coluna inicial da entrada 1
	 
    // a--;
     //while( )
    // delete m_AFN.at(a);
    // m_AFN.removeAt(a);
    // a--;
 }
 //// loop para apenas mostrar o resultado ( nova maquina criada)
 qDebug() << "Estado   Entrada   << NovoEstado";
 for( int y = 0; y  < m_AFD.count(); y++ )
 {
//     if( (m_AFD.at(y)->at(0).length() > 1) && (m_AFD.at(y)->at(0) == m_AFD.at(y)->at(2)))
 //        ;
 //    else
     qDebug() << m_AFD.at(y)->at(0) << "   " << m_AFD.at(y)->at(1) << "   " << m_AFD.at(y)->at(2);
     delete m_AFD.at(y);
 }
*/

}

#endif
void centerWidget(QWidget *widget)
{
    QRect rect = QApplication::desktop()->availableGeometry(widget);
    widget->move(rect.center() - widget->rect().center());
}

void configureDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL7");
///    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName("qraddb");
    db.setHostName("localhost");
    db.setPassword("dsmpassword");
    db.setUserName("dsm");

    if(!db.open())
    {
        qDebug() <<"Impossivel conectar no banco";
    }
    else
    {
        qDebug() << "Conexão Ok";
    }

}

QRadPluginLoader *g_pluginLoader;
MainWindow      *g_mainWindow;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MAIN();

    QRadSplashScreen *splash;
    QPixmap pix(":/png/splash.png");
//    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    int rc;

    splash = new QRadSplashScreen(pix, Qt::WindowStaysOnTopHint);
    splash->setMask(pix.mask());


    /* Configure database */

    if(( argc < 2) || strcmp( argv[1], "-nosql"))
    {
      debug_message("[main] -->configureDatabase...\n");

      configureDatabase();

      debug_message("[main] <--configureDatabase...\n");
    }

    /* Translate application components */

//    QTranslator tradutorExaro;
    QTranslator tradutorQt;

//    tradutorExaro.load(":/translate/exaro_pt", QCoreApplication::applicationDirPath());
    tradutorQt.load(":/translate/qt_pt", QCoreApplication::applicationDirPath());
//    a.installTranslator(&tradutorExaro);
    a.installTranslator(&tradutorQt);

    splash->show();
    splash->setMessage("Configurando janela principal");
    a.processEvents();

    g_mainWindow = new MainWindow();
    centerWidget(g_mainWindow);
    g_mainWindow->setParent(0,0);

    splash->setMessage("Carregando Plugins");
    a.processEvents();

    g_pluginLoader = new QRadPluginLoader(g_mainWindow,splash);
    g_pluginLoader->Load("plugins.xml");

    splash->finish(g_mainWindow);


    if((argc > 3) &&  !strcmp(argv[1], "--run"))
    {

        QString name = QString(argv[2]);
        QRadPluginInterface *iface = QRadPluginContainer::getInstance()->plugin(name);
        if (iface)
        {
            QString action = QString(argv[3]);
            iface->Process(action);
        }
    }
    else
    {
      g_mainWindow->setParent(0, 0);
      g_mainWindow->show();
    }

    rc = a.exec();

    // manter sempre esta ordem de delete, pois o objeto mainwindow utiliza o objeto pluginloader

    delete splash;
    delete g_pluginLoader;
    delete g_mainWindow;

    return rc;
}
