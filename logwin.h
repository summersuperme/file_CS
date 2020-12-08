#ifndef LOGWIN_H
#define LOGWIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "signwin.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>
namespace Ui {
class LogWin;
}

class LogWin : public QWidget
{
    Q_OBJECT

public:
    explicit LogWin(QWidget *parent = nullptr);
    ~LogWin();
signals:
    void siglogin(QString,QString);
    void sigsignup(QString,QString);
    void BackToWin();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void slotconfirm(QString username,QString passwd);
    void slotbacklog();
private:
    Ui::LogWin *ui;
public:
    SignWin *s;
};

#endif // LOGWIN_H
