#ifndef DSMTEXTEDIT_H
#define DSMTEXTEDIT_H

#include <QtCore>
#include <QTextEdit>
#include <QKeyEvent>

class DSMTextEdit : public QTextEdit
{
    Q_OBJECT;
public:
    DSMTextEdit( QWidget* parent = 0 );

protected:
    bool event(QEvent*);

signals:
    void tabPressed( void );
};

#endif // DSMTEXTEDIT_H
