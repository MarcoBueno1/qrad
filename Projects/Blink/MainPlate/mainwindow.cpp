#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QMessageBox>
#include <QCameraInfo>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlField>
#include <QFileDialog>
#include "qradshared.h"

#define CMD_DBD "c:\\dvl\\test_ocr\\openalpr_32\\alpr.exe -c eu \"c:\\Program Files (x86)\\Tesseract-OCR\\placa.jpg\""
#define CMD_CASA "C:\\Dvl\\ocr\\openalpr_64\\alpr.exe -c eu \"c:\\Program Files (x86)\\Tesseract-OCR\\placa.jpg\""

#define SELECT "select d.name as \"Morador\", t.name as \"Torre\", ap.numero as \"Apartamento\", v.board as \"Placa\" from dweller d "\
               " inner join vehicle v on v.owner = d.id"\
               " inner join tower t on t.id = d.tower "\
               " inner join ap on ap.id = d.ap where v.board = '%1'"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_camera = NULL;
    m_timer= NULL;
    m_bSave = true;

    if(!QCameraInfo::availableCameras().count())
        QMessageBox::warning(this, "Oops!", "Não foram encontradas cameras conectadas a este computador");
    else
    {
        m_FullPath = "C:\\Dvl\\qrad\\bin\\image.jpeg";
        m_camera = new QCamera(QCameraInfo::defaultCamera()) ;
        m_imageCapture = new QCameraImageCapture(m_camera);

        connect(m_camera, SIGNAL(stateChanged(QCamera::State)), this, SLOT(updateCameraState(QCamera::State)));
        connect(m_camera, SIGNAL(error(QCamera::Error)), this, SLOT(displayCameraError()));

        connect(m_imageCapture, SIGNAL(readyForCaptureChanged(bool)), this, SLOT(readyForCapture(bool)));
        connect(m_imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(processCapturedImage(int,QImage)));
        connect(m_imageCapture, SIGNAL(imageSaved(int,QString)), this, SLOT(imageSaved(int,QString)));
        connect(m_imageCapture, SIGNAL(error(int,QCameraImageCapture::Error,QString)), this,
                SLOT(displayCaptureError(int,QCameraImageCapture::Error,QString)));

        m_camera->setViewfinder(ui->viewfinder);
        m_camera->start();
        m_timer = new QTimer;
        m_timer->setInterval(500);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
        m_timer->start();

    }

}

void MainWindow::timeout()
{
    m_imageCapture->capture();
}

void MainWindow::updateCameraState(QCamera::State state)
{
    switch (state) {
    case QCamera::ActiveState:
        //ui->actionStartCamera->setEnabled(false);
       // ui->actionStopCamera->setEnabled(true);
       // ui->captureWidget->setEnabled(true);
        //ui->actionSettings->setEnabled(true);
        break;
    case QCamera::UnloadedState:
    case QCamera::LoadedState:
       // ui->actionStartCamera->setEnabled(true);
       // ui->actionStopCamera->setEnabled(false);
//        ui->captureWidget->setEnabled(false);
       // ui->actionSettings->setEnabled(false);
        break;
    }
}

void MainWindow::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera error"), m_camera->errorString());
}
void MainWindow::readyForCapture(bool ready)
{
  //  ui->takeImageButton->setEnabled(ready);
}

void MainWindow::processCapturedImage(int requestId, const QImage& img)
{
    Q_UNUSED(requestId);

    if( m_bSave )
    img.save(m_FullPath);
    //accept();
}

void MainWindow::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Oops! Problema ao capturar imagem..."), errorString);
    //isCapturingImage = false;
}

MainWindow::~MainWindow()
{
    if( m_timer )
    {
        m_timer->stop();
        delete m_timer;
    }
    if( m_camera )
        delete m_camera;

    delete ui;
}
QString MainWindow::FindPlate(QString path)
{
    m_bSave = false;
    QString result = "NAO IDENTIFICADO";
    QProcess *process = new QProcess(this);
    QString program = QString("%1 %2").arg(CMD_CASA).arg(path);
    process->start(program);
    process->waitForStarted(3000);
    process->waitForFinished(3000);
    process->waitForReadyRead(3000);
    QString stRead = QString(process->readAll());
    if( stRead.length() )
    {
         QStringList data = stRead.split("\n");
         result = data.at(1);
    }

    m_bSave = true;
    return result;
}

void MainWindow::on_pushButton_clicked()
{
    ui->LblMorador->setText("NAO CADASTRADO, PLACA:" );
    ui->LblMorador->setStyleSheet(FG_COLOR_RED);

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Abrir Arquivo"), "C:\\dvl\\qrad", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));


//    QString plate = FindPlate("C:\\Dvl\\qrad\\bin\\image.jpeg"/*"C:\\Dvl\\ocr\\openalpr_64\\WhatsApp.jpeg"*/);
    QString plate = FindPlate(fileName);

    plate.truncate(plate.indexOf("confidence"));
    plate.remove(plate.indexOf("-"),1);
    plate = plate.trimmed();
    if( !plate.isEmpty())
    {
       QSqlQueryModel *model = new QSqlQueryModel;
       model->setQuery(QString(SELECT).arg(plate));
       if(model->rowCount() > 0 )
       {
           QString Dweller = model->record(0).field(0).value().toString();
           QString Tower = model->record(0).field(1).value().toString();
           QString Ap = model->record(0).field(2).value().toString();
           QString Plate = model->record(0).field(3).value().toString();

           ui->LblMorador->setText(QString("%1, %2, %3, %4").arg(Dweller).arg(Tower).arg(Ap).arg(Plate));
           ui->LblMorador->setStyleSheet(FG_COLOR_GREEN);
       }
       delete model;
//      QMessageBox::information(this, "Retornado", QString(plate));
    }
}
