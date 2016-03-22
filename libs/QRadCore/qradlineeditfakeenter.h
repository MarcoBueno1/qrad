#ifndef QRadLINEEDITFAKEENTER_H
#define QRadLINEEDITFAKEENTER_H

#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include "qradshared.h"

class QRadLineEditFakeEnter : public QLineEdit
{
public:
    QRadLineEditFakeEnter(QWidget *parent);

    void clear();
};

#endif // QRadLINEEDITFAKEENTER_H
