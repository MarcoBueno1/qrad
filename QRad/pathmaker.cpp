#include "pathmaker.h"

#include <QDir>
#include <QDebug>

QString PathMaker::m_child = "";


PathMaker::PathMaker()
{

}

PathMaker::~PathMaker()
{

}
void PathMaker::setDir(QString strDir )
{
      m_child = strDir;
	  
#ifdef Q_WS_WIN
   if( !m_child.endsWith("\\") )
       m_child += "\\";
#else
   if( !m_child.endsWith("/") )
       m_child += "/";
#endif
	  
}


QString PathMaker::Path()
{
   QString path = QDir::currentPath();

   qDebug() << "Path: " <<path ;

#ifdef Q_WS_WIN
   if( !path.endsWith("\\") )
       path += "\\" + QString("built\\");
#else
   if( !path.endsWith("/") )
       path += "/" + QString("built/");
#endif
   QDir dir;

   path += m_child;
   dir.mkpath(path);
   return path;
}


