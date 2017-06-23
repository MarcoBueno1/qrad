#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#define BLINK_RELEASE "1.0"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Blink - Controle de Acessos");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
#ifdef _WIN32_
    QPixmap bkgnd("C:\\Dvl\\qrad\\Projects\\Blink\\background.jpg");
#else
    QPixmap bkgnd("/home/marco/cpcs/qrad/Projects/Blink/background.jpg");
#endif
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

}

void MainWindow::on_actionSobre_o_Blink_triggered()
{
    QMessageBox::about(this, QString::fromUtf8("Sobre o Blink"),
                       QString::fromUtf8("<h2>Blink %1</h2>"
                          "<p>Copyright &copy; 2017 Marco Bueno."
                          "<p>Blink é um sistema de controle de acessos para condomínios.\n").arg(BLINK_RELEASE));

}
