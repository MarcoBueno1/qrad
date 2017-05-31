#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QPluginLoader>
#include "qradplugininterface.h"
#include "pluginmodel.h"
#include "propertytablemodel.h"

namespace Ui
{
    class Dialog;
}

class PropertySync : public QThread
{
    Q_OBJECT
public:
    PropertySync();
    void run();

signals:
    void update();
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

    void setFileName(QString);
    void setActionName(QString);

private:
    Ui::Dialog *ui;
    QPluginLoader *m_loader;
    QRadPluginInterface* m_iface;
   PluginModel *m_model;
    PropertyTableModel *m_propModel;
    PropertySync *m_sync;

private slots:
    void onLoad();
    void onProcess();
    void onAction(QModelIndex);

    void onSetProperty();
    void onGetProperty();
    void onAddWatch();
};

#endif // DIALOG_H
