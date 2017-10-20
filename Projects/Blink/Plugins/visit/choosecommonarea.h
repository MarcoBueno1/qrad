#ifndef CHOOSECOMMONAREA_H
#define CHOOSECOMMONAREA_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class ChooseCommonArea;
}

class ChooseCommonArea : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseCommonArea(QWidget *parent = 0);
    ~ChooseCommonArea();

public slots:
    void doReserve(bool);
private:
    Ui::ChooseCommonArea *ui;
    QSqlQueryModel *m_model;
};

#endif // CHOOSECOMMONAREA_H
