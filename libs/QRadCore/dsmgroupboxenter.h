#ifndef DSMGROUPBOXENTER_H
#define DSMGROUPBOXENTER_H

#include <QGroupBox>
#include <QEvent>
#include <QKeyEvent>

class DSMGroupBoxEnter : public QGroupBox
{
public:
    DSMGroupBoxEnter(QWidget *parent);
    void keyPressEvent(QKeyEvent * event);
};

#endif // DSMGROUPBOXENTER_H
