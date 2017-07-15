#ifndef EMAILGUI_H
#define EMAILGUI_H

#include <QDialog>

namespace Ui {
class EmailGui;
}

class EmailGui : public QDialog
{
    Q_OBJECT

public:
    explicit EmailGui(QWidget *parent = 0);
    ~EmailGui();
    void setText(QString text);
    void setTo(QString to, bool enabled =false);
    void setSubject(QString Subject, bool enabled = false);
    void setFile(QString file);

    QString getText();

public slots:
    void doLink(const QString &link);
private:
    Ui::EmailGui *ui;
};

#endif // EMAILGUI_H
