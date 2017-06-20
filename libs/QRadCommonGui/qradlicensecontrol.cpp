#include <QSqlQuery>
#include <QSqlRecord>
#include <QRect>
#include <QMessageBox>
#include <QString>
#include "qradlicensecontrol.h"
#include "ui_qradlicensecontrol.h"
//#include "qradsgmessages.h"


#define SQL_SELECT_LAST_LICENSE  "select lastlicense from maincompany"


LicenseControl::LicenseControl(Clicense *pLicense, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::LicenseControl)
{
    int iResult;
    QString strClientIdentification;
    m_pLicense = pLicense;
    m_ui->setupUi(this);

    /* Deve ser seguida esta ordem pois no Windows existe um corportamente estranho [begin] */
    m_ui->groupBox_2->setVisible(false);
    this->setFixedHeight(234);
    /*  [end]  */

    iResult = m_pLicense->IsValidlicense( GetLastLicense() );
    if(   ( iResult != DSM_ERR_INVALID_LICENSE )
        && ( iResult != DSM_ERR_DATE_FRAUD ) )
    {
        m_ui->lblFirstMessage->setText( QString::fromUtf8("Importante: Você já esta utilizando uma licença válida.") );
        m_bAllowClose = true;
    }
    else
    {
        m_bAllowClose = false;
    }

    strClientIdentification = m_pLicense->GetClientIdentification();
    if( 0 != strClientIdentification.trimmed().length())
    {
       m_ui->textEditCredenciais->append( strClientIdentification );
    }
    else
    {
       m_ui->textEditCredenciais->append( QString::fromUtf8("Problema: Suas credenciais não podem ser apresentadas. Foi detectado que seu sistema não foi completamente instalado, por favor, finalize a instalação." ));
    }

    m_ui->lineEditlicencaAtual->setText( GetLastLicense());

    connect(m_ui->pushButtonAvancado, SIGNAL(clicked()), this, SLOT(ShowDetails()));

}

LicenseControl::~LicenseControl()
{
    delete m_ui;
}

void LicenseControl::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void LicenseControl::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Escape:
        {
            this->close();
        }
        break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            on_pushButtonOk_clicked();
        }
        break;
        default:
            QWidget::keyPressEvent( event );
            break;
    }

}

void LicenseControl::on_pushButtonOk_clicked()
{
    int iResult;
    QString strLicense = m_ui->lineEditNovaLicenca->text();
    iResult = m_pLicense->IsValidlicense(strLicense );

    if (m_ui->lineEditNovaLicenca->text().isEmpty())
    {
        QMessageBox::warning(this, QString("Controle de Licenças"), QString("A Licença está vazia, por favor informe uma nova licença"));
        reject();
    }
    else if ( iResult == DSM_ERR_INVALID_LICENSE  )
    {
        QMessageBox::warning(this,  QString("Controle de Licenças"), QString("Esta Licença é Inválida, por favor informe uma nova licença"));
        reject();
    }
    else if ( iResult == DSM_ERR_DATE_FRAUD )
    {
        QMessageBox::warning(this,  QString("Controle de Licenças") , QString("Foi detectada uma tentativa de fraude, por favor informe uma licença válida"));
        reject();

    }
    else // qrad_SUCCESS
    {
        QMessageBox::information(this,  QString("Controle de Licenças"), QString("Licença atualizada com sucesso!"));
        accept();
    }
}

QString LicenseControl::GetLastLicense()
{
    QSqlQuery query;

    query.prepare( SQL_SELECT_LAST_LICENSE );

    if( !query.exec() )
    {
        return "";
    }
    query.first();

    return query.value(query.record().indexOf("lastlicense")).toString();
}

void LicenseControl::closeEvent(QCloseEvent *event)
{
    if( !m_bAllowClose )
    {
       event->ignore();
    }
}

void LicenseControl::ShowDetails(void)
{
    if (m_ui->pushButtonAvancado->isChecked())
    {
        /* Deve ser seguida esta ordem pois no Windows existe um corportamente estranho [begin] */
        this->setFixedHeight(400);
        m_ui->groupBox_2->setVisible(true);
        /*  [end]  */
    }
    else
    {
        m_ui->groupBox_2->setVisible(false);
        this->setFixedHeight(234);
    }

}
