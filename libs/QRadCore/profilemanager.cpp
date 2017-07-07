#include "profilemanager.h"
#include "ui_profilemanager.h"
#include "profileregister.h"
#include <QMessageBox>
#include "qradshared.h"
#include "qradmessages.h"
#include "qradconfig.h"
#include "column2delegate.h"

#define SQL_SELECT_PROFILES         "select id, name from profile order by name"
#define SQL_SELECT_PROFILE_BY_ID    "select * from profile where id = %1"
#define SQL_SELECT_USER_BY_ID       "select * from dsm_user where profile = (%1) and removed = 'false'"
#define SQL_DELETE_PROFILE          "delete from profile where id = (%1)"

ProfileManager::ProfileManager(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ProfileManager)
{
    m_ui->setupUi(this);

    m_selectProfiles = new QSqlQueryModel;

    m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    InitialConfig();
    m_ui->tableViewProfile->addSearchColumn(1);
    m_ui->tableViewProfile->addSearchColumnFilter(1);

    connect(m_ui->lineEditSearch, SIGNAL(textEdited(QString)), m_ui->tableViewProfile, SLOT(Search(QString)));

    connect(m_ui->tableViewProfile, SIGNAL(found(QModelIndex)),this, SLOT(fillTheFields(QModelIndex)));
    connect(m_ui->tableViewProfile,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(m_ui->tableViewProfile, SIGNAL(found(QModelIndex)),this,SLOT(Founded(QModelIndex)));
    connect(m_ui->tableViewProfile, SIGNAL(notFound()),this,SLOT(notFound()));

    connect(m_ui->btnNew, SIGNAL(pressed()), this, SLOT(NewProfile()));
    connect(m_ui->btnEdit, SIGNAL(pressed()), this, SLOT(EditProfile()));
    connect(m_ui->btnDel, SIGNAL(pressed()), this, SLOT(DeleteProfile()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(close()));

}

ProfileManager::~ProfileManager()
{
    delete m_selectProfiles;
    delete m_ui;
}

void ProfileManager::changeEvent(QEvent *e)
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

void ProfileManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    InitialConfig();
    m_ui->lineEditSearch->setText(m_selectProfiles->index(0,1).data().toString());
    m_ui->tableViewProfile->selectRow(0);
    m_ui->lineEditSearch->selectAll();
    m_ui->lineEditSearch->setFocus();
    fillTheFields(m_ui->tableViewProfile->currentIndex());
    ShowCurrentInformations();
}

void ProfileManager::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Up:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Down:
        {
            m_ui->tableViewProfile->VerifyEvent(event);
            m_ui->lineEditSearch->setText(m_ui->tableViewProfile->currentIndex().sibling(m_ui->tableViewProfile->currentIndex().row(),m_ui->tableViewProfile->currentIndex().column()).data().toString());
            m_ui->lineEditSearch->selectAll();

            ShowCurrentInformations();
            break;
        }

    case Qt::Key_Delete:
        {
            DeleteProfile();
            break;
        }
    case Qt::Key_Escape:
        {
            close();
            break;
        }
    default:
        QWidget::keyPressEvent( event );
        break;
    }
}


void ProfileManager::InitialConfig(QString name)
{
    m_selectProfiles->setQuery(SQL_SELECT_PROFILES);
    m_ui->tableViewProfile->setModel(m_selectProfiles);
    ConfigHeaderTable();
    m_ui->tableViewProfile->show();
    m_ui->tableViewProfile->selectRow(0);

    QModelIndex currentIndex = m_ui->tableViewProfile->currentIndex();
    m_ui->tableViewProfile->setCurrentIndex(m_selectProfiles->index(0,1));

    if(!name.isEmpty())
    {
        QCoreApplication *app = QCoreApplication::instance();
        if (app->property("useTableFilter").toBool())
        {
            if( !m_ui->lineEditSearch->text().contains(name) && !name.startsWith(m_ui->lineEditSearch->text()) && !name.endsWith(m_ui->lineEditSearch->text()))
            {
                m_ui->tableViewProfile->searchSortModel(name);
                m_ui->lineEditSearch->setFocus();
                return;
            }
        }

        QModelIndex index = m_ui->tableViewProfile->model()->index(currentIndex.row(),1);
        QModelIndexList match = m_ui->tableViewProfile->model()->match(index,Qt::DisplayRole,name.toUpper(),1, Qt::MatchWrap);

        if(match.count())
        {
            m_ui->tableViewProfile->selectRow( match.at(0).row());
        }


    }

    m_ui->lineEditSearch->setFocus();
    ShowCurrentInformations();

}

void ProfileManager::ConfigHeaderTable(void)
{
    m_ui->tableViewProfile->hideColumn(0);

    m_selectProfiles->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Nome do Perfil"));
    m_ui->tableViewProfile->setColumnWidth(1, 744);
    m_ui->tableViewProfile->horizontalHeader()->setStretchLastSection(true);

    m_ui->tableViewProfile->setItemDelegateForColumn( 1, new ColumnLeft() );
}

void ProfileManager::fillTheFields(QModelIndex currentIndex)
{
    m_profileId = currentIndex.sibling(currentIndex.row(),0).data().toInt();
}

void ProfileManager::SearchProfile(QString text)
{
    int index;

    if ( text.isEmpty() )
        return;

    m_ui->tableViewProfile->keyboardSearch(text);

    index = m_ui->tableViewProfile->currentIndex().row();

    if (m_selectProfiles->record(index).value("name").toString().startsWith(m_ui->lineEditSearch->text(),Qt::CaseInsensitive))
    {
        m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
    else
    {
        m_ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
    }
}

void ProfileManager::NewProfile(void)
{
    QModelIndex currentIndex = m_ui->tableViewProfile->currentIndex();
    ProfileRegister *profileRegister = new ProfileRegister(this);
    QRadConfig::centralizarWidget(profileRegister);
    profileRegister->exec();
    QString name = profileRegister->GetProfileName();

    if(!name.isNull())
    {
        InitialConfig(name);
        getAlignment(m_ui->tableViewProfile->currentIndex());
    }
    else
        m_ui->tableViewProfile->selectRow(currentIndex.row());

    delete profileRegister;

    m_ui->lineEditSearch->setText(m_ui->tableViewProfile->currentIndex().sibling(m_ui->tableViewProfile->currentIndex().row(),m_ui->tableViewProfile->currentIndex().column()).data().toString());
    m_ui->lineEditSearch->selectAll();
    m_ui->lineEditSearch->setFocus();
    fillTheFields(m_ui->tableViewProfile->currentIndex());
    ShowCurrentInformations();
}

void ProfileManager::EditProfile(void)
{
    QModelIndex currentIndex = m_ui->tableViewProfile->currentIndex();
    ProfileRegister *profileRegister = new ProfileRegister(this);
    profileRegister->SendProfileId(m_profileId);
    QRadConfig::centralizarWidget(profileRegister);
    profileRegister->exec();
    QString name = profileRegister->GetProfileName();

    if(!name.isNull())
    {
        InitialConfig(name);
        getAlignment(m_ui->tableViewProfile->currentIndex());
    }
    else
        m_ui->tableViewProfile->selectRow(currentIndex.row());

    delete profileRegister;

    m_ui->lineEditSearch->setText(m_ui->tableViewProfile->currentIndex().sibling(m_ui->tableViewProfile->currentIndex().row(),m_ui->tableViewProfile->currentIndex().column()).data().toString());
    m_ui->lineEditSearch->selectAll();
    m_ui->lineEditSearch->setFocus();
    fillTheFields(m_ui->tableViewProfile->currentIndex());
    ShowCurrentInformations();
}

void ProfileManager::DeleteProfile(void)
{
    QSqlQueryModel  *select = new QSqlQueryModel;
    QModelIndex index = m_ui->tableViewProfile->currentIndex();

    select->setQuery(QString(SQL_SELECT_USER_BY_ID).arg(m_profileId));
    if (select->rowCount() == 0)
    {
        if (QMessageBox::question(this, MSG_QUESTION_TITLE, MSG_QUESTION_DELETE_PROFILE, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            QSqlQuery   *deleteProfile = new QSqlQuery;
            if (!deleteProfile->exec(QString(SQL_DELETE_PROFILE).arg(m_profileId)))
            {
                QMessageBox::critical(this, MSG_ERROR_DATABASE_TITLE, MSG_ERROR_DATABASE);
            }
            else
            {
                QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_DELETE_OK);
                InitialConfig();
            }
            delete deleteProfile;
        }
        m_ui->tableViewProfile->selectRow(index.row());
        m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
        m_ui->lineEditSearch->setText(m_ui->tableViewProfile->currentIndex().sibling(m_ui->tableViewProfile->currentIndex().row(),m_ui->tableViewProfile->currentIndex().column()).data().toString());
        m_ui->lineEditSearch->selectAll();
        getAlignment(m_ui->tableViewProfile->currentIndex());
        fillTheFields(m_ui->tableViewProfile->currentIndex());
        ShowCurrentInformations();
    }
    else
    {
        QMessageBox::warning(this, MSG_WARNING_TITLE, MSG_WARNING_DELETE_PROFILE);
    }
    delete select;


}

void ProfileManager::TableClicked(QModelIndex currentIndex)
{
    m_ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),1).data().toString());
    m_ui->lineEditSearch->selectAll();
    fillTheFields(currentIndex);
    ShowCurrentInformations();

}

void ProfileManager::ShowCurrentInformations(void)
{
    QString strTemp;

    if( m_selectProfiles->rowCount()  )
    {
     strTemp.sprintf("Perfis ( %d de %d )", m_ui->tableViewProfile->currentIndex().row() + 1, m_selectProfiles->rowCount());
     m_ui->groupBox_2->setTitle(strTemp);
    }

}

void ProfileManager::Founded(QModelIndex index)
{
    Q_UNUSED(index);
    ShowCurrentInformations();
    m_ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

}
void ProfileManager::notFound()
{
    m_ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
}

void ProfileManager::getAlignment(QModelIndex index)
{
    int averageRow = m_ui->tableViewProfile->getAverageRow()-1;
    int rowCount =  m_ui->tableViewProfile->model()->rowCount();

    if(index.row()!= 0)
    {
        if (index.row() < rowCount-(averageRow))
        {
            m_ui->tableViewProfile->selectRow(index.row() + averageRow);
            int e =  m_ui->tableViewProfile->currentIndex().row();
            m_ui->tableViewProfile->selectRow(e-averageRow);
        }
    }

}
