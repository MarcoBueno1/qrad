#include "dsmtextedit.h"

DSMTextEdit::DSMTextEdit( QWidget* parent )
    : QTextEdit( parent )
{
}


bool DSMTextEdit::event( QEvent* e )
{
    if ( e->type() == QEvent::KeyPress )
    {
        switch ( ((QKeyEvent*)e)->key() )
        {
            case Qt::Key_Tab:   // continue
            case Qt::Key_Return:
            case Qt::Key_Enter:
            {
                emit tabPressed();
                return true;
            }
            break;
        }//switch
    }

    return QTextEdit::event( e );
}
