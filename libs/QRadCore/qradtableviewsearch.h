#ifndef QRadTABLEVIEWSEARCH_H
#define QRadTABLEVIEWSEARCH_H

#include <QTableView>
#include <QString>
#include <QKeyEvent>
#include <QObject>
#include <QList>
#include <QPair>
#include <QFlags>
#include <QMap>
#include <QSortFilterProxyModel>
#include <QRegExp>
#include <QTreeView>
#include <QSqlQueryModel>
#include <QAbstractItemModel>
#include <QItemSelectionModel>
#include <QTime>
#include <QTimer>
#include <QShowEvent>
#include <QLineEdit>
#include "model.h"

typedef enum e_mode
{
    mContains,
    mStartWith,
    mMix
}QRad_SEARCH_MODE, *PQRad_SEARCH_MODE;

class QRadTableViewSearch : public QTableView
{
    Q_OBJECT

public:
    QRadTableViewSearch(QWidget *parent);
    virtual ~QRadTableViewSearch();

    void keyPressEvent(QKeyEvent * event);
    void addSearchColumn(int column , Qt::MatchFlags flag = Qt::MatchStartsWith);
    void addSearchColumnFilter(int column = 1);
    void clearSearchList();
    void clearSearchListFilter();
    void setModel( QAbstractItemModel * model );
    bool searchSortModel(QString text, bool bSmart = true);
    int getAverageRow();
    void CalculateRow(int);
    void SetNoEmptySearch( bool bValue );
    bool searchAtColumn(int column , Qt::MatchFlags flag ,QString text);
    void showEvent(QShowEvent *event);
    int getColumnOf(QString Title );
    void setSearchEdit(QLineEdit *pSearchEdit, int nDefaultColumnSearch  );
    void refreshTable();
    void MatchNewest( Model *newest );

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
protected:
    void getAlignment();
    int rowsPerViewport();
    int m_averageRow , m_rowCount ;
    QList<int>  m_searchColumnFilter;
    QList< QPair<int,Qt::MatchFlags> > m_columnList;

public slots:
    void Search(QString);
    void VerifyEvent(QKeyEvent *event);
    void AferShowSlot();

    void StartTimer(QString);
    void Found(QModelIndex);
    void OnNotFound();
    void OnCurrentChanged(QModelIndex currentIndex);
    void KeyPressTimeout();
    void TblColumnResized(int logicalIndex, int oldSize, int newSize);
signals:
    void found(QModelIndex index);
    void notFound();
    void CurrentChanged(QModelIndex index);
    void RequestAppend();
    void RequestRemove(QModelIndex index);
    void AfterShowEvent();
    void OnDrop(QString);
    void EnterPressed();
    void ShowCurrentInformations( void );
    void OnLoadTableView(void);

private:
    QSortFilterProxyModel *m_proxyModel;
    bool m_useTableFilter;
    QRad_SEARCH_MODE  m_SearchMode;
    bool m_SuperMarket;
    bool m_InUse;
    QTime *m_time;
    QTimer *m_timerAfterShowEvent;
    bool m_noEmptySearch;
    QString m_strLastString;
    QLineEdit *m_pSearchEdit;
    QTimer *m_keyinterval;
    void ShowToolTip();
    void DoDefaultSearch(QString string);
    bool MixFilteredSearch(QString text);
    int m_nDefaultColumnSearch;

    QString BuildClmnTableName();
};

#endif // QRadTABLEVIEWSEARCH_H
