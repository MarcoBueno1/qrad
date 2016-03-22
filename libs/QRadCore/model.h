#ifndef MODEL_H
#define MODEL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlResult>
#include <QTime>
#include <QDate>
#include <QString>
#include <QDebug>

#include "qradround.h"

typedef enum ModelAttrType_en
{
    ATTR_QString    = 0x01,
    ATTR_int        = 0x02,
    ATTR_double     = 0x03,
    ATTR_bool       = 0x04,
    ATTR_QDate      = 0x05,
    ATTR_QTime      = 0x06,
    ATTR_QChar      = 0x07,
    ATTR_char       = 0x08,
    ATTR_Serial     = 0x09,
}ModelAttrType;

#define _convert_QString(obj)  obj.toString()
#define _convert_int(obj)      obj.toInt()
#define _convert_double(obj)   obj.toDouble()
#define _convert_bool(obj)     obj.toBool()
#define _convert_QDate(obj)    obj.toDate()
#define _convert_QTime(obj)    obj.toTime()
#define _convert_QChar(obj)    obj.toString().at(0)

#define _toString_QString(obj)  obj
#define _toString_int(obj)      QString::number(obj)
#define _toString_double(obj)   QString::number(obj)
#define _toString_bool(obj)     (true == obj) ? "true" : "false"
#define _toString_QDate(obj)    obj.toString("yyyy-MM-dd")
#define _toString_QTime(obj)    obj.toString("hh:mm:ss")
#define _toString_QChar(obj)    QString("%1").arg(obj)

#define MODEL_INITIALIZE(className) delete new className();

#define ATTRIBUTE_CLASS(attrType)\
    class ModelAttribute_##attrType : public ModelAttribute \
    {\
        public: \
            ModelAttrType type() { return ATTR_##attrType; }\
            attrType value() { return _convert_##attrType(m_value); }\
            virtual QString description() { return #attrType; } \
            virtual QString toString() { return _toString_##attrType(value()); }\
    }

class ModelAttribute
{
    public:
        QString fieldName() { return m_fieldName; }
        void setFieldName(QString fieldName) { m_fieldName = fieldName; }
        void setValue(QVariant value) { m_value = value; }
        QVariant value() { return m_value; }

        virtual ModelAttrType type() = 0;
        virtual QString description() = 0;
        virtual QString toString() = 0;
    protected:
        QVariant m_value;
        QString m_fieldName;
        QString m_description;
};

ATTRIBUTE_CLASS(QString);
ATTRIBUTE_CLASS(QDate);
ATTRIBUTE_CLASS(QTime);
ATTRIBUTE_CLASS(QChar);
ATTRIBUTE_CLASS(int);
ATTRIBUTE_CLASS(double);
ATTRIBUTE_CLASS(bool);

#define MODEL_DECLARE_ATTRIBUTE(attrType, attr)\
    protected:\
        ModelAttribute_##attrType m_##attr;\
    public: \
        attrType get##attr(void) { return m_##attr.value(); }\
        void set##attr(attrType value) { m_##attr.setValue(value); }\
        bool update##attr(attrType value) \
        { \
          m_##attr.setValue(value);\
          QString str = QString("update %1 set %2 = \'%3\' where %4 = \'%5\';") \
                        .arg(tableName()) \
                        .arg(fieldName(#attr)) \
                        .arg(m_##attr.toString().replace("\'", "\'\'"))\
                        .arg(m_primaryKey->fieldName()) \
                        .arg(m_primaryKey->value().toString());\
        QSqlQuery *query;\
        if(isCustomDatabase()) \
        {\
            query = new QSqlQuery(getDatabase());\
        }\
        else\
        {\
            query = new QSqlQuery();\
        }\
        int bError = true;\
        if (!query->exec(str)) \
            bError= false;\
         delete query;\
         return bError; \
        }



#define MODEL_DECLARE_POSITIVE_ATTRIBUTE(attrType, attr)\
    protected:\
        ModelAttribute_##attrType m_##attr;\
    public: \
        attrType get##attr(void) { return m_##attr.value(); }\
        void set##attr(attrType value) { m_##attr.setValue(value); }\
        bool update##attr(attrType value) \
        { \
          m_##attr.setValue(value);\
          QString str = QString("update %1 set %2 = case when %3 >= 0 then %3 else 0 end where %4 = \'%5\';") \
                        .arg(tableName()) \
                        .arg(fieldName(#attr)) \
                        .arg(m_##attr.toString().replace("\'", "\'\'"))\
                        .arg(m_primaryKey->fieldName()) \
                        .arg(m_primaryKey->value().toString());\
            QSqlQuery *query;\
            if(isCustomDatabase()) \
            {\
                query = new QSqlQuery(getDatabase());\
            }\
            else\
            {\
                query = new QSqlQuery();\
            }\
            int bError = true;\
            if (!query->exec(str)) \
                bError= false;\
             delete query;\
             return bError; \
        }

#define MODEL_DECLARE_STRING_ATTRIBUTE(attr, _size)\
    protected:\
        ModelAttribute_QString m_##attr;\
    public: \
        QString get##attr(void) { return m_##attr.value(); }\
        void set##attr(QString value) { m_##attr.setValue(value.mid(0, _size)); }\
        bool update##attr(QString value) \
        { \
          m_##attr.setValue(value);\
          QString str = QString("update %1 set %2 = \'%3\' where %4 = \'%5\';") \
                        .arg(tableName()) \
                        .arg(fieldName(#attr)) \
                        .arg(m_##attr.toString().replace("\'", "\'\'"))\
                        .arg(m_primaryKey->fieldName()) \
                        .arg(m_primaryKey->value().toString());\
            QSqlQuery *query;\
            if(isCustomDatabase()) \
            {\
                query = new QSqlQuery(getDatabase());\
            }\
            else\
            {\
                query = new QSqlQuery();\
            }\
            int bError = true;\
            if (!query->exec(str)) \
                bError= false;\
             delete query;\
             return bError; \
        }

#define MODEL_DECLARE_DOUBLE_ATTRIBUTE(attr)\
    protected:\
        ModelAttribute_double m_##attr;\
    public: \
        double get##attr(bool bRound = true) { return bRound?QRadRound::PowerRound(m_##attr.value()):m_##attr.value(); }\
        void set##attr(double value, bool bRound = true) { m_##attr.setValue(bRound?QRadRound::PowerRound(value):value); }\
        bool update##attr(double value, bool bRound = true) \
        { \
          m_##attr.setValue(bRound?QRadRound::PowerRound(value):value);\
          QString str = QString("update %1 set %2 = \'%3\' where %4 = \'%5\';") \
                        .arg(tableName()) \
                        .arg(fieldName(#attr)) \
                        .arg(m_##attr.toString().replace("\'", "\'\'"))\
                        .arg(m_primaryKey->fieldName()) \
                        .arg(m_primaryKey->value().toString());\
            QSqlQuery *query;\
            if(isCustomDatabase()) \
            {\
                query = new QSqlQuery(getDatabase());\
            }\
            else\
            {\
                query = new QSqlQuery();\
            }\
            int bError = true;\
            if (!query->exec(str)) \
                bError= false;\
             delete query;\
             return bError; \
        }

#define MODEL_DECLARE_DOUBLE_3_DECIMAL_ATTRIBUTE(attr)\
    protected:\
        ModelAttribute_double m_##attr;\
    public: \
        double get##attr(void) { return QRadRound::PowerRoundAmount(m_##attr.value()); }\
        void set##attr(double value) { m_##attr.setValue(QRadRound::PowerRoundAmount(value)); }\
        bool update##attr(double value) \
        { \
          m_##attr.setValue(QRadRound::PowerRoundAmount(value));\
          QString str = QString("update %1 set %2 = \'%3\' where %4 = \'%5\';") \
                        .arg(tableName()) \
                        .arg(fieldName(#attr)) \
                        .arg(m_##attr.toString().replace("\'", "\'\'"))\
                        .arg(m_primaryKey->fieldName()) \
                        .arg(m_primaryKey->value().toString());\
            QSqlQuery *query;\
            if(isCustomDatabase()) \
            {\
                query = new QSqlQuery(getDatabase());\
            }\
            else\
            {\
                query = new QSqlQuery();\
            }\
            int bError = true;\
            if (!query->exec(str)) \
                bError= false;\
             delete query;\
             return bError; \
        }

#define MODEL_DECLARE_DOUBLE_POSITIVE_ATTRIBUTE(attr)\
    protected:\
        ModelAttribute_double m_##attr;\
    public: \
        double get##attr(void) { return QRadRound::PowerRound(m_##attr.value()); }\
        void set##attr(double value) { m_##attr.setValue(QRadRound::PowerRound(value)); }\
        bool update##attr(double value) \
        { \
          m_##attr.setValue(QRadRound::PowerRound(value));\
          QString str = QString("update %1 set %2 = case when %3 >= 0 then %3 else 0 end where %4 = \'%5\';") \
                        .arg(tableName()) \
                        .arg(fieldName(#attr)) \
                        .arg(m_##attr.toString().replace("\'", "\'\'"))\
                        .arg(m_primaryKey->fieldName()) \
                        .arg(m_primaryKey->value().toString());\
            QSqlQuery *query;\
            if(isCustomDatabase()) \
            {\
                query = new QSqlQuery(getDatabase());\
            }\
            else\
            {\
                query = new QSqlQuery();\
            }\
            int bError = true;\
            if (!query->exec(str)) \
                bError= false;\
             delete query;\
             return bError; \
        }


#define MODEL_DECLARE_FOREINGKEY(attrType, attrId, obj, objType) \
        protected:\
            ModelAttribute_##attrType m_##attrId; \
            objType m_##obj; \
        public: \
            attrType get##attrId(void) { return m_##attrId.value(); } \
            void set##attrId(attrType value) { m_##attrId.setValue(value); m_##obj.fillModelByPrimaryKey(value);} \
            objType* get##obj() { set##attrId(m_##attrId.value()); return &m_##obj; } \
            void set##obj(objType* _obj) { m_##obj.copyFrom(_obj); } \
            bool update##attrId(attrType value) \
            { \
              m_##attrId.setValue(value);\
              QString str = QString("update %1 set %2 = \'%3\' where %4 = \'%5\';") \
                            .arg(tableName()) \
                            .arg(fieldName(#attrId)) \
                            .arg(m_##attrId.toString())\
                            .arg(m_primaryKey->fieldName()) \
                            .arg(m_primaryKey->value().toString());\
                QSqlQuery *query;\
                if(isCustomDatabase()) \
                {\
                    query = new QSqlQuery(getDatabase());\
                }\
                else\
                {\
                    query = new QSqlQuery();\
                }\
                int bError = true;\
                if (!query->exec(str)) \
                    bError= false;\
                 delete query;\
                 return bError; \
            }

#define MODEL_DEFAULT_CONSTRUCTOR(className, _tableName) \
        public: \
            className() : Model()\
            {\
                mapFields(); \
                m_tableName = _tableName; \
                m_usecustodatabase = false; \
            } \
            className(QVariant primaryKeyValue) : Model()\
            {\
                mapFields(); \
                m_tableName = _tableName; \
                m_primaryKey->setValue(primaryKeyValue);\
                m_usecustodatabase = false; \
            }\
            className(QString databaseName) : Model()\
            {\
                mapFields(); \
                m_tableName = _tableName; \
                if (QSqlDatabase::contains(databaseName)) \
                { \
                    m_database = QSqlDatabase::database(databaseName); \
                } \
                m_usecustodatabase = true; \
            } \
            static QSqlDatabase database() { return m_database; } \
            static bool useCustomDatabase() { return m_usecustodatabase; /*return false;*/ } \
            QSqlDatabase getDatabase() { return className::database(); } \
            bool isCustomDatabase() { return className::useCustomDatabase(); }

#define MODEL_CONSTRUCTOR_WITH_DATABASE(_className, _tableName, databaseName) \
        public: \
            _className() : Model()\
            {\
                mapFields(); \
                m_tableName = _tableName; \
                if (QSqlDatabase::contains(databaseName)) \
                { \
                    m_database = QSqlDatabase::database(databaseName); \
                } \
            } \
            _className(QVariant primaryKeyValue) : Model()\
            {\
                mapFields(); \
                m_tableName = _tableName; \
                m_primaryKey->setValue(primaryKeyValue);\
                if (QSqlDatabase::contains(databaseName)) \
                { \
                    m_database = QSqlDatabase::database(databaseName); \
                } \
            } \
            _className(QString dbasea) : Model()\
            {\
                mapFields(); \
                m_tableName = _tableName; \
                if (QSqlDatabase::contains(dbasea)) \
                { \
                    m_database = QSqlDatabase::database(dbasea); \
                    m_usecustodatabase = true; \
                } \
            } \
        static QSqlDatabase database() { return m_database; } \
        static bool useCustomDatabase() { return true; } \
        bool isCustomDatabase() { return _className::useCustomDatabase(); } \
        QSqlDatabase getDatabase() { return _className::database(); }
            

#define MODEL_SETQUERY(className, sqlquerymodel, query) \
            if (useCustomDatabase()) \
                sqlquerymodel->setQuery(QSqlQuery(query, database())); \
            else \
                sqlquerymodel->setQuery(query);

#define MODEL_SETQUERY_DATABASE(className, sqlquerymodel, query, database) \
                sqlquerymodel->setQuery(QSqlQuery(query, database));

#define MODEL_STATIC_METHODS(_className, _tableName) \
        protected: QString className() { return QString::fromUtf8(#_className); } \
        public: static QSqlDatabase m_database; \
        public: static bool m_usecustodatabase; \
        public: static QString tableName() { return _tableName; }\
        public: static QString fieldName(QString attrName) { return m_fieldMap[attrName]; }\
        public: static bool fillModelList(_className##List* _lst, QString query) {\
            QSqlQueryModel *model = new QSqlQueryModel();\
            MODEL_SETQUERY(_className, model, query) \
            if (model->rowCount() <= 0) { delete model; return false; }\
            for (int i=0; i < model->rowCount(); i++) {\
                _className* obj = new _className();\
                obj->fillModel(model->record(i));\
                _lst->append(obj);\
            } delete model;\
            return true;\
        }\
    public: static bool fillModelListDB(_className##List* _lst, QString query, QSqlDatabase database) {\
        QSqlQueryModel *model = new QSqlQueryModel();\
        MODEL_SETQUERY_DATABASE(_className, model, query, database) \
        if (model->rowCount() <= 0) { delete model; return false; }\
        for (int i=0; i < model->rowCount(); i++) {\
            _className* obj = new _className();\
            obj->fillModel(model->record(i));\
            _lst->append(obj);\
        } delete model;\
        return true;\
    }\
        public: static bool deleteAll() { \
                MODEL_INITIALIZE(_className); \
                QSqlQuery *query; \
                if (useCustomDatabase())\
                {\
                    query = new QSqlQuery(database());\
                }\
                else\
                {\
                    query = new QSqlQuery();\
                }\
                bool ret = query->exec(QString("delete from %1").arg(_tableName));\
                delete query; \
                return ret; \
        }\
        public: bool fillModelFromQuery(QString query) {\
            QSqlQueryModel *model = new QSqlQueryModel();\
            MODEL_SETQUERY(_className, model, query) \
            if (model->rowCount() <= 0) { delete model; return false; }\
            fillModel(model->record(0));\
            delete model;\
            return true;\
        }\
    public: bool fillModelFromQuery(QString query, QSqlDatabase database) {\
        QSqlQueryModel *model = new QSqlQueryModel();\
        MODEL_SETQUERY_DATABASE(_className, model, query,database) \
        if (model->rowCount() <= 0) { delete model; return false; }\
        fillModel(model->record(0));\
        delete model;\
        return true;\
    }\
        public: bool fillModelByPrimaryKey(QVariant primaryKey)\
        {\
            QString query = QString("select * from %1 where %2 = %3")\
                            .arg(m_tableName)\
                            .arg(m_primaryKey->fieldName())\
                            .arg(primaryKey.toString());\
            QSqlQueryModel *model = new QSqlQueryModel();\
            MODEL_SETQUERY(_className, model, query) \
            if (model->rowCount() <= 0) { delete model; return false; }\
            fillModel(model->record(0));\
            delete model;\
            return true;\
        }\
        public: static _className* findByAttribute(QString attrName, QVariant value)\
        {\
            _className *obj = new _className();\
            QString query = QString("select * from %1 where %2 = %3")\
                      .arg(_className::tableName())\
                      .arg(_className::fieldName(attrName))\
                      .arg(value.toString());\
            if (!obj->fillModelFromQuery(query)) { delete obj; obj = NULL; } \
            return obj;\
        }\
        public: static _className* findByAttribute(QString attrName, QVariant value, QString db)\
        {\
            _className *obj = new _className(db);\
            QString query = QString("select * from %1 where %2 = %3")\
                      .arg(_className::tableName())\
                      .arg(_className::fieldName(attrName))\
                      .arg(value.toString());\
            if (!obj->fillModelFromQuery(query)) { delete obj; obj = NULL; } \
            return obj;\
        }\
        public: static _className* findByPrimaryKey(QVariant primaryKey)\
        {\
            _className *obj = new _className();\
            if (!obj->fillModelByPrimaryKey(primaryKey))\
            {\
                delete obj;\
                obj = NULL;\
            }\
            return obj;\
        } \
        public: static _className* findByPrimaryKey(QVariant primaryKey, QString db)\
        {\
           _className *obj = new _className(db) ;\
            if (!obj->fillModelByPrimaryKey(primaryKey))\
            {\
                delete obj;\
                obj = NULL;\
            }\
            return obj;\
        } \
        public: void mapFields();

#define MODEL_BEGIN_MAP(className) \
            QSqlDatabase className::m_database; \
            bool className::m_usecustodatabase; \
            void className::mapFields() {

#define MODEL_MATCH_TABLE(className, _tableName)\
        MODEL_DEFAULT_CONSTRUCTOR(className, _tableName) \
        MODEL_STATIC_METHODS(className, _tableName)

#define MODEL_MATCH_TABLE_WITH_DATABASE(className, _tableName, databaseName) \
        MODEL_CONSTRUCTOR_WITH_DATABASE(className, _tableName, databaseName) \
        MODEL_STATIC_METHODS(className, _tableName)

#define MODEL_MAP_FIELD(attr, _fieldName)\
            m_##attr.setFieldName(_fieldName);\
            Model::InitAttribute(&m_##attr);\
            m_attrList.append(&m_##attr);\
            m_fieldMap[#attr] = _fieldName;

#define MODEL_MAP_PRIMARYKEY(attr, _fieldName)\
            m_##attr.setFieldName(_fieldName);\
            m_attrList.append(&m_##attr);\
            m_fieldMap[#attr] = _fieldName;\
            m_primaryKey = &m_##attr;\
            m_primaryKey->setValue(0);

#define MODEL_MAP_FOREIGNKEY(attr, obj, objType)\
            obj = new objType(); \
            obj->fillModelByPrimaryKey(attr->value());


#define MODEL_END_MAP() }

#define DECLARE_MODEL(className)\
            class className;\
            typedef QList<className*> className##List;

#define MODEL_INIT_LIST(className, _list)\
            if (NULL == _list) _list = new className##List(); \
            foreach(className* obj, *_list) delete obj;\
            _list->clear();

#define MODEL_CLEAN_LIST(className, _list) \
            foreach(className* obj, *_list) delete obj;\
            _list->clear();


class Model
{
public:
    Model();
    virtual ~Model() {}

    QString fieldName(QString attribute);
    QSqlError lastError();
    virtual void mapFields() = 0;
    virtual QSqlDatabase getDatabase() = 0;
    virtual bool isCustomDatabase() = 0;

    virtual bool Save();
    virtual bool Create();
    virtual bool Update();
    virtual bool Delete();
    bool logError();

    bool refresh();
    int  lastInsertId();

    void copyFrom(Model* model);

    QList<ModelAttribute*> attributes();

    static void InitAttribute(ModelAttribute *attr);
    static QString getNoAccentuation( QString strPortuguese ) ;
protected:
    virtual QString className() = 0;
protected:
    QString                 m_tableName;
    QList<ModelAttribute*>  m_attrList;
    ModelAttribute*         m_primaryKey;
    static QMap<QString, QString> m_fieldMap;
    QSqlError m_lastError;
    void DumpSQLError( QString query, bool bUpdate = false  );
    bool TableExist();
    bool FieldExist(QString FieldName);
    void CreateTable( bool bUpdate =  false );
    QString CreateFieldTxt( ModelAttribute *attr );
    void EnsureFieldsExist();
    void fillModel(QSqlRecord record);
    QString DebugAttribute(ModelAttribute *attr);

    bool do_insert();
    bool do_update();
    bool do_delete();

};

#endif // MODEL_H
