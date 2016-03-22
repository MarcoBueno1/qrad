#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qradlineeditcompleter.h"
#include <QSqlQuery>
#include "qradfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CriaTabela();

    ui->lineEdit->setText(QRadFile::GetParamValue("phonecard/databasename", "config.enc"));

    ui->lineEdit->setSelect("select rg || ' | ' || nome, id, rg, cpf, nome from teste");
    ui->lineEdit->Add(ui->lineEditcpf);
    ui->lineEdit->Add(ui->lineEditnome);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::CriaTabela()
{
    QString str = "create table teste ( id integer, rg character variyng, cpf character variyng, nome character variyng);";
    QSqlQuery *q = new QSqlQuery;
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 1,'1054264393', '62040863087','Marco Antonio Bueno da Silva' )";
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 2,'1055001001', '52014339287', 'Tricia Bueno da Silva' )";
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 3,'1100121231', '12345678901','Miguel Bueno da Silva' )";
    q->exec(str);

    str = "insert into teste ( id, rg, cpf, nome ) values ( 4,'1234567890', '00000000001','Theo Bueno da Silva' )";
    q->exec(str);

    delete q;
}
