/*===============================================================
 *   Copyright (C) 2020 All rights reserved.
 *   
 *   文件名称：link_sql.c
 *   创 建 者：supermesummer
 *   创建日期：2020年11月04日
 *   描    述：
 *
 *   更新日志：
 *
================================================================*/
#ifndef LINK_SQL_H
#define LINK_SQL_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <sqlite3.h>
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
sqlite3* sqlconnect();
int sqlexec(sqlite3 *db,char *sqlstr);
int sqlexec_table(sqlite3 *db,char *sqlstr);
#endif
