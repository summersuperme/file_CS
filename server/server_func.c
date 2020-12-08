/*===============================================================
*   Copyright (C) 2020 All rights reserved.
*   
*   文件名称：server_func.c
*   创 建 者：supermesummer
*   创建日期：2020年08月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include "server.h"
#define N 2048
int OK=1;
int server_init(short port,int backlog)
{
	int fd;
	if(-1 == (fd = socket(AF_INET,SOCK_STREAM,0)))
	{
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("0.0.0.0");

	if(-1 == bind(fd,(void *)&addr,sizeof(addr)))
	{
		perror("bind");
		exit(-1);
	}
	if(-1 == listen(fd,backlog))
	{
		perror("listen");
		exit(-1);
	}
	printf("listening...\n");
	return fd;
}

int server_wait(int fd)
{
	struct sockaddr_in cddr;
	int len = sizeof(cddr);
	bzero(&cddr,len);
	int newfd;
	newfd  = accept(fd,(void *)&cddr,&len);
	if(-1 == newfd)
	{
		perror("accept");
		exit(-1);
	}
	printf("用户%s链接成功\n",inet_ntoa(cddr.sin_addr));
	return newfd;
}
void get_clear(char *buf)
{
        int i = 0;
        while(1)
        {
                if(*(buf+i) == '\n')
                {
                        *(buf+i) = '\0';
                        break;
                }
                i++;
        }
}
int getfile(char *path,int sfd)
{
        int fd = open(path,O_RDONLY);
        int ret,len,num=0;
        float pro;
        if(fd < 0)
        {
                perror("getfile open");
                return -1;
        }
        len = lseek(fd,0,SEEK_END);
        printf("下载的文件大小为：%d\n",len);
        write(sfd,&len,sizeof(len));
        lseek(fd,0,SEEK_SET);
        char buf[N] = {0};
        while((ret = read(fd,buf,N-1)))
        {
                num += ret;
                printf("%.2f\%\n",(float)num/(float)len*100);
                write(sfd,buf,ret);
                bzero(buf,sizeof(buf));
        }
        printf("下载完成！\n");
        close(fd);
        return 0;
}
int putfile(char *path,int sfd)
{
        printf("%s\n",path);
        int fd = open(path,O_CREAT | O_RDWR,0664);
        int ret,len=0,num=0;
        if(fd < 0)
        {
                perror("putfile open");
                return -1;
        }
        char buf[N] = {0};
        write(sfd,&OK,4);
        read(sfd,&len,4);
        printf("%s文件大小为：%d\n",path,len);
        float pro;
        while(1)
        {
                ret = read(sfd,buf,N-1);
                write(fd,buf,ret);
                num += ret;
                pro = (float)num/(float)len;
                printf("%.2f%\n",pro*100);
                bzero(buf,sizeof(buf));
                if(num >= len)
                        break;
        }
        printf("上传成功!\n");
        close(fd);
        return 0;
}
int getlist(int newfd)
{
        DIR *dir;
        if(NULL ==(dir = opendir(".")))
        {
                perror("opendir");
                return -1;
        }
        struct dirent *dirp;
        char buf[1024]={0};
        char *p;
        int len=0;
        p = buf;
        while((dirp = readdir(dir)))
        {
                if(8 == dirp->d_type){
                        sprintf(p,"%s ",dirp->d_name);
                        p += (strlen(dirp->d_name)+1);
                        len += (strlen(dirp->d_name)+1);
                }
        }
        printf("%d\n",len);
        printf("%s\n",buf);
        write(newfd,&len,4);
        write(newfd,buf,len);
        closedir(dir);
        return 0;

}

