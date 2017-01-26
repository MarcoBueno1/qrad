#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraImageCapture>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void readyForCapture(bool ready);
    void processCapturedImage(int requestId, const QImage &img);
    void displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString);
    void updateCameraState(QCamera::State state);
    void displayCameraError();
    void timeout();
private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString FindPlate(QString path);
    QCamera *m_camera;
    QCameraImageCapture *m_imageCapture;
    QString m_FullPath;
    QTimer *m_timer;
    bool m_bSave;
};

#endif // MAINWINDOW_H
