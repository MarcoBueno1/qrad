#ifndef DSMRADIOBUTTONENTER_H
#define DSMRADIOBUTTONENTER_H

#include <QRadioButton>
#include <QEvent>
#include <QKeyEvent>

class DSMRadioButtonEnter : public QRadioButton
{
public:
    DSMRadioButtonEnter(QWidget *parent);
    void keyPressEvent(QKeyEvent * event);
};

#endif // DSMRADIOBUTTONENTER_H
