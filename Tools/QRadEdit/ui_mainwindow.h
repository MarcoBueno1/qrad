/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Novo;
    QAction *action_Abrir;
    QAction *action;
    QAction *action_Salvar;
    QAction *actionSalvar_Como;
    QAction *actionSair;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menuBar;
    QMenu *menu_Arquivo;
    QMenu *menu_Editar;
    QMenu *menuA_juda;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(700, 500);
        MainWindow->setMaximumSize(QSize(9999999, 9999999));
        MainWindow->setMouseTracking(true);
        MainWindow->setAcceptDrops(true);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/QRadedit.ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        MainWindow->setDocumentMode(true);
        MainWindow->setUnifiedTitleAndToolBarOnMac(true);
        action_Novo = new QAction(MainWindow);
        action_Novo->setObjectName(QStringLiteral("action_Novo"));
        action_Abrir = new QAction(MainWindow);
        action_Abrir->setObjectName(QStringLiteral("action_Abrir"));
        action = new QAction(MainWindow);
        action->setObjectName(QStringLiteral("action"));
        action_Salvar = new QAction(MainWindow);
        action_Salvar->setObjectName(QStringLiteral("action_Salvar"));
        actionSalvar_Como = new QAction(MainWindow);
        actionSalvar_Como->setObjectName(QStringLiteral("actionSalvar_Como"));
        actionSair = new QAction(MainWindow);
        actionSair->setObjectName(QStringLiteral("actionSair"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        QFont font;
        font.setFamily(QStringLiteral("Verdana"));
        plainTextEdit->setFont(font);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 700, 23));
        menu_Arquivo = new QMenu(menuBar);
        menu_Arquivo->setObjectName(QStringLiteral("menu_Arquivo"));
        menu_Editar = new QMenu(menuBar);
        menu_Editar->setObjectName(QStringLiteral("menu_Editar"));
        menuA_juda = new QMenu(menuBar);
        menuA_juda->setObjectName(QStringLiteral("menuA_juda"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_Arquivo->menuAction());
        menuBar->addAction(menu_Editar->menuAction());
        menuBar->addAction(menuA_juda->menuAction());
        menu_Arquivo->addAction(action_Novo);
        menu_Arquivo->addAction(action_Abrir);
        menu_Arquivo->addSeparator();
        menu_Arquivo->addAction(action_Salvar);
        menu_Arquivo->addAction(actionSalvar_Como);
        menu_Arquivo->addAction(actionSair);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QRad Edit", 0));
        action_Novo->setText(QApplication::translate("MainWindow", "N&ovo", 0));
        action_Novo->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0));
        action_Abrir->setText(QApplication::translate("MainWindow", "&Abrir", 0));
        action_Abrir->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", 0));
        action->setText(QApplication::translate("MainWindow", "_", 0));
        action_Salvar->setText(QApplication::translate("MainWindow", "&Salvar", 0));
        action_Salvar->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0));
        actionSalvar_Como->setText(QApplication::translate("MainWindow", "Salvar &Como", 0));
        actionSalvar_Como->setShortcut(QApplication::translate("MainWindow", "Ctrl+Shift+S", 0));
        actionSair->setText(QApplication::translate("MainWindow", "Sair", 0));
        actionSair->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0));
        menu_Arquivo->setTitle(QApplication::translate("MainWindow", "&Arquivo", 0));
        menu_Editar->setTitle(QApplication::translate("MainWindow", "&Editar", 0));
        menuA_juda->setTitle(QApplication::translate("MainWindow", "A&juda", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
