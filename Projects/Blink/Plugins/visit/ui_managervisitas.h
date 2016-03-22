/********************************************************************************
** Form generated from reading UI file 'managervisitas.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MANAGERVISITAS_H
#define UI_MANAGERVISITAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <qradlineeditfakeenter.h>
#include "qradtableviewsearch.h"

QT_BEGIN_NAMESPACE

class Ui_Managervisitas
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBoxItens;
    QHBoxLayout *horizontalLayout_4;
    QRadTableViewSearch *tableViewSearch;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QRadLineEditFakeEnter *lineEditSearch;
    QPushButton *PshBtnEditar;
    QPushButton *PshBtnNovo;
    QPushButton *PshBtnSair;

    void setupUi(QDialog *Managervisitas)
    {
        if (Managervisitas->objectName().isEmpty())
            Managervisitas->setObjectName(QStringLiteral("Managervisitas"));
        Managervisitas->resize(800, 600);
        tabWidget = new QTabWidget(Managervisitas);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 800, 516));
        QFont font;
        font.setFamily(QStringLiteral("Verdana"));
        font.setPointSize(8);
        tabWidget->setFont(font);
        tabWidget->setFocusPolicy(Qt::NoFocus);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBoxItens = new QGroupBox(tab);
        groupBoxItens->setObjectName(QStringLiteral("groupBoxItens"));
        QFont font1;
        font1.setFamily(QStringLiteral("Verdana"));
        font1.setPointSize(9);
        font1.setBold(false);
        font1.setWeight(50);
        groupBoxItens->setFont(font1);
        groupBoxItens->setAlignment(Qt::AlignCenter);
        horizontalLayout_4 = new QHBoxLayout(groupBoxItens);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        tableViewSearch = new QRadTableViewSearch(groupBoxItens);
        tableViewSearch->setObjectName(QStringLiteral("tableViewSearch"));
        tableViewSearch->setFont(font);
        tableViewSearch->setFocusPolicy(Qt::NoFocus);
        tableViewSearch->setStyleSheet(QStringLiteral("selection-background-color: rgba(102, 176, 239, 75%); selection-color: rgb(255, 255, 255);"));
        tableViewSearch->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableViewSearch->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableViewSearch->setSortingEnabled(true);
        tableViewSearch->horizontalHeader()->setMinimumSectionSize(20);
        tableViewSearch->verticalHeader()->setVisible(false);
        tableViewSearch->verticalHeader()->setMinimumSectionSize(18);

        horizontalLayout_4->addWidget(tableViewSearch);


        gridLayout_2->addWidget(groupBoxItens, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(0, 60));
        groupBox_2->setMaximumSize(QSize(16777215, 60));
        QFont font2;
        font2.setFamily(QStringLiteral("Verdana"));
        font2.setPointSize(9);
        font2.setBold(true);
        font2.setWeight(75);
        groupBox_2->setFont(font2);
        groupBox_2->setAlignment(Qt::AlignHCenter|Qt::AlignTop);
        horizontalLayout = new QHBoxLayout(groupBox_2);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(9, 0, 9, 9);
        lineEditSearch = new QRadLineEditFakeEnter(groupBox_2);
        lineEditSearch->setObjectName(QStringLiteral("lineEditSearch"));
        lineEditSearch->setMinimumSize(QSize(0, 34));
        lineEditSearch->setMaximumSize(QSize(16777215, 34));
        QFont font3;
        font3.setFamily(QStringLiteral("Verdana"));
        font3.setPointSize(18);
        lineEditSearch->setFont(font3);

        horizontalLayout->addWidget(lineEditSearch);


        gridLayout_2->addWidget(groupBox_2, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        PshBtnEditar = new QPushButton(Managervisitas);
        PshBtnEditar->setObjectName(QStringLiteral("PshBtnEditar"));
        PshBtnEditar->setGeometry(QRect(620, 560, 85, 27));
        PshBtnNovo = new QPushButton(Managervisitas);
        PshBtnNovo->setObjectName(QStringLiteral("PshBtnNovo"));
        PshBtnNovo->setGeometry(QRect(533, 560, 85, 27));
        PshBtnSair = new QPushButton(Managervisitas);
        PshBtnSair->setObjectName(QStringLiteral("PshBtnSair"));
        PshBtnSair->setGeometry(QRect(707, 560, 85, 27));

        retranslateUi(Managervisitas);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Managervisitas);
    } // setupUi

    void retranslateUi(QDialog *Managervisitas)
    {
        Managervisitas->setWindowTitle(QString());
        groupBoxItens->setTitle(QApplication::translate("Managervisitas", "Visitas", 0));
        groupBox_2->setTitle(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Managervisitas", "Visitas", 0));
        PshBtnEditar->setText(QApplication::translate("Managervisitas", "&Editar", 0));
        PshBtnNovo->setText(QApplication::translate("Managervisitas", "&Novo", 0));
        PshBtnSair->setText(QApplication::translate("Managervisitas", "&Sair", 0));
    } // retranslateUi

};

namespace Ui {
    class Managervisitas: public Ui_Managervisitas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MANAGERVISITAS_H
