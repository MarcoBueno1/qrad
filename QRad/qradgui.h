#ifndef QRADGUI_H
#define QRADGUI_H

#include <QDialog>

namespace Ui {
class QRadGui;
}

class QRadGui : public QDialog
{
    Q_OBJECT

public:
    explicit QRadGui(QWidget *parent = 0);
    ~QRadGui();

private:
    Ui::QRadGui *ui;
};

#endif // QRADGUI_H
