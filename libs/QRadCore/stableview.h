#ifndef STABLEVIEW_H
#define STABLEVIEW_H

#include <QTableView>
#include <QSqlTableModel>
#include "column2delegate.h"

class STableView : public QTableView
{
    Q_OBJECT
	
public:
    STableView(QWidget *parent);
    virtual ~STableView();
	
    void setModel( QSqlTableModel  * model );

private:

     ColumnSpin *m_clSpn;
     ColumnCombo *m_clCmb;
	

};

#endif // STABLEVIEW_H
