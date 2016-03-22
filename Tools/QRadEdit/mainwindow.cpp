#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDomDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_encryptedFile = new QRadSecureFile();
    m_plainFile     = new QFile();

    m_highlighter = new Highlighter(ui->plainTextEdit);

    m_useEncryption = true;

    m_currentDirectory = ".";

    connect(ui->plainTextEdit->document(), SIGNAL(contentsChanged()), this, SLOT(onTextChange()));

    if (QCoreApplication::instance()->arguments().count() > 1)
    {
        if (QFile::exists(QCoreApplication::instance()->arguments().at(1)))
        {
            ContinueOpenEncrypted(QCoreApplication::instance()->arguments().at(1)   );
        }
    }

    m_hasChange     = false;

}

MainWindow::~MainWindow()
{
    if (m_plainFile->isOpen())
    {
        m_plainFile->close();
    }

    if (m_encryptedFile->isOpen())
    {
        m_encryptedFile->close();
    }

    delete m_highlighter;
    delete m_plainFile;
    delete m_encryptedFile;
    delete ui;
}

void MainWindow::on_action_Novo_triggered()
{
    NewEncrypted();
}

void MainWindow::on_action_Abrir_triggered()
{
    OpenEncrypted();
}

void MainWindow::on_action_Salvar_triggered()
{
    SaveEncrypted();
}

void MainWindow::on_actionSalvar_Como_triggered()
{
    SaveAsEncrypted();
}

bool MainWindow::NewEncrypted(void)
{
    if (m_hasChange)
    {
        int res = QMessageBox::question(this,
                                        windowTitle(),
                                        QString::fromUtf8("Existem alterações que não foram salvas. Deseja salvá-las?"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::Yes);

        if (QMessageBox::Yes == res)
        {
            SaveEncrypted();
        }
    }
    else if (m_encryptedFile->isOpen())
    {
        m_encryptedFile->close();
    }

    ui->plainTextEdit->clear();
    m_hasChange = false;

    return true;
}

bool MainWindow::OpenEncrypted(void)
{
    if (m_hasChange)
    {
        int res = QMessageBox::question(this,
                                        windowTitle(),
                                        QString::fromUtf8("Existem alterações que não foram salvas. Deseja salvá-las?"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::Yes);

        if (QMessageBox::Yes == res)
        {
            SaveEncrypted();
        }
    }
    else if (m_encryptedFile->isOpen())
    {
        m_encryptedFile->close();
    }

    QFileDialog dlgFile(this);
    QString fileName = dlgFile.getOpenFileName(this, windowTitle(), m_currentDirectory, "All Files(*.*)");

    QStringList aux = fileName.split('.', QString::SkipEmptyParts);

    if (fileName.isEmpty())
    {
        return false;
    }

    ContinueOpenEncrypted(fileName);

    return true;
}

void MainWindow::ContinueOpenEncrypted(QString fileName)
{
    m_encryptedFile->setFileName(fileName);

    if (!m_encryptedFile->open(QIODevice::ReadWrite))
    {
        return;
    }

    QByteArray content = m_encryptedFile->readAll();

    if (content.isEmpty())
    {
        QMessageBox::warning(this,
                             windowTitle(),
                             QString::fromUtf8("O arquivo aberto está vazio ou não foi possível decifrar o arquivo."));
    }

    /* check if is xml */

    QDomDocument doc;
    QString err;
    int l, c;

    if (doc.setContent(QString(content), &err, &l, &c))
        m_highlighter->setType(Highlighter::HighlightGenericXml);

    ui->plainTextEdit->clear();
    ui->plainTextEdit->appendPlainText(QString::fromUtf8(content));

    m_hasChange = false;

    ui->statusBar->setToolTip("Editando...");
}

bool MainWindow::SaveEncrypted(void)
{
    if (m_encryptedFile->isOpen())
    {
        m_encryptedFile->close();

        if (!m_encryptedFile->open(QIODevice::WriteOnly))
            return false;

        m_encryptedFile->write(ui->plainTextEdit->document()->toPlainText().toUtf8());
        m_encryptedFile->flush();

        m_hasChange = false;

        return true;
    }

    QFileDialog dlgFile(this);
    QString fileName = dlgFile.getSaveFileName(this, windowTitle(), m_currentDirectory, "All Files(*.*)");

    if (fileName.isEmpty())
    {
        return false;
    }

    m_encryptedFile->setFileName(fileName);

    if (!m_encryptedFile->open(QIODevice::WriteOnly))
    {
        return false;
    }

    m_encryptedFile->write(ui->plainTextEdit->document()->toPlainText().toUtf8());
    m_encryptedFile->flush();

    m_hasChange = false;

    return true;
}

bool MainWindow::SaveAsEncrypted(void)
{
    if (m_encryptedFile->isOpen() || m_hasChange)
    {
        m_encryptedFile->close();
        m_hasChange = false;
    }

    QFileDialog dlgFile(this);
    QString fileName = dlgFile.getSaveFileName(this, windowTitle(), m_currentDirectory, "All Files(*.*)");

    if (fileName.isEmpty())
    {
        return false;
    }

    m_encryptedFile->setFileName(fileName);

    if (!m_encryptedFile->open(QIODevice::WriteOnly))
    {
        return false;
    }

    m_encryptedFile->write(ui->plainTextEdit->document()->toPlainText().toUtf8());
    m_encryptedFile->flush();

    return true;
}

bool MainWindow::NewPlain(void)
{
    return true;
}

bool MainWindow::OpenPlain(void)
{
    return true;
}

bool MainWindow::SavePlain(void)
{
    return true;
}

bool MainWindow::SaveAsPlain(void)
{
    return true;
}

void MainWindow::onTextChange(void)
{
    m_hasChange = true;
}

void MainWindow::on_actionSair_triggered()
{
    if (m_hasChange)
    {
        int res = QMessageBox::question(this,
                                        windowTitle(),
                                        QString::fromUtf8("Existem alterações que não foram salvas. Deseja salvá-las?"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::Yes);

        if (QMessageBox::Yes == res)
        {
            SaveEncrypted();
        }
    }
    close();
}
