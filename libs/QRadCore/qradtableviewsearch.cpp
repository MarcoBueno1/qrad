#include "qradtableviewsearch.h"
#include <QApplication>
#include <QDebug>
#include <QToolTip>
#include <QHeaderView>
#include "qraddebug.h"
#include <QMimeData>


QRadTableViewSearch::QRadTableViewSearch(QWidget *parent) :
        QTableView(parent),
        m_proxyModel(new QSortFilterProxyModel)
{   
    m_averageRow  = 0;
    m_InUse = 0;
    m_noEmptySearch = true;
    m_timerAfterShowEvent = NULL;
}

QRadTableViewSearch::~QRadTableViewSearch()
{
    delete m_proxyModel;    
    //delete m_time;
}

void QRadTableViewSearch::keyPressEvent(QKeyEvent *event)
{
    if(m_InUse)
        return;

    if (NULL == model() || (0 == model()->rowCount()))
    {
        qDebug() << "model is NULL or has no data";
        return;
    }

    if (!currentIndex().isValid())
        selectRow(0);

    int currentRow = currentIndex().row();

    CalculateRow(currentRow);

    switch(event->key())
    {
        case Qt::Key_PageUp:
        {
            if ((currentRow - (rowsPerViewport() + getAverageRow())) >= 0)
            {
                selectRow(currentRow - (rowsPerViewport() + getAverageRow()));
                selectRow(currentIndex().row() + getAverageRow() - 1);
            }
            else if ((currentRow - rowsPerViewport()) >= 0)
            {
                selectRow(currentRow - rowsPerViewport());
            }
            else
            {
                selectRow(0);
            }
            emit CurrentChanged( currentIndex() );
        }
        break;

        case Qt::Key_Up:
        {
            if ((currentRow - getAverageRow()) >= 0)
            {
                this->selectRow(currentRow - getAverageRow());
                this->selectRow(currentIndex().row() + getAverageRow() - 1);

                event->accept();
                emit CurrentChanged( currentIndex() );

            }
            else if (currentRow > 0)
            {
                selectRow(currentRow - 1);
                event->accept();
                emit CurrentChanged( currentIndex() );
            }
            else event->ignore();
        }
        break;

        case Qt::Key_PageDown:
        {
            if ((model()->rowCount() - currentRow) > (rowsPerViewport() + getAverageRow()))
            {
                selectRow(currentRow + rowsPerViewport() + getAverageRow());
                selectRow(currentIndex().row() - getAverageRow() + 1);
            }
            else if ((model()->rowCount() - currentRow) > rowsPerViewport())
            {
                selectRow(model()->rowCount() + rowsPerViewport());
            }
            else
            {
                selectRow(model()->rowCount() - 1);
            }

            emit CurrentChanged( currentIndex() );
            event->accept();
        }
        break;

        case Qt::Key_Down:
        {
            if ((model()->rowCount() - currentRow) > getAverageRow())
            {
                selectRow(currentRow + getAverageRow());
                selectRow(currentIndex().row() - getAverageRow() + 1);
                emit CurrentChanged( currentIndex() );
            }
            else if (currentRow < (model()->rowCount() - 1))
            {
                selectRow(currentRow + 1);
                emit CurrentChanged( currentIndex() );
            }
            else
            {
                emit RequestAppend();

            }
        }
        break;
        case  Qt::Key_F12:
        {
             if(!m_strLastString.trimmed().isEmpty())
                 searchSortModel(m_strLastString, false);
        }
        break;
        case Qt::Key_Delete:
        {
            emit RequestRemove(currentIndex());
        }
        break;
        case Qt::Key_Enter:
        {
            emit EnterPressed();
        }
        break;

        default: event->ignore();
    } // switch
    
    QSortFilterProxyModel *proxy =  qobject_cast<QSortFilterProxyModel*>(model());
    emit found(proxy->mapToSource(currentIndex()));

}

void QRadTableViewSearch::CalculateRow(int)
{
    int numRows = rowsPerViewport();

    if (numRows > model()->rowCount())
        numRows = model()->rowCount();

    m_averageRow = (numRows/2) + 1;
}

int QRadTableViewSearch::rowsPerViewport()
{
    int viewportHeight = this->viewport()->height();
    int rowHeight = this->rowHeight(0);

    if (rowHeight <= 0)
        rowHeight = 1;

    if(viewportHeight <= 0)
        viewportHeight = 1;

    return viewportHeight/rowHeight;
}

int QRadTableViewSearch::getAverageRow()
{
    return m_averageRow;
}

void QRadTableViewSearch::Search(QString string)
{
    debug_message("-->Search(%s)\n", string.toUtf8().data() );

    if( m_InUse )
    {
       debug_message("<--Search L:%d\n", __LINE__);
       return;
    }

    m_InUse = 1;

//    m_time->start();

    if (!model())
    {
        m_InUse = 0;
        debug_message("<--Search L:%d\n", __LINE__);
        return;
    }

    if (!currentIndex().isValid())
        selectRow(0);
    else
        selectRow(currentIndex().row());

    QCoreApplication *app = QCoreApplication::instance();
    m_useTableFilter = true; /*app->property("useTableFilter").toBool();*/
    m_SearchMode     = (QRad_SEARCH_MODE)app->property("SearchMode").toInt();

    debug_message("m_SearchMode:%d m_useTableFilter=%s L%d\n", m_SearchMode,
                  m_useTableFilter?"true":"false",
                  __LINE__);

    m_SuperMarket    = app->property("Supermarket").toBool();

    if( m_SearchMode != mMix)
      DoDefaultSearch(string);
    else
    {

        // aracaju special search
        // match exactly for code and codebar, contains for description.

        if (m_columnList.isEmpty())
        {
            qDebug() << "Warning: You forgot to set the columns to search text. Try to use addSearchColumn() before searching.";
            m_InUse = 0;
            debug_message("<--Search L:%d\n", __LINE__);
            return;
        }

        MixFilteredSearch( string );

//        for( int i = 0; i < 3; i++ )
//        {
//            if (searchAtColumn(i, i<2?Qt::MatchExactly:Qt::MatchContains , string))
//            {
//                QSortFilterProxyModel *proxy =  qobject_cast<QSortFilterProxyModel*>(model());
//                emit found(proxy->mapToSource(currentIndex()));
//                m_InUse = 0;
//                return;
//            }
////            else
//        }
//        emit notFound();
        m_InUse = 0;

        // not found, try description with contains

    }
    debug_message("<--Search L:%d\n", __LINE__);
}

void QRadTableViewSearch::DoDefaultSearch(QString string)
{
    debug_message("-->DoDefaultSearch L:%d\n", __LINE__);

    if (m_useTableFilter)
    {
        searchSortModel(string);
        m_InUse = 0;
    }
    else
    {
        if (m_columnList.isEmpty())
        {
            qDebug() << "Warning: You forgot to set the columns to search text. Try to use addSearchColumn() before searching.";
            m_InUse = 0;
            return;
        }

        QPair<int,Qt::MatchFlags> pair;

        foreach (pair, m_columnList)
        {
            if (searchAtColumn(pair.first, pair.second , string))
            {
                QSortFilterProxyModel *proxy =  qobject_cast<QSortFilterProxyModel*>(model());
                debug_message("-->emit found L:%d\n", __LINE__);
                emit found(proxy->mapToSource(currentIndex()));
                m_InUse = 0;
                return;
            }
            else
            {
                selectRow(0);
                emit notFound();
                debug_message("-->emit Notfound L:%d\n", __LINE__);
            }
                m_InUse = 0;
        }
    }
    debug_message("<--DoDefaultSearch L:%d\n", __LINE__);
}

bool QRadTableViewSearch::searchAtColumn(int column, Qt::MatchFlags flag , QString text)
{
    if (NULL == model())
    {
        qDebug() << "Model is Null.";
        return false;
    }

    if (0 == model()->rowCount())
        return false;


    if (!currentIndex().isValid())
        setCurrentIndex(model()->index(0, column));
    else
        setCurrentIndex(model()->index(currentIndex().row(), column));

    if(m_SearchMode == mContains )
        flag = Qt::MatchContains ;

    QModelIndex index = model()->index(currentIndex().row(),column);
    QModelIndexList match = model()->match(index,Qt::DisplayRole,text,1, flag | Qt::MatchWrap);


    if(match.count())
    {
        setCurrentIndex(match.first());
        return true;
    }

    return false;

}

void QRadTableViewSearch::showEvent(QShowEvent *event)
{
    debug_message("-->QRadTableViewSearch::showEvent()\n");
    if( !m_timerAfterShowEvent )
        m_timerAfterShowEvent = new QTimer;

    connect( m_timerAfterShowEvent, SIGNAL(timeout()), this, SLOT(AferShowSlot()));
    m_timerAfterShowEvent->setSingleShot(true);
    m_timerAfterShowEvent->setInterval(50);
    m_timerAfterShowEvent->start();

    setAcceptDrops(true);


    debug_message("<--QRadTableViewSearch::showEvent()\n");
}

int QRadTableViewSearch::getColumnOf(QString Title)
{
    int ColId = 0;
    for( ; ColId < this->horizontalHeader()->count(); ColId++ )
    {
        if(this->model()->headerData(ColId,Qt::Horizontal).toString().toUpper() == Title.toUpper())
        {
            qDebug() <<"QMetaType::Void:" << this->model()->headerData(ColId,Qt::Horizontal).type();
            break;
        }
    }
    return ColId;

}
void QRadTableViewSearch::AferShowSlot()
{
    debug_message("-->QRadTableViewSearch::AferShowSlot()\n");
    emit AfterShowEvent();
}

bool QRadTableViewSearch::searchSortModel(QString text, bool bSmart)
{
    QString regxp;
    QString strNewText;

    if( m_noEmptySearch )
    {
        if( text.trimmed().isEmpty() )
        {
            if(m_rowCount > 2000){
                emit found(currentIndex());
                return true;
            }

        }
    }
    if (m_searchColumnFilter.isEmpty())
    {
        qDebug() << "Warning: You forgot to set the columns to search text. Try to use addSearchColumnFilter() before searching.";
        return false;
    }
//     qDebug() << "a:" << m_time->elapsed();
    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(model());

    if (text.startsWith('*'))
        proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive,QRegExp::FixedString));
    else
    {
        //qDebug() << "b:" << m_time->elapsed();
        text.replace("+","\\+");
        //text.replace("\","\\");
        text.replace("[","\\[");
        text.replace("^","\\^");
        text.replace("$","\\$");
        text.replace(".","\\.");
        text.replace("|","\\|");
        text.replace("?","\\?");
        text.replace(")","\\)");
        text.replace("*","\\*");
        text.replace(")","\\)");
        text.replace(",","\\.");

        if ( ( m_SearchMode == mStartWith) || (text.toLongLong() && bSmart)){
             regxp = "^"+text;
        } else {
                regxp = "^"+text+"|"+text+"$"+"|"+text+"\\b"+"|"+text+".*" ;
        }

        //QRegExp rx("^"+text+"|"+text+"$"+"|"+text+"\\b"+"|"+text+".*",Qt::CaseInsensitive);
//        qDebug() << "b3:" << m_time->elapsed();

        QRegExp rx(regxp,Qt::CaseInsensitive);

       // qDebug() << "b4:" << m_time->elapsed();
        proxyModel->setFilterRegExp(rx);
        //proxyModel->setFilterRegExp(text);
    }

//    qDebug() << "c:" << m_time->elapsed();

    this->selectRow(1);



  //  qDebug() << "d:" << m_time->elapsed();
    m_strLastString = "";
    if( QToolTip::isVisible() )
        QToolTip::hideText();


    foreach( int i , m_searchColumnFilter )
    {        
        proxyModel->setFilterKeyColumn(i);

        if(proxyModel->rowCount() > 0)
        {
            this->selectRow(0);
            emit found(proxyModel->mapToSource(currentIndex()));

            if(text.toLongLong() && bSmart)
            {
                ShowToolTip();
                m_strLastString  = text;
            }
            //qDebug() << "found:" << m_time->elapsed();
            //break;//return;

            return true;
        }
        else
        {
            selectRow(0);
            emit notFound();
        }
    }

    if( !this->isVisible() && !text.toLongLong())
    {
        return false;
    }

    if( text.toLongLong() && m_SuperMarket && bSmart )
    {
        qDebug() << "Search cat 2..!";
        strNewText = "2" + text;
        regxp = "^"+strNewText;
        QRegExp rx(regxp,Qt::CaseInsensitive);
       // qDebug() << "b4:" << m_time->elapsed();
        proxyModel->setFilterRegExp(rx);

        foreach( int i , m_searchColumnFilter )
        {
            proxyModel->setFilterKeyColumn(i);

            if(proxyModel->rowCount() > 0)
            {
                this->selectRow(0);
                emit found(proxyModel->mapToSource(currentIndex()));

                ShowToolTip();
                m_strLastString  = text;

                return true;
            }
            else
            {
                selectRow(0);
                emit notFound();
            }
        }

    }

    if( text.toLongLong() ||  (m_SearchMode == mStartWith) )
    {
        qDebug() << "Expanding search options ...!!";
        regxp = "^"+text+"|"+text+"$"+"|"+text+"\\b"+"|"+text+".*" ;
        QRegExp rx(regxp,Qt::CaseInsensitive);
       // qDebug() << "b4:" << m_time->elapsed();
        proxyModel->setFilterRegExp(rx);

        foreach( int i , m_searchColumnFilter )
        {
            proxyModel->setFilterKeyColumn(i);

            if(proxyModel->rowCount() > 0)
            {
                this->selectRow(0);
                emit found(proxyModel->mapToSource(currentIndex()));
                //qDebug() << "found:" << m_time->elapsed();
                //break;//return;

                return true;
            }
            else
            {
                selectRow(0);
                emit notFound();
            }
        }
    }

    //qDebug() << "e:" << m_time->elapsed();

    return false;

}
void QRadTableViewSearch::getAlignment()
{
    int currenteRow = currentIndex().row();
    setCurrentIndex(model()->index(currenteRow,1));
    int index;

    if (currenteRow > (getAverageRow()-1) && currenteRow < (model()->rowCount() - (getAverageRow())))
    {
        this->selectRow(currenteRow + (getAverageRow()-1));
        index = currentIndex().row();
        this->selectRow(index -(getAverageRow()-1));

    }

}

void QRadTableViewSearch::VerifyEvent(QKeyEvent *event)
{
    this->keyPressEvent(event);
}

void QRadTableViewSearch::addSearchColumn(int column , Qt::MatchFlags flag)
{
    m_columnList.append(qMakePair(column,flag));
}

void QRadTableViewSearch::clearSearchList()
{
    m_columnList.clear();
}
void QRadTableViewSearch::clearSearchListFilter()
{
    m_searchColumnFilter.clear();
}

void QRadTableViewSearch::setModel( QAbstractItemModel *model)
{
    m_rowCount = model->rowCount();
    m_proxyModel->setSourceModel(model);
    QTableView::setModel(m_proxyModel);

}

void QRadTableViewSearch::addSearchColumnFilter(int column)
{
    m_searchColumnFilter.append(column);
}

void QRadTableViewSearch::SetNoEmptySearch( bool bValue )
{
    m_noEmptySearch =  bValue;
}


void QRadTableViewSearch::ShowToolTip()
{
    if( !this->isVisible() )
        return;

    QPoint point = mapToGlobal(QPoint());

    point.setX(point.x()+170);
    point.setY(point.y() - 70);

    QString strAddress = "F12 - Encontrar mais";
    QToolTip::showText(point,QString::fromUtf8("<font size='-1'>"\
                                               "<img src=:/png/infomation-icon.png> </font> <b> <font size= '2' face = 'verdana'> %1< /font> </b>"
                                               "<font size = '2' face='verdana'>"
                                               "<ul>"
                                               "<li> </li>"
                                               "</ul>"
                                               "</font>").arg(strAddress),this);



}


bool QRadTableViewSearch::MixFilteredSearch(QString text)
{
    QString regxp;

    if( m_noEmptySearch )
    {
        if( text.trimmed().isEmpty() )
        {
            if(m_rowCount > 2000){
                emit found(currentIndex());
                return true;
            }

        }
    }
    if (m_searchColumnFilter.isEmpty())
    {
        qDebug() << "Warning: You forgot to set the columns to search text. Try to use addSearchColumnFilter() before searching.";
        return false;
    }

    QSortFilterProxyModel *proxyModel = qobject_cast<QSortFilterProxyModel*>(model());

    if (text.startsWith('*'))
        proxyModel->setFilterRegExp(QRegExp(text, Qt::CaseInsensitive,QRegExp::FixedString));
    else
    {

        text.replace("+","\\+");
        //text.replace("\","\\");
        text.replace("[","\\[");
        text.replace("^","\\^");
        text.replace("$","\\$");
        text.replace(".","\\.");
        text.replace("|","\\|");
        text.replace("?","\\?");
        text.replace(")","\\)");
        text.replace("*","\\*");
        text.replace(")","\\)");
        text.replace(",","\\.");

        if(text.toLongLong() && (text.length() <10))  //{
            regxp = "^"+text+"$";
          else
             regxp = "^"+text+"|"+text+"$"+"|"+text+"\\b"+"|"+text+".*" ;
//        }

        //QRegExp rx("^"+text+"|"+text+"$"+"|"+text+"\\b"+"|"+text+".*",Qt::CaseInsensitive);
//        qDebug() << "b3:" << m_time->elapsed();


       // qDebug() << "b4:" << m_time->elapsed();
        //proxyModel->setFilterRegExp(text);
    }

//    QRegExp rxExact(regxp,Qt::CaseInsensitive,QRegExp::FixedString);
    QRegExp rx(regxp,Qt::CaseInsensitive, QRegExp::RegExp);
//    qDebug() << "c:" << m_time->elapsed();




    this->selectRow(1);



  //  qDebug() << "d:" << m_time->elapsed();
    m_strLastString = "";
    if( QToolTip::isVisible() )
        QToolTip::hideText();

    foreach( int i, m_searchColumnFilter )
    {
        proxyModel->setFilterRegExp(rx);

        proxyModel->setFilterKeyColumn(i);

        qDebug() << "column: "  << i;

        if(proxyModel->rowCount() > 0)
        {
            this->selectRow(0);

            if( text.toLongLong() && (currentIndex().sibling(0,i).data().toString() != text) )
            {
                qDebug() << "Encontrado [" << currentIndex().sibling(0,0).data().toString() <<"], porem não exatamente itual ... ";

                continue;
            }
            emit found(proxyModel->mapToSource(currentIndex()));
            return true;
        }
        else
        {
            selectRow(0);
            emit notFound();
        }
    }

    return false;
}


void QRadTableViewSearch::dropEvent(QDropEvent *event)
{
    QString strPath = event->mimeData()->text();
    emit OnDrop(strPath);
}

void QRadTableViewSearch::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
void QRadTableViewSearch::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}
