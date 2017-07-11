#include "qradprogresswindow.h"
#include "ui_qradprogresswindow.h"

QRadProgressWindow* QRadProgressWindow::m_instance = NULL;

QRadProgressWindow* QRadProgressWindow::getInstance()
{
    if (NULL == m_instance)
        m_instance = new QRadProgressWindow();

    return m_instance;
}


QRadProgressWindow::QRadProgressWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRadProgressWindow)
{
    ui->setupUi(this);
    setWindowTitle("Informação");
    setWindowModality(Qt::ApplicationModal);
}

QRadProgressWindow::~QRadProgressWindow()
{
    delete ui;
}

void QRadProgressWindow::setDetail( QString detail )
{
  ui->LblDetail->setText(detail);
  show();
}

void QRadProgressWindow::setTitle( QString Title )
{
  ui->LblTitle->setText(Title);
}

