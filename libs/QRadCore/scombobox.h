#ifndef SCOMBOBOX_H
#define SCOMBOBOX_H

#include <QComboBox>
//#include <QString>
#include <QSqlQueryModel>
#include "qraddebug.h"
#include "qradconfig.h"

class SComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(QString Table READ Table WRITE setTable NOTIFY TableChanged)
    Q_PROPERTY(QString Field READ Field WRITE setField NOTIFY FieldChanged)
    Q_PROPERTY(QString DatabaseName READ DatabaseName WRITE setDatabaseName)
    Q_PROPERTY(QString UserName READ UserName WRITE setUserName)
    Q_PROPERTY(QString Password READ Password WRITE setPassword)
    Q_PROPERTY(QString Host READ Host WRITE setHost)
    Q_PROPERTY(QString Permission READ Host WRITE setPermission)
    Q_PROPERTY(bool Build READ Build WRITE setBuild NOTIFY BuildChanged)
    Q_PROPERTY(bool CanAdd READ CanAdd WRITE setCanAdd)
    Q_PROPERTY(int  CurentId READ CurrentId WRITE setCurrentId)

public:
    SComboBox(QWidget *parent = 0);
    ~SComboBox();

    void setTable(QString text )
    {
        debug_message("-->setTable\n");
        m_Table =  text;
        debug_message("<--setTable\n");
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
    void setCurrentId( int id )
    {
        m_CurrentId = id;
    }
    void setPermission(QString Permission )
    {
        m_Permission = Permission;
    }
    void SetEditor(QDialog *pEditor);


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

        if( m_Permission.isEmpty() || QRadConfig::GetUserProfile(m_Permission))
            return m_CanAdd;
        else
            return false;
    }
    int CurrentId()
    {
        return m_CurrentId;
    }
    QString GetPermission()
    {
        return m_Permission;
    }

public slots:
     void NovoActivacted(QString Text);
     void doEdit(void);
     void doUpdate(QString New);
signals:
    void TableChanged(QString);
    void FieldChanged(QString);
    void BuildChanged(bool);
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
  int m_CurrentId;
  QString m_Permission;

  QString m_UIFileName;
  QSqlDatabase m_currentdb;
  QDialog *m_pEditor;


  void showEvent ( QShowEvent * event );
  QString CreateEditUi();
  bool ConnectDB();
  bool PersistObjects();
  void CreateTableIfNoExist();
  QString TableName();
  QString TableCaption();
  QString FieldName();
  QString FieldCaption();
  QString TrueToken();
};

#endif
