#ifndef QRadDATEEDITFAKEENTER_H
#define QRadDATEEDITFAKEENTER_H

#include <QDateEdit>
#include <QKeyEvent>
#include "qradshared.h"

class QRadDateEditFakeEnter : public QDateEdit
{
public:
    QRadDateEditFakeEnter(QWidget *parent);
};

#endif // QRadDATEEDITFAKEENTER_H
