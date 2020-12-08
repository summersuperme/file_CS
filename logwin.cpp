#include "logwin.h"
#include "ui_logwin.h"

LogWin::LogWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWin)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    s = new SignWin();
    connect(s,SIGNAL(confirm(QString,QString)),
            this,SLOT(slotconfirm(QString,QString)));
    connect(s,SIGNAL(backlog()),
            this,SLOT(slotbacklog()));
}

LogWin::~LogWin()
{
    delete ui;
    delete s;
}

void LogWin::on_pushButton_clicked()
{
    QString username = ui->lineEdit->text();
    QString passwd = ui->lineEdit_2->text();
    emit siglogin(username,passwd);
    qDebug()<<"验证登录";
}

void LogWin::on_pushButton_2_clicked()
{
    this->hide();
    s->show();
}

void LogWin::slotconfirm(QString username,QString passwd)
{
    emit sigsignup(username,passwd);
}

void LogWin::slotbacklog()
{
    emit BackToWin();
}


