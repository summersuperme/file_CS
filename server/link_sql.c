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
#include "link_sql.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	printf("%d\n",argc);
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

sqlite3* sqlconnect()
{
	sqlite3 *db;
	int rc;
	char databases[20]="user_info";

	rc = sqlite3_open(databases, &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return NULL;
	}

	return db;

}

int sqlexec(sqlite3 *db,char *str)
{
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_exec(db, str, callback, 0, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	return 0;
}

int sqlexec_table(sqlite3 *db,char *str)
{
	char *zErrMsg = 0;
	char **pazResult = NULL;
	int pnRow;           
  	int pnColumn;
	int rc;
	rc = sqlite3_get_table(db, str, &pazResult, &pnRow,&pnColumn, &zErrMsg);
	if( rc!=SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	}
	if(!pnRow && !pnColumn)
		return -1;
	return 0;
}
