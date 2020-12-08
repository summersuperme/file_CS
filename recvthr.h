#ifndef RECVTHR_H
#define RECVTHR_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>
#include <QtEndian>
#include <QPixmap>
#include <QFile>

class RecvThr :public QThread
{
    Q_OBJECT
public:
    explicit  RecvThr(QObject *parent = nullptr):
        QThread(parent)
    {}
public:
    void setnetip(QString ip)
    {
        this->ip = ip;
    }
    void exitThr(){
        QMutexLocker locker(&mutex);
        isExit = false;
    }
    void setstate(int sta)
    {
        this->state=sta;
    }
    void getfile(QString &file)
    {
        this->filename=file;
    }
signals:
    void sigError(QString err);
    void sigConnected(QByteArray);
    void sigDisconnected(QString);
    void sigsuccessdown(QByteArray);

private:
    void run(){
        //创建套接字文件
        QTcpSocket s;
        //连接网络
        s.connectToHost(QHostAddress(ip),6666);
        //阻塞等待连接成功
        if(s.waitForConnected(~0))
        {
            isExit = true;
            QString info;
            //初始化一个size大的数组为0
            /*QByteArray arr(1024,0);
            //得到数组缓存首地址
            char *p = arr.data();
            if(s.waitForReadyRead(~0))
            {
                qDebug()<<"第"<<__LINE__<<"被执行";
                qint64 num;
                while (1) {
                    if((num = s.read(p,1024)))
                    {
                        qDebug()<<num;
                        p += num;
                    }else{
                        break;
                    }
                }
            }
            qDebug()<<arr.data();
            emit sigConnected(arr);*/
            qDebug()<<"第"<<__LINE__<<"被执行";
            while(1)
            {
                //是否结束线程
                mutex.lock();
                if(!isExit)
                {
                    mutex.unlock();
                    info = "线程退出";
                    qDebug()<<info;
                    QString Qstr("quit\n");
                    qint64 num1=s.write("quit\n",Qstr.length());
                    qDebug()<<num1;
                    break;
                }
                mutex.unlock();

                if(state == 1)
                {
                    QString Qstr("get file_server.c\n");
                    qint64 num=s.write("get file_server.c\n",Qstr.length());
                    qDebug()<<num;
                    download(s);
                    state = -1;
                }
                if(state == 2)
                {
                    upload(s);
                    state = -1;
                }
                if(state == 3)
                {
                    this->exitThr();
                    state = -1;
                }
            }
        }else{
            qDebug()<<s.errorString();
        }
    }

    void upload(QTcpSocket &s)
    {
        qDebug()<<"开始上传";
        return;
    }
    void downlist(QTcpSocket &s)
    {
        /*QString info;
                //阻塞等是否接收缓存有数据
                //TRUE：有数据；FASLE：连接断开
                if(s.waitForReadyRead(~0)){
                    qDebug()<<"第"<<__LINE__<<"被执行";
                    int len;
                    //判断接收缓存是否有4字节
                    if(s.bytesAvailable()>=4){
                        //从接收缓存读出4字节
                        s.read(reinterpret_cast<char *>(&len), 4);
                        //从大端转到本地字节序：网络字节序就是大端
                        len = qFromBigEndian(len);
                    }else{
                        qDebug("服务器没有按照协议来");
                        info = "服务器乱发数据";
                        return;
                    }

                    int size = len;
                    //初始化一个size大的数组为0
                    QByteArray arr(size,0);
                    //得到数组缓存首地址
                    char *p = arr.data();
                    while(size){
                        //阻塞等是否接收缓存有数据
                        if(!s.waitForReadyRead(~0)){
                            info = "连接断开：" + s.errorString();
                            emit sigError(info);
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
                }*/
        qDebug()<<"列表";
        return;
    }
    void download(QTcpSocket &s)
    {
        QString info;
        //阻塞等是否接收缓存有数据
        //TRUE：有数据；FASLE：连接断开
        if(s.waitForReadyRead(3000)){
            qDebug()<<"第"<<__LINE__<<"被执行";
            int len;
            //判断接收缓存是否有4字节
            if(s.bytesAvailable()>=4){
                //从接收缓存读出4字节
                s.read(reinterpret_cast<char *>(&len), 4);
                //从大端转到本地字节序：网络字节序就是大端
                /*len = qFromBigEndian(len);
                qDebug()<<"len:"<<len;*/
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
private:
    QString ip;
    bool isExit;
    QMutex mutex;
    int state;
    QString filename;
};
#endif // RECVTHR_H

