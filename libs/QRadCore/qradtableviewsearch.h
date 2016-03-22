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

signals:
    void found(QModelIndex index);
    void notFound();
    void CurrentChanged(QModelIndex index);
    void RequestAppend();
    void RequestRemove(QModelIndex index);
    void AfterShowEvent();
    void OnDrop(QString);
    void EnterPressed();

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
    void ShowToolTip();
    void DoDefaultSearch(QString string);
    bool MixFilteredSearch(QString text);

};

#endif // QRadTABLEVIEWSEARCH_H
