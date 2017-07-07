#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QSyntaxHighlighter>
#include <QDebug>
#include "highlighter.h"
#include "qradsecurefile.h"

#define LNX_PASSWORD QString("61a7529782cd60237f7da15e49167749")

#define LNX_CHECK_PASSWORD() \
     QString password = QString( QCryptographicHash::hash(QByteArray(argv[4]), QCryptographicHash::Md5).toHex() ); \
     if( password != LNX_PASSWORD )\
     {\
        fprintf(stderr, "\nInvalid Password!!\n\n");\
        exit(0);\
     }

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow      *ui;
    QRadSecureFile       *m_encryptedFile;
    QFile               *m_plainFile;
    QString             m_currentDirectory;
    bool                m_useEncryption;
    bool                m_hasChange;
    Highlighter         *m_highlighter;

    void    ContinueOpenEncrypted(QString fileName);

private slots:
    void on_actionSair_triggered();
    bool NewEncrypted(void);
    bool OpenEncrypted(void);
    bool SaveEncrypted(void);
    bool SaveAsEncrypted(void);

    bool NewPlain(void);
    bool OpenPlain(void);
    bool SavePlain(void);
    bool SaveAsPlain(void);

private slots:
    void on_actionSalvar_Como_triggered();
    void on_action_Salvar_triggered();
    void on_action_Abrir_triggered();
    void on_action_Novo_triggered();

    void onTextChange(void);
};

#endif // MAINWINDOW_H
