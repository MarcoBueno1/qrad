#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QPluginLoader>
#include <QFileDialog>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    m_loader = new QPluginLoader(this);
    m_model = new PluginModel(this);

    m_propModel = new PropertyTableModel();

    ui->tableView->setModel(m_model);
    ui->tableViewProp->setModel(m_propModel);

    m_iface = NULL;
    ui->pushButton_2->setEnabled(false);

    m_sync = new PropertySync();
    m_sync->start();

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(onLoad()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(onProcess()));
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onAction(QModelIndex)));
    connect(ui->pushButtonGet, SIGNAL(clicked()), this, SLOT(onGetProperty()));
    connect(ui->pushButtonSet, SIGNAL(clicked()), this, SLOT(onSetProperty()));
    connect(ui->pushButtonAddWatch, SIGNAL(clicked()), this, SLOT(onAddWatch()));
    connect(m_sync, SIGNAL(update()), m_propModel, SLOT(update()));

}

Dialog::~Dialog()
{
    delete m_model;
    delete m_propModel;
    delete ui;

}

void Dialog::setFileName(QString fileName)
{
    ui->lineEditPlugin->setText(fileName);

    m_loader->setFileName(fileName);

    if (!m_loader->load())
    {
        QMessageBox::critical(this, "Error", m_loader->errorString());
        return;
    }

    m_iface = qobject_cast<QRadPluginInterface*>(m_loader->instance());

    if (!m_iface)
    {
        QMessageBox::critical(this, "Error", "Plugin is null.");
        return;
    }

    m_iface->setParent(this);

    m_iface->onLoad(NULL);

    m_model->setPlugin(m_iface);
    m_propModel->setInterface(m_iface);


    ui->pushButton->setText("Unload");
    ui->pushButton_2->setEnabled(true);

    ui->tableView->selectRow(0);
    ui->tableView->selectRow(0);
//    ui->pushButton_2->setFocus();

}

void Dialog::setActionName(QString act)
{
    ui->lineEditAction->setText(act);
}

void Dialog::onLoad()
{
    if (NULL != m_iface)
	{
		qDebug() << "Unloading...";
		m_iface = NULL;

		m_model->setPlugin(NULL);

		ui->lineEditPlugin->clear();
		ui->lineEditAction->clear();
		ui->pushButton->setText("Load");
		ui->pushButton_2->setEnabled(false);

		return;
	}

	QFileDialog file(this);

#ifndef _WIN32
	QString fileName = file.getOpenFileName(this, "Test", "../../src/qrad-plugins", "Plugin (*.so)");
#else
	QString fileName = file.getOpenFileName(this, "Test", "../../../src/qrad-plugins", "Plugin (*.dll)");
#endif

	if (NULL == fileName)
	{
		QMessageBox::critical(this, "Error", "Could not found plugin.");
		return;
	}

	setFileName(fileName);
    ui->tableView->setFocus();

}

void Dialog::onProcess()
{
    if (!m_iface)
        return;

    m_iface->Process(ui->lineEditAction->text());

}

void Dialog::onAction(QModelIndex index)
{
    QString action = m_model->action(index);

    ui->lineEditAction->setText(action);

}

void Dialog::onSetProperty()
{
    if (NULL == m_iface)
        return;

    QString propName  = ui->lineEditPropName->text();
    QString propValue = ui->lineEditPropValue->text();

    m_iface->setProperty(propName.toUtf8().data(), QVariant(propValue));

}

void Dialog::onGetProperty()
{
    if (NULL == m_iface)
        return;

    QString propName  = ui->lineEditPropName->text();

    QString propValue = m_iface->property(propName.toUtf8().data()).toString();

    ui->lineEditPropValue->setText(propValue);

}

void Dialog::onAddWatch()
{
    QString propName = ui->lineEditWatch->text();

    if (propName.isEmpty())
        return;

    m_propModel->appendProperty(propName);
    ui->lineEditWatch->clear();

}



/* Sync */

PropertySync::PropertySync()
{
}

void PropertySync::run()
{
/*    while (true)
    {
        emit update();
        sleep(1);
    }
    */
}

