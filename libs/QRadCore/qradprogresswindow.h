#ifndef QRADPROGRESSWINDOW_H
#define QRADPROGRESSWINDOW_H

#include <QDialog>
#include <QApplication>


#ifndef _NO_WIDGETS_
    #define QRAD_SHOW_PRPGRESS(text)\
    QApplication* aApplication = qobject_cast<QApplication*>(QCoreApplication::instance());\
    if (aApplication && aApplication->topLevelWidgets().count() )\
    {\
        QRadProgressWindow *pW = QRadProgressWindow::getInstance();\
        pW->setDetail(QString(text));\
    }
   #define QRAD_SHOW_PRPGRESS_STEP(text)\
    if (aApplication && aApplication->topLevelWidgets().count() )\
    {\
        QRadProgressWindow *pW = QRadProgressWindow::getInstance();\
        pW->setDetail(QString(text));\
    }

//QApplication* aApplication = qobject_cast<QApplication*>(QCoreApplication::instance());


    #define QRAD_HIDE_PRPGRESS()\
    if (aApplication && aApplication->topLevelWidgets().count())\
    {\
        QRadProgressWindow *pW = QRadProgressWindow::getInstance();\
        pW->hide();\
    }
#else
  #define QRAD_SHOW_PRPGRESS(text)
  #define QRAD_HIDE_PRPGRESS()
#endif


namespace Ui {
class QRadProgressWindow;
}

class QRadProgressWindow : public QDialog
{
    Q_OBJECT

public:
    explicit QRadProgressWindow(QWidget *parent = 0);
    ~QRadProgressWindow();

    static QRadProgressWindow* getInstance();
    void setDetail( QString detail );
    void setTitle( QString Title );


private:
    Ui::QRadProgressWindow *ui;
    static QRadProgressWindow* m_instance;

};

#endif // QRADPROGRESSWINDOW_H
