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
    connect(m_completer,SIGNAL(highlighted(QModelIndex)), this, SLOT(activated(QModelIndex)));
    connect(this,SIGNAL(returnPressed()), this, SLOT(returnPressed()));
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
            break;
        }
    }
}
int QRadLineEditCompleter::getCurrentId()
{
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
}

void QRadLineEditCompleter::textchanged(QString text)
{
    if( text.indexOf("|") > 0 )
    {
        text.truncate(text.indexOf("|"));
        this->setText(text.trimmed());
        this->selectAll();
    }
}
void QRadLineEditCompleter::activated(QModelIndex model)
{
    debug_message("-->\n");
//    if( m_lineEdits.count() >= 2)
    for( int i =0; i < m_lineEdits.count(); i++ )
    {
        m_lineEdits.at(i)->setText(model.sibling(model.row(), i+3).data().toString());
        debug_message("%s\n", model.sibling(model.row(), i+3).data().toString().toLatin1().data());
//        m_lineEdits.at(i)->setText(model.sibling(model.row(), 4).data().toString());
    }
    debug_message("<--\n");
}

void QRadLineEditCompleter::textEdited(QString text)
{
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
}

void QRadLineEditCompleter::returnPressed()
{
    debug_message("-->\n");

    if( m_lineEdits.count())
    {
        int i = 0;
        for( ; i < m_completerModel->rowCount(); i++ )
        {
            QString data = m_completerModel->index(i,2).data().toString();
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

    debug_message("<--\n");
}
