#ifndef QRadListView_H
#define QRadListView_H

#include <QListView>
#include <QDropEvent>
#include <QSqlQueryModel>

class QRadListView : public QListView
{
    Q_OBJECT
    Q_PROPERTY(QString Table READ Table WRITE setTable NOTIFY TableChanged)
    Q_PROPERTY(QString Field READ Field WRITE setField NOTIFY FieldChanged)
    Q_PROPERTY(QString DatabaseName READ DatabaseName WRITE setDatabaseName)
    Q_PROPERTY(QString UserName READ UserName WRITE setUserName)
    Q_PROPERTY(QString Password READ Password WRITE setPassword)
    Q_PROPERTY(QString Host READ Host WRITE setHost)
    Q_PROPERTY(bool CanAdd READ CanAdd WRITE setCanAdd)
    Q_PROPERTY(int  ForeingKey READ ForeingKey WRITE setForeingKey)
    Q_PROPERTY(bool  FileSelect READ ForeingKey WRITE SetFileSelect)

public:
    QRadListView(QWidget *parent = 0);
    ~QRadListView();

    void setTable(QString text )
    {
        m_Table =  text;
    }
    void setField(QString text )
    {
        m_Field =  text;
    }
    void setBuild(bool build )
    {
        m_Build = build;
    }
    void setHost(QString Host )
    {
        m_Host = Host;
    }
    void setDatabaseName(QString DatabaseName )
    {
        m_DatabaseName = DatabaseName;
    }
    void setUserName(QString UserName )
    {
        m_UserName = UserName;
    }
    void setPassword(QString Password )
    {
        m_Password = Password;
    }
    void setCanAdd(bool build )
    {
        m_CanAdd = build;
    }
    void setForeingKey( int id )
    {
        m_ForeingKey = id;
    }

    void SetFileSelect( bool FileSelect)
    {
        m_IsFileSelect = FileSelect;
    }


    QString Table()
    {
        return m_Table;
    }
    QString Field()
    {
        return m_Field;
    }
    bool Build()
    {
        return m_Build;
    }
    QString Host()
    {
        return m_Host;
    }
    QString DatabaseName()
    {
        return m_DatabaseName;
    }
    QString UserName()
    {
        return m_UserName;
    }
    QString Password()
    {
        return m_Password;
    }
    bool CanAdd()
    {
        return m_CanAdd;
    }
    int ForeingKey()
    {
        return m_ForeingKey;
    }

    bool IsFileSelect()
    {
        return m_IsFileSelect;
    }

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
	bool Save(int fkey );
    void Load(int fkey );

    void keyPressEvent(QKeyEvent *event);
public slots:
    void Activacted(QModelIndex index);
    void doEdit(void);
    void doUpdate(QString New);
signals:
    void TableChanged(QString);
    void FieldChanged(QString);
    void OnEdit(void);
    void OnUpdate(QString New);


private:
  QString m_Table;
  QString m_Field;
  bool    m_Build;
  QString m_Host;
  QString m_DatabaseName;
  QString m_UserName;
  QString m_Password;
  QSqlQueryModel *m_pModelLocal;
  bool   m_CanAdd;
  int m_ForeingKey;
  bool m_IsFileSelect;

  QString m_UIFileName;
  QSqlDatabase m_currentdb;


  void showEvent ( QShowEvent * event );
  QString CreateEditUi();
  void ConnectDB();
  bool PersistObjects();
  void CreateTableIfNoExist();
};

#endif
