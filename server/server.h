/*===============================================================
*   Copyright (C) 2020 All rights reserved.
*   
*   文件名称：server.h
*   创 建 者：supermesummer
*   创建日期：2020年08月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#ifndef SEVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>

int server_init(short port,int backlog);
int server_wait(int fd);
int putfile(char *path,int sfd);
int getfile(char *path,int sfd);
void get_clear(char *buf);
int getlist(int newfd);

#endif
