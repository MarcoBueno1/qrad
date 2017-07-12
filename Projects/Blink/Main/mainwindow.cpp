#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "clicense.h"
#include <QLabel>

#define BLINK_RELEASE "1.0"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CSM - Gerência de Condomínios");

/*
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);
    setWindowState(Qt::WindowMaximized);
*/

//    setWindowIcon(QIcon(":/png/mainicon.png"));
    Clicense *pLic = new Clicense ;

    setRemainingDays(pLic->RemainingDays());
    delete pLic;
}



void MainWindow::setRemainingDays(int value)
{
   QLabel *label;
    if ( value == 0 )
    {
        label = new QLabel(QString("A sua licença expirou! Entre em contato com a M2 para renovação."));
        label->setStyleSheet("color: rgb(255, 0, 0);font: bold;");
    }
    else if ( value <= 7  )
    {
        label = new QLabel(QString("Faltam apenas %1 dia(s) para a sua licença vencer. Entre em contato com a M2 para renovação.").arg(value));
        label->setStyleSheet("color: rgb(255, 85, 0);font: bold;");
    }
    else
    {
        label = new QLabel(QString("A sua licença é válida por %1 dias.").arg(value));
        label->setStyleSheet("color: rgb(255, 255, 255);font: bold;");
    }
    statusBar()->insertPermanentWidget(0,label,1);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
#ifdef _WIN32
    QPixmap bkgnd("C:\\Dvl\\qrad\\Projects\\Blink\\background.jpg");
#else
    QPixmap bkgnd("/home/marco/cpcs/qrad/Projects/Blink/background.jpg");
#endif
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio/*Qt::KeepAspectRatio*/);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

}

void MainWindow::on_actionSobre_o_Blink_triggered()
{
    QMessageBox::about(this, QString::fromUtf8("Sobre o Blink"),
                       QString::fromUtf8("<h2>CSM %1</h2>"
                          "<p>Copyright &copy; 2017 Marco Bueno."
                          "<p>Blink é um sistema de controle de acessos para condomínios.\n").arg(BLINK_RELEASE));

}
