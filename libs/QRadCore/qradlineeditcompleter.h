#ifndef QRADLINEEDITCOMPLETER_H
#define QRADLINEEDITCOMPLETER_H

#include <QLineEdit>
#include <QSqlQueryModel>
#include <QCompleter>
#include <QKeyEvent>
#include <QFocusEvent>
 //#include <QKeyEvent>

class QRadLineEditCompleter : public QLineEdit
{
    Q_OBJECT
public:
    QRadLineEditCompleter(QWidget *parent = 0);
    ~QRadLineEditCompleter();

    void Add(QLineEdit *pLineEdit );
    void setSelect(QString strSelect );
    int getCurrentId();

    void setCurrentId(int id);
    void focusInEvent(QFocusEvent *e);
  //  void keyPressEvent(QKeyEvent *event);
public slots:
    void textchanged(QString text);
    void activated(QModelIndex model);
    void textEdited(QString text);

    void returnPressed();

signals:

    void found( int id );
    void notFound();


private:
    QList<QLineEdit *> m_lineEdits;
    QCompleter     *m_completer;
    QSqlQueryModel *m_completerModel;
    int m_nCurrentId = 0;

    void ClearAll();

};

#endif // QRADLINEEDITCOMPLETER_H
