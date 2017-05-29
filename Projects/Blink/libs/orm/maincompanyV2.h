#ifndef maincompanyV2_H
#define maincompanyV2_H
 
#include "orm.h"
#include "city.h"
#include "state.h"

DECLARE_ORM(MainCompany)

DECLARE_CLASS(MainCompany)
    ORM_DECLARE_ATTRIBUTE(MainCompany,int, id)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, name)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, fantasyname)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, cnpj)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, ie)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, phone)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, email)
    ORM_DECLARE_FOREINGKEY(MainCompany,int, city, City, City)
    ORM_DECLARE_FOREINGKEY(MainCompany,int, state, State, State)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, CEP)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, Addrees)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, Neighborhood)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, HouseNumber)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, Complement)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, Max)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, License)
    ORM_DECLARE_ATTRIBUTE(MainCompany,QString, lastLicense)

     ORM_MATCH_TABLE(MainCompany, "maincompany")
ORM_END_MAP()
#endif 
