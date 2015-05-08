#include <stdio.h>
#include "sqlite3.h"

//查询的回调函数声明
int select_callback(void * data, int col_count, char ** col_values, char ** col_Name);



int main(int argc, char * argv[])

{

const char * sSQL1 = "create table users(userid int PRIMARY KEY , age int);";
const char * sSQL2 = "create table users2(userid int PRIMARY KEY , age int);";

char * pErrMsg = 0;

int result = 0;

// 连接数据库

sqlite3 * db = 0;

int ret = sqlite3_open("./test.db", &db);

if( ret != SQLITE_OK ) {

fprintf(stderr, "无法打开数据库： %s", sqlite3_errmsg(db));

return(1);

}

printf("数据库连接成功!\n");
int rowID = sqlite3_last_insert_rowid(db);

printf("%d\n",rowID);
// 执行建表SQL

sqlite3_exec( db, sSQL1, 0, 0, &pErrMsg );
sqlite3_exec( db, sSQL2, 0, 0, &pErrMsg );

if( ret != SQLITE_OK ){

fprintf(stderr, "SQL error: %s\n", pErrMsg);

sqlite3_free(pErrMsg);

}

// 执行插入记录SQL
result = sqlite3_exec(db, "begin transaction", 0, 0, &pErrMsg);
printf("事务开始\n");

result = sqlite3_exec( db, "insert into users(userid,age) select 1,2 \
 union all select 2,80   ;", 0, 0, &pErrMsg);
rowID = sqlite3_last_insert_rowid(db);
if(result == SQLITE_OK){

printf("插入数据成功\n");
printf("%d\n",rowID);
}else{
 result = sqlite3_exec(db, "rollback transaction", 0, 0, &pErrMsg);
 fprintf(stderr, "rollback transaction: %s\t %d\n", pErrMsg,result);
 sqlite3_exec(db, "end transaction", 0, 0, &pErrMsg);
 fprintf(stderr, "end transaction: %s\t %d\n", pErrMsg,result);

}

result = sqlite3_exec( db, "insert into users2 values(3,20);", 0, 0, &pErrMsg);
rowID = sqlite3_last_insert_rowid(db);
if(result == SQLITE_OK){

printf("插入数据成功\n");

printf("%d\n",rowID);

}else{
 result = sqlite3_exec(db, "rollback transaction", 0, 0, &pErrMsg);
 fprintf(stderr, "rollback: %s\t %d\n", pErrMsg,result);
}
 result = sqlite3_exec(db, "commit transaction", 0, 0, &pErrMsg);
 fprintf(stderr, "commit: %s\t %d\n", pErrMsg,result);
 result = sqlite3_exec(db, "end transaction", 0, 0, &pErrMsg);
 fprintf(stderr, "end: %s\t %d\n", pErrMsg,result);

// 查询数据表

printf("查询数据库内容\n");

sqlite3_exec( db, "select * from users;", select_callback, 0, &pErrMsg);
sqlite3_exec( db, "select * from users2;", select_callback, 0, &pErrMsg);

// 关闭数据库

sqlite3_close(db);

db = 0;

printf("数据库关闭成功!\n");

return 0;

}

int select_callback(void * data, int col_count, char ** col_values, char ** col_Name)

{

// 每条记录回调一次该函数，有多少条就回调多少次

int i;

for( i=0; i < col_count; i++){
printf( "%s = %s\t", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
}
printf("\n");

return 0;

}
