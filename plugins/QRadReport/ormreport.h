#ifndef ormreport_H
#define ormreport_H

#include "orm.h"

DECLARE_ORM(report)

DECLARE_CLASS(report)
    ORM_DECLARE_ATTRIBUTE(report, int, Id)
    ORM_DECLARE_ATTRIBUTE(report, QString, Name)
    ORM_DECLARE_ATTRIBUTE(report, QString, Path)
    ORM_DECLARE_ATTRIBUTE(report, QString, Menu)
    ORM_DECLARE_ATTRIBUTE(report, int, DateMode)
    ORM_DECLARE_ATTRIBUTE(report, QString, Sql)
    ORM_DECLARE_ATTRIBUTE(report, QString, SqlCombo)
    ORM_DECLARE_ATTRIBUTE(report, QString, Permission)
    ORM_DECLARE_ATTRIBUTE(report, bool, Visible)
    ORM_MATCH_TABLE(report, "report")
ORM_END_MAP()


 
#endif 
