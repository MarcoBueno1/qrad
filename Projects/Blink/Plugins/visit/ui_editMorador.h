/********************************************************************************
** Form generated from reading UI file 'editMorador.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITMORADOR_H
#define UI_EDITMORADOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <qradlineeditcompleter.h>
#include <qradlineeditenter.h>

QT_BEGIN_NAMESPACE

class Ui_EditMorador
{
public:
    QFrame *frame;
    QGroupBox *groupBox_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *LblPhoto;
    QRadLineEditCompleter *lineEditRG;
    QLineEdit *lineEditCPF;
    QRadLineEditEnter *lineEditMorador;
    QPushButton *pushButtonBaterFoto;
    QGroupBox *groupBox_4;
    QLabel *label_4;
    QRadLineEditCompleter *lineEditMorador1;
    QLabel *label_5;
    QLabel *label_6;
    QRadLineEditEnter *lineEditAP;
    QRadLineEditEnter *lineEditTorre;
    QRadLineEditEnter *lineEditRamal;
    QLabel *label_9;
    QCheckBox *checkBox;
    QFrame *topmenu;
    QLabel *labelStatus;
    QPushButton *PshBtnCancel;
    QPushButton *PshBtnSave;

    void setupUi(QDialog *EditMorador)
    {
        if (EditMorador->objectName().isEmpty())
            EditMorador->setObjectName(QStringLiteral("EditMorador"));
        EditMorador->resize(800, 600);
        frame = new QFrame(EditMorador);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(10, 50, 781, 500));
        frame->setFrameShape(QFrame::WinPanel);
        frame->setFrameShadow(QFrame::Raised);
        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 760, 310));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 46, 40, 17));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(20, 87, 40, 17));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 128, 40, 17));
        LblPhoto = new QLabel(groupBox_2);
        LblPhoto->setObjectName(QStringLiteral("LblPhoto"));
        LblPhoto->setGeometry(QRect(447, 0, 311, 181));
        LblPhoto->setPixmap(QPixmap(QString::fromUtf8("pessoa.jpg")));
        LblPhoto->setScaledContents(true);
        lineEditRG = new QRadLineEditCompleter(groupBox_2);
        lineEditRG->setObjectName(QStringLiteral("lineEditRG"));
        lineEditRG->setGeometry(QRect(70, 40, 320, 27));
        lineEditCPF = new QLineEdit(groupBox_2);
        lineEditCPF->setObjectName(QStringLiteral("lineEditCPF"));
        lineEditCPF->setGeometry(QRect(70, 80, 320, 27));
        lineEditMorador = new QRadLineEditEnter(groupBox_2);
        lineEditMorador->setObjectName(QStringLiteral("lineEditMorador"));
        lineEditMorador->setGeometry(QRect(70, 120, 320, 27));
        pushButtonBaterFoto = new QPushButton(groupBox_2);
        pushButtonBaterFoto->setObjectName(QStringLiteral("pushButtonBaterFoto"));
        pushButtonBaterFoto->setGeometry(QRect(671, 184, 85, 27));
        pushButtonBaterFoto->setFocusPolicy(Qt::TabFocus);
        pushButtonBaterFoto->setAutoDefault(false);
        groupBox_4 = new QGroupBox(groupBox_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 180, 440, 171));
        groupBox_4->setCheckable(true);
        groupBox_4->setChecked(false);
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(21, 27, 50, 17));
        lineEditMorador1 = new QRadLineEditCompleter(groupBox_4);
        lineEditMorador1->setObjectName(QStringLiteral("lineEditMorador1"));
        lineEditMorador1->setGeometry(QRect(77, 27, 360, 27));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(21, 60, 16, 17));
        label_6 = new QLabel(groupBox_4);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(21, 93, 32, 17));
        lineEditAP = new QRadLineEditEnter(groupBox_4);
        lineEditAP->setObjectName(QStringLiteral("lineEditAP"));
        lineEditAP->setGeometry(QRect(77, 60, 120, 27));
        lineEditAP->setReadOnly(true);
        lineEditTorre = new QRadLineEditEnter(groupBox_4);
        lineEditTorre->setObjectName(QStringLiteral("lineEditTorre"));
        lineEditTorre->setGeometry(QRect(77, 93, 120, 27));
        lineEditTorre->setReadOnly(true);
        lineEditRamal = new QRadLineEditEnter(groupBox_4);
        lineEditRamal->setObjectName(QStringLiteral("lineEditRamal"));
        lineEditRamal->setGeometry(QRect(310, 93, 120, 27));
        lineEditRamal->setReadOnly(true);
        label_9 = new QLabel(groupBox_4);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(265, 100, 41, 17));
        checkBox = new QCheckBox(frame);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(40, 320, 141, 22));
        checkBox->setChecked(true);
        topmenu = new QFrame(EditMorador);
        topmenu->setObjectName(QStringLiteral("topmenu"));
        topmenu->setGeometry(QRect(1, 1, 801, 42));
        topmenu->setMinimumSize(QSize(0, 42));
        topmenu->setMaximumSize(QSize(16777215, 42));
        topmenu->setStyleSheet(QStringLiteral("background-color: rgb(50, 0, 133);"));
        topmenu->setFrameShadow(QFrame::Raised);
        labelStatus = new QLabel(topmenu);
        labelStatus->setObjectName(QStringLiteral("labelStatus"));
        labelStatus->setEnabled(false);
        labelStatus->setGeometry(QRect(4, 0, 541, 39));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(50, 0, 133, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        labelStatus->setPalette(palette);
        QFont font;
        font.setFamily(QStringLiteral("Verdana"));
        font.setPointSize(18);
        font.setBold(true);
        font.setWeight(75);
        labelStatus->setFont(font);
        labelStatus->setFrameShape(QFrame::NoFrame);
        labelStatus->setFrameShadow(QFrame::Plain);
        labelStatus->setLineWidth(1);
        labelStatus->setTextFormat(Qt::AutoText);
        labelStatus->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelStatus->setWordWrap(false);
        labelStatus->setTextInteractionFlags(Qt::NoTextInteraction);
        PshBtnCancel = new QPushButton(EditMorador);
        PshBtnCancel->setObjectName(QStringLiteral("PshBtnCancel"));
        PshBtnCancel->setGeometry(QRect(622, 568, 85, 27));
        PshBtnSave = new QPushButton(EditMorador);
        PshBtnSave->setObjectName(QStringLiteral("PshBtnSave"));
        PshBtnSave->setGeometry(QRect(709, 568, 85, 27));

        retranslateUi(EditMorador);

        pushButtonBaterFoto->setDefault(false);


        QMetaObject::connectSlotsByName(EditMorador);
    } // setupUi

    void retranslateUi(QDialog *EditMorador)
    {
        EditMorador->setWindowTitle(QApplication::translate("EditMorador", "Edit", 0));
        groupBox_2->setTitle(QApplication::translate("EditMorador", " Morador", 0));
        label->setText(QApplication::translate("EditMorador", "RG", 0));
        label_2->setText(QApplication::translate("EditMorador", "CPF", 0));
        label_3->setText(QApplication::translate("EditMorador", "Nome", 0));
        LblPhoto->setText(QString());
        pushButtonBaterFoto->setText(QApplication::translate("EditMorador", "Bater &Foto", 0));
        groupBox_4->setTitle(QApplication::translate("EditMorador", " Pr\303\251-Autorizado?", 0));
        label_4->setText(QApplication::translate("EditMorador", "Morador", 0));
        lineEditMorador1->setPlaceholderText(QApplication::translate("EditMorador", "Digite o n\303\272mero do AP ou nome do morador", 0));
        label_5->setText(QApplication::translate("EditMorador", "AP", 0));
        label_6->setText(QApplication::translate("EditMorador", "Torre", 0));
        label_9->setText(QApplication::translate("EditMorador", "Ramal", 0));
        checkBox->setText(QApplication::translate("EditMorador", "Anunciar Chegada?", 0));
        labelStatus->setStyleSheet(QString());
        labelStatus->setText(QApplication::translate("EditMorador", "Cadastro de Morador", 0));
        PshBtnCancel->setText(QApplication::translate("EditMorador", "&Cancelar", 0));
        PshBtnSave->setText(QApplication::translate("EditMorador", "&Salvar", 0));
    } // retranslateUi

};

namespace Ui {
    class EditMorador: public Ui_EditMorador {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITMORADOR_H
