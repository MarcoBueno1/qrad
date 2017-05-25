#ifndef DSMCOMBOBOXENTER_H
#define DSMCOMBOBOXENTER_H

#include <QComboBox>
#include <QEvent>
#include <QString>
#include <QKeyEvent>

class DSMComboBoxEnter : public QComboBox
{
public:
    DSMComboBoxEnter(QWidget *parent);
    void keyPressEvent ( QKeyEvent * event );
};

#endif // DSMCOMBOBOXENTER_H
