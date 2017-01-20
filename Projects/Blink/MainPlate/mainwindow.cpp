#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QMessageBox>

#define CMD "c:\\dvl\\test_ocr\\openalpr_32\\alpr.exe -c eu \"c:\\Program Files (x86)\\Tesseract-OCR\\placa.jpg\""

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
QString MainWindow::FindPlate(QString path)
{
    QProcess *process = new QProcess(this);
    QString program = CMD;
    process->start(program);
    process->waitForStarted();
    process->waitForFinished();
    process->waitForReadyRead();
    QStringList data = QString(process->readAll()).split("\n");
    //if(!data.count())
//        QMessageBox::information(this,"Result", data.join());
//    else
        QMessageBox::information(this,"Placa:", data.at(1));


    /*




   alpr.exe -c eu "c:\Program Files (x86)\Tesseract-OCR
\placa.jpg" > result.txt

    QLibrary wrp("c:\\dvl\\qrad\\bin\\OpenalprWrapper");
    typedef int (*Find)(char *, char *);

    if( !wrp.load() )
        QMessageBox::information(this, "Nao pude carregar a DLL", "load");

    Find fnFind = (Find) wrp.resolve("Find");
    if (!fnFind)
    {
        QMessageBox::information(this, "Nao pude resolver a DLL", "Error");
        return "";
    }

    char szPlate[1024];
    if( fnFind(path.toLatin1().data(), szPlate))
        return QString(szPlate);

    return "";
    */
}

void MainWindow::on_pushButton_clicked()
{
    FindPlate("C:\\qrad\\bin\\placa.jpg");

//    QMessageBox::information(this, "Retornado", QString(plate));
}
