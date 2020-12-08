#include "widget.h"
#include "ui_widget.h"

#include<qdebug.h>
#include <QListWidget>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    L = new LogWin();
    S = new SignWin();
    connect(this, SIGNAL(sigError(QString)),
            this, SLOT(slotError(QString)));
    connect(this, SIGNAL(sigConnected()),
            this, SLOT(slotConnected()));
    connect(this, SIGNAL(sigDisconnected(QString)),
            this, SLOT(slotDisconnected(QString)));
    connect(this, SIGNAL(sigsuccessdown(QByteArray)),
            this, SLOT(slotsuccessdown(QByteArray)));
    connect(L,SIGNAL(siglogin(QString,QString)),
            this,SLOT(slotlogin(QString,QString)));
    connect(L,SIGNAL(sigsignup(QString,QString)),
            this,SLOT(slotsigsignup(QString,QString)));
    connect(L,SIGNAL(BackToWin()),
            this,SLOT(slotBackToWin()));
    ui->lineEdit->setText("192.168.1.155");
}

Widget::~Widget()
{
    delete ui;
    delete L;
    delete S;
}

/*
 * 文档显示函数
 * 作用:用于在textEdit显示文件内容
*/
void Widget::on_display_clicked()
{
    ui->textEdit->setReadOnly(true);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("ALL files(*.*);;Images (*.png *.xpm *.jpg)"));
    qDebug()<<fileName<<endl;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::about(this, "文件", "文件打开失败");
    }
    QTextStream txtInput(&file);
    QString lineStr;
    lineStr = txtInput.readAll();
    qDebug()<<lineStr.length();
    ui->textEdit->setText(lineStr);
    file.close();
}

/*
 * 文件下载函数
 * 作用:下载文件并保存
*/
void Widget::on_download_clicked()
{
    ui->listWidget->clear();
    if(!getisExit())
    {
        disconnection();
        QMessageBox::about(this, "链接", "链接断开");
    }else{
        download();
    }

}
/*
 * 槽函数,下载成功后触发信号
*/
void Widget::slotsuccessdown(QByteArray arr)
{
    QFile file;
    QString fileName;
    fileName = getfilename();
    file.setFileName("/" + fileName);

    if(file.exists())
    {
        QMessageBox::about(this, "文件", "文件已经存在");
    }else{
        QMessageBox::about(this, "文件", "为您创建文件");

        file.open(QIODevice::WriteOnly | QIODevice::Text);
        file.write(arr);
        file.close();
    }
    QTextStream txtInput(&file);
    QString lineStr;
    QString text;
    file.close();
}
void Widget::slotError(QString str)
{
    QString str1 = str;
    qDebug()<<str;
}
void Widget::slotConnected()
{
    ui->link->setText("退出");
    QMessageBox::about(this,"连接","恭喜连接成功!!");
    ui->link->setEnabled(true);
}
void Widget::slotDisconnected(QString str)
{
    QMessageBox::about(this,"连接",str);
}
/*
 * 链接函数
 * 作用触发链接函数
*/
void Widget::on_link_clicked()
{
    ui->link->setEnabled(false);
    ui->lineEdit->setEnabled(false);
    if("登录" == ui->link->text())
    {
        ui->link->setText("退出");
        ui->link->setEnabled(true);
        //获取文本的网络地址
        QString ip = ui->lineEdit->text();
        setip(ip);
        if(!connection()){
            QMessageBox::about(this, "链接", "断开链接");
        }
        this->hide();
        L->show();
    }else{
        ui->link->setText("登录");
        ui->link->setEnabled(true);
        ui->lineEdit->setEnabled(true);
        //断开链接
        disconnection();
        ui->listWidget->clear();
    }

}

void Widget::on_upload_clicked()
{
    int flag=0;
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("ALL files(*.*);;Images (*.png *.xpm *.jpg)"));
    QFile file(fileName);
    qDebug()<<fileName.split("/")[fileName.split("/").length()-1]<<endl;
    QString arr("put "+ fileName.split("/")[fileName.split("/").length()-1] + "\n");
    s.write(arr.toUtf8().data(),arr.length());
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::about(this, "文件", "文件打开失败");
    }
    QTextStream txtInput(&file);
    QString lineStr;
    QString text;
    qint64 len;
    if(s.waitForReadyRead(30000))
    {
        s.read(reinterpret_cast<char *>(&flag), 4);
        if(flag)
        {
                lineStr = txtInput.readAll(); //读取数据
                len = lineStr.length();
                s.write(reinterpret_cast<char *>(&len),4);
                s.write(lineStr.toUtf8().data(),lineStr.length());
        }else{
            disconnection();
        }
    }else{
        QMessageBox::about(this,"连接","连接断开");
    }
    QMessageBox::about(this,"上传","文件上传成功");
}

bool Widget::connection()
{
    s.connectToHost(QHostAddress(ip),6666);
    if(s.waitForConnected(3000))
    {
        isExit = true;
        emit slotConnected();
    }else{
        s.close();
        s.disconnectFromHost();
        return false;
    }
    return true;
}
/*
  * 文件列表下载
  * 作用:向客户端请求下载列表
*/
void Widget::download()
{
    ui->textEdit->clear();
    s.write("ls\n",3);
    //初始化一个size大的数组为0
    QByteArray arr(1024,0);
    //得到数组缓存首地址
    char *p = arr.data();
    if(s.waitForReadyRead(~0))
    {
        qDebug()<<"第"<<__LINE__<<"被执行";
        QString info;
        int len;
        if(s.bytesAvailable()>=4){
            //从接收缓存读出4字节
            s.read(reinterpret_cast<char *>(&len), 4);
        }else{
            qDebug("服务器没有按照协议来");
            return;
        }
        while (len) {
            if(!s.waitForReadyRead(30000)){
                info = "连接断开：" + s.errorString();
                emit sigError(info);
                break;
            }
            //从接收缓存读出数据
            //想读size，实际读出的字节数num
            qint64 num = s.read(p, len);
            len -= num;
            p += num;
        }
    }
    QString file = arr;
    QStringList Qlist;
    file.split(" ");
    for(int i=0;i<file.split(" ").length();i++)
    {
        Qlist<<QString(file.split(" ")[i]);
    }
    ui->listWidget->addItems(Qlist);

}
void Widget::disconnection()
{
    s.write("quit\n",5);
    qDebug()<<"第"<<__LINE__<<"行被执行";
    s.close();
    s.disconnectFromHost();
}


void Widget::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<item->text();
    setfilename(item->text());
    QString filename(item->text());
    QString Qstr("get " + filename + "\n");
    qint64 num=s.write(Qstr.toUtf8().data(),Qstr.length());
    qDebug()<<num;
    QString info;
    //阻塞等是否接收缓存有数据
    //TRUE：有数据；FASLE：连接断开
    if(s.waitForReadyRead(3000)){
        int len;
        //判断接收缓存是否有4字节
        if(s.bytesAvailable()>=4){
            //从接收缓存读出4字节
            s.read(reinterpret_cast<char *>(&len), 4);
        }else{
            qDebug("服务器没有按照协议来");
            info = "服务器乱发数据";
            return;
        }

        int size = len;
        qDebug()<<size;
        //初始化一个size大的数组为0
        QByteArray arr(size,0);
        //得到数组缓存首地址
        char *p = arr.data();
        while(size){
            //阻塞等是否接收缓存有数据
            if(!s.waitForReadyRead(~0)){
                info = "连接断开：" + s.errorString();
                emit sigError(info);
                break;
            }
            //从接收缓存读出数据
            //想读size，实际读出的字节数num
            qint64 num = s.read(p, size);
            size -= num;
            p += num;
        }
        emit sigsuccessdown(arr);
    }else{
        info = "连接断开：" + s.errorString();
        return;
    }
}

void Widget::slotlogsuccessed()
{
    qDebug()<<"登录成功"<<endl;
}

void Widget::slotlogin(QString username,QString passwd)
{
    int OK;
    QString info("log" + username + ';' + passwd + "\n");
    qDebug()<<info<<endl;
    s.write(info.toUtf8().data(),info.length());
    if(s.waitForReadyRead(30000))
    {
        s.read(reinterpret_cast<char *>(&OK),4);
        if(OK == 1)
        {
            L->hide();
            QMessageBox::information(this,"登录状态","登录成功!");
            this->show();
        }else{
            QMessageBox::information(this,"登录状态","用户名或者密码错误!");
        }
    }
}
void Widget::slotsigsignup(QString username,QString passwd)
{
    int OK;
    QString info("sign" + username + ';' + passwd + "\n");
    s.write(info.toUtf8().data(),info.length());
    if(s.waitForReadyRead(30000))
    {
        s.read(reinterpret_cast<char *>(&OK),4);
        if(OK == 1)
        {
            L->s->hide();
            L->show();
            QMessageBox::information(this,"注册状态","注册成功!");
        }else{
            QMessageBox::information(this,"注册状态","用户名已被注册");
        }
    }
}

void Widget::slotBackToWin()
{
    L->s->hide();
    L->show();
}
