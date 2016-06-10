#include "qradlineeditcompleter.h"
#include <QAbstractItemView>
#include "qraddebug.h"

QRadLineEditCompleter::QRadLineEditCompleter(QWidget *parent):QLineEdit(parent)
{
    m_completerModel =  new QSqlQueryModel;
    m_completer = new QCompleter(this);

    connect(this,SIGNAL(textChanged(QString)), this, SLOT(textchanged(QString)));
    connect(this,SIGNAL(textEdited(QString)), this, SLOT(textEdited(QString)));
   connect(m_completer,SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
//    connect(m_completer,SIGNAL(highlighted(QModelIndex)), this, SLOT(activated(QModelIndex)));
   connect(m_completer->popup(),SIGNAL(clicked(QModelIndex)), this, SLOT(activated(QModelIndex)));
//    connect(m_completer->popup(),SIGNAL(entered(QModelIndex)), this, SLOT(activated(QModelIndex)));
//    connect(m_completer->popup(),SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
    connect(this,SIGNAL(returnPressed()), this, SLOT(returnPressed()));

//    connect(this,SIGNAL(returnPressed()), this, SLOT(returnPressed()));



    //connect(m_completer->popup(),SIGNAL((( QModelIndex)), this, SLOT(activated(QModelIndex)));

    m_nCurrentId = 0;
}

QRadLineEditCompleter::~QRadLineEditCompleter()
{
    delete m_completer;
    delete m_completerModel;
}

void QRadLineEditCompleter::Add(QLineEdit *pLineEdit)
{
    m_lineEdits.append(pLineEdit);
}

void QRadLineEditCompleter::setSelect(QString strSelect)
{
    m_completerModel->setQuery(strSelect);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_completer->setFilterMode(Qt::MatchContains );
    m_completer->setModel(m_completerModel);
    this->setCompleter(m_completer);
}

void QRadLineEditCompleter::setCurrentId(int id)
{
    for( int i = 0; i < m_completerModel->rowCount(); i++ )
    {
        if( m_completerModel->index(i,1).data().toInt() ==  id)
        {
            completer()->setCurrentRow(m_completerModel->index(i,1).row());
            m_nCurrentId = id;
            break;
        }
    }
}

void QRadLineEditCompleter::focusInEvent(QFocusEvent *e)
{
        debug_message("-->\n");
        disconnect(m_completer,SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
        disconnect(m_completer->popup(),SIGNAL(clicked(QModelIndex)), this, SLOT(activated(QModelIndex)));
        connect(m_completer,SIGNAL(activated(QModelIndex)), this, SLOT(activated(QModelIndex)));
        connect(m_completer->popup(),SIGNAL(clicked(QModelIndex)), this, SLOT(activated(QModelIndex)));


        QLineEdit::focusInEvent(e);
        debug_message("<--\n");

}
int QRadLineEditCompleter::getCurrentId()
{
    /*
    int id = -1;
    for( int i = 0; i < m_completerModel->rowCount(); i++ )
    {
        if( !m_completerModel->index(i,2).data().toString().trimmed().isEmpty() && (m_completerModel->index(i,2).data().toString() ==  this->text()))
        {
            id = m_completerModel->index(i,1).data().toInt();
            break;
        }
    }
    return id;
    */
    return m_nCurrentId;
}

void QRadLineEditCompleter::textchanged(QString text)
{
   // QString texto = text;
    debug_message("-->\n");
    if( text.indexOf("|") > 0 )
    {
        text.truncate(text.indexOf("|"));
        text = text.trimmed();
        this->setText(text);
        this->selectAll();
    }
   // activated(m_completer->currentIndex());

    debug_message("<--\n");
}
void QRadLineEditCompleter::activated(QModelIndex model)
{
    disconnect(this,SIGNAL(textChanged(QString)), this, SLOT(textchanged(QString)));

    debug_message("-->LineEditCount: %d\n", m_lineEdits.count());
//    if( m_lineEdits.count() >= 2)
    if( !model.isValid() )
        debug_message("MODEL INVALIDO ****\n");

    this->setText(model.sibling(model.row(), 2).data().toString());
    for( int i =0; i < m_lineEdits.count(); i++ )
    {
        m_lineEdits.at(i)->setText(model.sibling(model.row(), i+3).data().toString());
        debug_message("%s\n", model.sibling(model.row(), i+3).data().toString().toLatin1().data());
//        m_lineEdits.at(i)->setText(model.sibling(model.row(), 4).data().toString());        
    }
    m_nCurrentId = model.sibling(model.row(),1).data().toInt();
    emit found(m_nCurrentId);

    m_lineEdits.at(m_lineEdits.count()-1)->setFocus();

    connect(this,SIGNAL(textChanged(QString)), this, SLOT(textchanged(QString)));

    debug_message("<--\n");
}
/*
void QRadLineEditCompleter::keyPressEvent(QKeyEvent *event)
{

    switch( event->key() )
    {
        case Qt::Key_Escape:
                            ClearAll();
                            break;
    }
    QLineEdit::keyReleaseEvent(event);
}
*/
void QRadLineEditCompleter::ClearAll()
{
    for( int i =0 ; i < m_lineEdits.count(); i++)
    {
        m_lineEdits.at(i)->clear();
    }
}

void QRadLineEditCompleter::textEdited(QString text)
{
    debug_message("-->\n");
    ClearAll();
    /*if( text.indexOf("|") > 0 )
    {
        text.truncate(text.indexOf("|"));
        text = text.trimmed();
        this->setText(text);
        this->selectAll();
    }
    */

   // ClearAll();
   // m_nCurrentId = 0;
   // emit notFound();
    /*
    debug_message("-->\n");
    if( m_lineEdits.count() )
    {
        int i = 0;
        for( ; i < m_completerModel->rowCount(); i++ )
        {
            if( m_completerModel->index(i,2).data().toString() == text )
            {
                int j = 0;
                for( ; j < m_lineEdits.count(); j++ )
                {
                    m_lineEdits.at(j)->setText(m_completerModel->index(i,j+3).data().toString());
    //                m_lineEdits.at(1)->setText(m_completerModel->index(i,4).data().toString());
    //                m_lineEdits.at(1)->setFocus();
                }
                m_lineEdits.at(j-1)->setFocus();
                break;
            }
        }
        if( i == m_completerModel->rowCount()) // not found, clear
        {
            for( i = 0; i < m_lineEdits.count(); i++ )
                m_lineEdits.at(i)->setText("");
        }
    }
    debug_message("<--\n");
    */
    debug_message("<--\n");
}

void QRadLineEditCompleter::returnPressed()
{
    debug_message("-->\n");

#if(0)
    bool bFound = false;
    int i = 0;

    if( m_lineEdits.count())
    {
        for( ; !bFound && (i < m_completerModel->rowCount()); i++ )
        {
            for( int j = 0; !bFound && (j < m_lineEdits.count()); j++ )
            {
                QString data  = m_completerModel->index(i,2).data().toString();
//                QString Text   = this->text();
                QString lnText = m_lineEdits.at(j)->text();
                if( !data.trimmed().isEmpty() && (data == this->text()))
                {
                    bFound = true;
                   // i = m_completerModel->rowCount();
                    break;
                }
                else if( !lnText.isEmpty() && (lnText == m_completerModel->index(i,j+3).data().toString() ))
                {
                    bFound = true;
                  //  i = m_completerModel->rowCount();
                    break;
                }
            }
        }
    }
    if( !bFound )
    {
        this->focusNextChild();
        emit notFound();
    }
    else
    {
        m_lineEdits.at(m_lineEdits.count()-1)->setFocus();
        m_nCurrentId = m_completerModel->index(i,1).data().toInt();
        emit found(m_nCurrentId);
    }
/*            QString data = m_completerModel->index(i,2).data().toString();
            debug_message("data:%s, this->text():%s\n", data.toLatin1().data(), this->text().toLatin1().data());
            if( !data.trimmed().isEmpty() && (data == this->text()) )
            {
                debug_message("data:%s\n", data.toLatin1().data());
                int j = 0;
                for( ; j < m_lineEdits.count(); j++ )
                {
                    debug_message("for...:%s\n", m_completerModel->index(i,j+3).data().toString().toLatin1().data());
                    m_lineEdits.at(j)->setText(m_completerModel->index(i,j+3).data().toString());
                  //  m_lineEdits.at(1)->setText(m_completerModel->index(i,4).data().toString());
                }

                m_lineEdits.at(j-1)->setFocus();
                emit found(m_completerModel->index(i,1).data().toInt());

                break;
            }
        }
        if( i == m_completerModel->rowCount()) // not found, but enter, go to next..
        {
            m_lineEdits.at(0)->setFocus();
        }
    }else
        this->focusNextChild();
*/
#endif
    debug_message("<--\n");
}
