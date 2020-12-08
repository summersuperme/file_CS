#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidgetItem>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>
#include <QtEndian>
#include <QPixmap>
#include <QFile>
#include "logwin.h"
#include "signwin.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public:
    void setfilename(QString filename)
    {
        this->creatfile = filename;
    }
    QString getfilename() const
    {
        return this->creatfile;
    }
    void setisExit(bool value)
    {
        this->isExit = value;
    }
    bool getisExit()
    {
        return this->isExit;
    }
    void setip(QString &ip)
    {
        this->ip = ip;
    }
    bool connection();
    void disconnection();
    void download();
    void upload();
signals:
    void sigError(QString err);
    void sigConnected(QByteArray);
    void sigDisconnected(QString);
    void sigsuccessdown(QByteArray);

private slots:
    void on_display_clicked();
    void slotError(QString str);
    void slotConnected();
    void slotDisconnected(QString str);
    void on_download_clicked();
    void slotsuccessdown(QByteArray arr);
    void slotlogin(QString username,QString passwd);//不能传引用
    void slotsigsignup(QString username,QString passwd);
    void on_link_clicked();
    void on_upload_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void slotlogsuccessed();
    void slotBackToWin();
private:
    Ui::Widget *ui;
    QString ip;
    bool isExit;
    QMutex mutex;
    QTcpSocket s;
    QString creatfile;
    LogWin *L;
    SignWin *S;
};
#endif // WIDGET_H
