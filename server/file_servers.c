/*===============================================================
 *   Copyright (C) 2020 All rights reserved.
 *   
 *   文件名称：file_server.c
 *   创 建 者：supermesummer
 *   创建日期：2020年08月20日
 *   描    述：
 *
 *   更新日志：
 *
 ================================================================*/
#include "server.h"
#include "link_sql.h"
#include <pthread.h>
#define N 2048
sqlite3 *db;
int login(sqlite3 *db,char *username,char *passwd);
int signup(sqlite3 *db,char *username,char *passwd);
void getname(const char *buf,char *username,char *passwd);
void *dealpthread(int *fd)
{
	int succeed=1;
	int fail=0;
	int result;
	int newfd = *fd;
	char buf[N];
	char flag[N];
	while(1)
	{
		read(newfd,buf,N);
		get_clear(buf);
		char passwd[50];
		char username[50];
		printf("%s\n",buf);
		if(!strncmp(buf,"log",3))
		{
			getname(buf+3,username,passwd);
			if(!(result = login(db,username,passwd)))
			{
				write(newfd,&succeed,4);
				bzero(buf,sizeof(buf));
				break;
			}else{
				write(newfd,&fail,4);
				bzero(buf,sizeof(buf));
			}
		}
		if(!strncmp(buf,"sign",4))
		{
			getname(buf+4,username,passwd);
			if(!(result = signup(db,username,passwd)))
			{
				write(newfd,&succeed,4);
				bzero(buf,sizeof(buf));
			}else{
				write(newfd,&fail,4);
				bzero(buf,sizeof(buf));
			}
		}
	}
	while(1)
	{
		read(newfd,buf,N);
		get_clear(buf);
		if(!strncmp(buf,"get",3))
		{
			printf("请求成功，正在为您下载%s\n",buf+4);
			getfile(buf+4,newfd);
			continue;
		}
		if(!strncmp(buf,"put",3))
		{
			printf("正在上传文件%s\n",buf+4);
			putfile(buf+4,newfd);
			continue;
		}
		if(!strncmp(buf,"ls",2))
		{
			printf("下载列表已传送\n");
			getlist(newfd);
			continue;
		}
		if(!strncmp(buf,"quit",4))
			break;
		bzero(buf,sizeof(buf));
	}
	return 0;

}
int main(int argc,char *argv[])
{
	db = sqlconnect();
	int fd = server_init(6666,10);
	if(fd == -1)
		return -1;
	while(1)
	{
		int newfd = server_wait(fd);
		if(newfd == -1)
			return 0;
		pthread_t thread;
		int ret = pthread_create(&thread, NULL, (void *)dealpthread, &newfd);
		printf("进入线程！\n");
		if(ret < 0)
		{
			perror("pthread_create");
			exit(-1);
		}
	}
	return 0;
}
int signup(sqlite3 *db,char *username,char *passwd)
{
	char name[50];
	char pwd[50];
	char str[200]={0};
	strcpy(name,username);
	strcpy(pwd,passwd);
	sprintf(str,"SELECT * FROM user_info WHERE username='%s'",name);
	if(!sqlexec_table(db,str))
		return -1;
	bzero(str,sizeof(str));
	sprintf(str,"INSERT INTO user_info VALUES('%s',%s)",name,pwd);
	if(sqlexec(db,str)<0)
		return -1;
	return 0;	
}

int login(sqlite3 *db,char *username,char *passwd)
{
	char name[50];
	char pwd[50];
	char str[200]={0};
	strcpy(name,username);
	strcpy(pwd,passwd);
	sprintf(str,"SELECT * FROM user_info WHERE passwd = %s and username='%s'",pwd,name);
	if(sqlexec_table(db,str) < 0)
	{
		printf("失败！！\n");
		return -1;
	}
	return 0;
}
void getname(const char *buf,char *username,char *passwd)
{
	int i=0,j=0;
	while(*(buf+i) != ';')
	{
		*(username+i) = *(buf+i);
		i++;
	}
	i++;
	while(*(buf+i) != '\0')
	{
		*(passwd+j) = *(buf+i);
		i++;
		j++;
	}
}
