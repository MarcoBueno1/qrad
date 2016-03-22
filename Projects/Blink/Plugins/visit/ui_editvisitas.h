/********************************************************************************
** Form generated from reading UI file 'editvisitas.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITVISITAS_H
#define UI_EDITVISITAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>
#include <qradlineeditcompleter.h>
#include <qradlineeditenter.h>

QT_BEGIN_NAMESPACE

class Ui_Editvisitas
{
public:
    QFrame *frame;
    QGroupBox *groupBox_2;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *LblPhoto;
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QLabel *LblData;
    QDateEdit *DtEdtData;
    QLabel *LblHora;
    QTimeEdit *TmEdtHora;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_2;
    QLabel *LblSaida;
    QDateEdit *DtEdtSaida;
    QLabel *Lblsaida_hora;
    QTimeEdit *TmEdtsaida_hora;
    QRadLineEditCompleter *lineEditRG;
    QLineEdit *lineEditCPF;
    QRadLineEditEnter *lineEditVisitante;
    QPushButton *pushButtonBaterFoto;
    QGroupBox *groupBox_5;
    QLineEdit *lineEditPreAutorizado;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *lineEditAnunciarChegada;
    QGroupBox *groupBox_4;
    QLabel *label_4;
    QRadLineEditCompleter *lineEditMorador;
    QLabel *label_5;
    QLabel *label_6;
    QRadLineEditEnter *lineEditAP;
    QRadLineEditEnter *lineEditTorre;
    QRadLineEditEnter *lineEditRamal;
    QLabel *label_9;
    QFrame *topmenu;
    QLabel *labelStatus;
    QPushButton *PshBtnCancel;
    QPushButton *PshBtnSave;

    void setupUi(QDialog *Editvisitas)
    {
        if (Editvisitas->objectName().isEmpty())
            Editvisitas->setObjectName(QStringLiteral("Editvisitas"));
        Editvisitas->resize(800, 600);
        frame = new QFrame(Editvisitas);
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
        groupBox = new QGroupBox(groupBox_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 160, 191, 100));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 28, 170, 62));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        LblData = new QLabel(layoutWidget);
        LblData->setObjectName(QStringLiteral("LblData"));
        LblData->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(LblData, 0, 0, 1, 1);

        DtEdtData = new QDateEdit(layoutWidget);
        DtEdtData->setObjectName(QStringLiteral("DtEdtData"));
        DtEdtData->setMinimumSize(QSize(20, 0));
        DtEdtData->setCalendarPopup(true);

        gridLayout->addWidget(DtEdtData, 0, 1, 1, 1);

        LblHora = new QLabel(layoutWidget);
        LblHora->setObjectName(QStringLiteral("LblHora"));
        LblHora->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(LblHora, 1, 0, 1, 1);

        TmEdtHora = new QTimeEdit(layoutWidget);
        TmEdtHora->setObjectName(QStringLiteral("TmEdtHora"));
        TmEdtHora->setMinimumSize(QSize(20, 0));

        gridLayout->addWidget(TmEdtHora, 1, 1, 1, 1);

        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(200, 164, 191, 90));
        layoutWidget1 = new QWidget(groupBox_3);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(11, 26, 161, 62));
        gridLayout_2 = new QGridLayout(layoutWidget1);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        LblSaida = new QLabel(layoutWidget1);
        LblSaida->setObjectName(QStringLiteral("LblSaida"));
        LblSaida->setMinimumSize(QSize(30, 0));

        gridLayout_2->addWidget(LblSaida, 0, 0, 1, 1);

        DtEdtSaida = new QDateEdit(layoutWidget1);
        DtEdtSaida->setObjectName(QStringLiteral("DtEdtSaida"));
        DtEdtSaida->setMinimumSize(QSize(100, 0));

        gridLayout_2->addWidget(DtEdtSaida, 0, 1, 1, 1);

        Lblsaida_hora = new QLabel(layoutWidget1);
        Lblsaida_hora->setObjectName(QStringLiteral("Lblsaida_hora"));
        Lblsaida_hora->setMinimumSize(QSize(30, 0));

        gridLayout_2->addWidget(Lblsaida_hora, 1, 0, 1, 1);

        TmEdtsaida_hora = new QTimeEdit(layoutWidget1);
        TmEdtsaida_hora->setObjectName(QStringLiteral("TmEdtsaida_hora"));
        TmEdtsaida_hora->setMinimumSize(QSize(60, 0));

        gridLayout_2->addWidget(TmEdtsaida_hora, 1, 1, 1, 1);

        lineEditRG = new QRadLineEditCompleter(groupBox_2);
        lineEditRG->setObjectName(QStringLiteral("lineEditRG"));
        lineEditRG->setGeometry(QRect(70, 40, 320, 27));
        lineEditCPF = new QLineEdit(groupBox_2);
        lineEditCPF->setObjectName(QStringLiteral("lineEditCPF"));
        lineEditCPF->setGeometry(QRect(70, 80, 320, 27));
        lineEditVisitante = new QRadLineEditEnter(groupBox_2);
        lineEditVisitante->setObjectName(QStringLiteral("lineEditVisitante"));
        lineEditVisitante->setGeometry(QRect(70, 120, 320, 27));
        pushButtonBaterFoto = new QPushButton(groupBox_2);
        pushButtonBaterFoto->setObjectName(QStringLiteral("pushButtonBaterFoto"));
        pushButtonBaterFoto->setGeometry(QRect(671, 184, 85, 27));
        pushButtonBaterFoto->setFocusPolicy(Qt::TabFocus);
        pushButtonBaterFoto->setAutoDefault(false);
        groupBox_5 = new QGroupBox(groupBox_2);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(440, 210, 311, 80));
        lineEditPreAutorizado = new QLineEdit(groupBox_5);
        lineEditPreAutorizado->setObjectName(QStringLiteral("lineEditPreAutorizado"));
        lineEditPreAutorizado->setGeometry(QRect(160, 20, 113, 27));
        lineEditPreAutorizado->setReadOnly(true);
        label_7 = new QLabel(groupBox_5);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(7, 31, 100, 17));
        label_8 = new QLabel(groupBox_5);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(8, 59, 141, 17));
        lineEditAnunciarChegada = new QLineEdit(groupBox_5);
        lineEditAnunciarChegada->setObjectName(QStringLiteral("lineEditAnunciarChegada"));
        lineEditAnunciarChegada->setGeometry(QRect(160, 50, 113, 27));
        lineEditAnunciarChegada->setReadOnly(true);
        groupBox_4 = new QGroupBox(frame);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 340, 910, 130));
        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(21, 27, 50, 17));
        lineEditMorador = new QRadLineEditCompleter(groupBox_4);
        lineEditMorador->setObjectName(QStringLiteral("lineEditMorador"));
        lineEditMorador->setGeometry(QRect(77, 27, 360, 27));
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
        topmenu = new QFrame(Editvisitas);
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
        PshBtnCancel = new QPushButton(Editvisitas);
        PshBtnCancel->setObjectName(QStringLiteral("PshBtnCancel"));
        PshBtnCancel->setGeometry(QRect(622, 568, 85, 27));
        PshBtnCancel->setFocusPolicy(Qt::TabFocus);
        PshBtnCancel->setAutoDefault(false);
        PshBtnSave = new QPushButton(Editvisitas);
        PshBtnSave->setObjectName(QStringLiteral("PshBtnSave"));
        PshBtnSave->setGeometry(QRect(709, 568, 85, 27));
        PshBtnSave->setFocusPolicy(Qt::TabFocus);
        PshBtnSave->setAutoDefault(false);
        QWidget::setTabOrder(lineEditRG, lineEditCPF);
        QWidget::setTabOrder(lineEditCPF, lineEditVisitante);
        QWidget::setTabOrder(lineEditVisitante, lineEditMorador);
        QWidget::setTabOrder(lineEditMorador, lineEditAP);
        QWidget::setTabOrder(lineEditAP, lineEditTorre);
        QWidget::setTabOrder(lineEditTorre, lineEditRamal);
        QWidget::setTabOrder(lineEditRamal, pushButtonBaterFoto);
        QWidget::setTabOrder(pushButtonBaterFoto, PshBtnCancel);
        QWidget::setTabOrder(PshBtnCancel, PshBtnSave);
        QWidget::setTabOrder(PshBtnSave, DtEdtData);
        QWidget::setTabOrder(DtEdtData, TmEdtHora);
        QWidget::setTabOrder(TmEdtHora, DtEdtSaida);
        QWidget::setTabOrder(DtEdtSaida, TmEdtsaida_hora);
        QWidget::setTabOrder(TmEdtsaida_hora, lineEditPreAutorizado);
        QWidget::setTabOrder(lineEditPreAutorizado, lineEditAnunciarChegada);

        retranslateUi(Editvisitas);

        pushButtonBaterFoto->setDefault(false);


        QMetaObject::connectSlotsByName(Editvisitas);
    } // setupUi

    void retranslateUi(QDialog *Editvisitas)
    {
        Editvisitas->setWindowTitle(QApplication::translate("Editvisitas", "Edit", 0));
        groupBox_2->setTitle(QApplication::translate("Editvisitas", " Visitante", 0));
        label->setText(QApplication::translate("Editvisitas", "RG", 0));
        label_2->setText(QApplication::translate("Editvisitas", "CPF", 0));
        label_3->setText(QApplication::translate("Editvisitas", "Nome", 0));
        LblPhoto->setText(QString());
        groupBox->setTitle(QApplication::translate("Editvisitas", " Entrada", 0));
        LblData->setText(QApplication::translate("Editvisitas", "Data", 0));
        LblHora->setText(QApplication::translate("Editvisitas", "Hora", 0));
        groupBox_3->setTitle(QApplication::translate("Editvisitas", " Saida", 0));
        LblSaida->setText(QApplication::translate("Editvisitas", "Saida", 0));
        Lblsaida_hora->setText(QApplication::translate("Editvisitas", "Hora", 0));
        pushButtonBaterFoto->setText(QApplication::translate("Editvisitas", "Bater &Foto", 0));
        groupBox_5->setTitle(QApplication::translate("Editvisitas", "Informa\303\247\303\265es", 0));
        lineEditPreAutorizado->setText(QApplication::translate("Editvisitas", "NAO", 0));
        label_7->setText(QApplication::translate("Editvisitas", "Pre-Autorizado?", 0));
        label_8->setText(QApplication::translate("Editvisitas", "Anunciar sua chegada?", 0));
        lineEditAnunciarChegada->setText(QApplication::translate("Editvisitas", "SIM", 0));
        groupBox_4->setTitle(QApplication::translate("Editvisitas", " Autorizado Por", 0));
        label_4->setText(QApplication::translate("Editvisitas", "Morador", 0));
        lineEditMorador->setPlaceholderText(QApplication::translate("Editvisitas", "Digite o n\303\272mero do AP ou nome do morador", 0));
        label_5->setText(QApplication::translate("Editvisitas", "AP", 0));
        label_6->setText(QApplication::translate("Editvisitas", "Torre", 0));
        label_9->setText(QApplication::translate("Editvisitas", "Ramal", 0));
        labelStatus->setStyleSheet(QString());
        labelStatus->setText(QApplication::translate("Editvisitas", "Nova Visita", 0));
        PshBtnCancel->setText(QApplication::translate("Editvisitas", "&Cancelar", 0));
        PshBtnSave->setText(QApplication::translate("Editvisitas", "&Salvar", 0));
    } // retranslateUi

};

namespace Ui {
    class Editvisitas: public Ui_Editvisitas {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITVISITAS_H
