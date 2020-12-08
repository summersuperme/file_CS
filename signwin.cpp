#include "signwin.h"
#include "ui_signwin.h"

SignWin::SignWin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignWin)
{
    ui->setupUi(this);
    ui->lineEdit_2->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    ui->lineEdit_3->setEchoMode(QLineEdit::PasswordEchoOnEdit);
}

SignWin::~SignWin()
{
    delete ui;
}

void SignWin::on_pushButton_2_clicked()
{
    QString confpwd;
    username = ui->lineEdit->text();
    passwd = ui->lineEdit_2->text();
    confpwd = ui->lineEdit_3->text();
    //判空
    if(username.isEmpty() || passwd.isEmpty() || confpwd.isEmpty())
    {
        QMessageBox::warning(this,"错误","请输入信息");
        return;
    }
    if(passwd != confpwd)
    {
        QMessageBox::warning(this,"错误","两次密码输入不同!!");
        return;
    }
    emit confirm(username,passwd);
    qDebug()<<"注册验证";
}

void SignWin::on_pushButton_clicked()
{
    emit backlog();
}
