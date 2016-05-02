#include <stdio.h>
#include <sqlite3.h>
#include "cgi.h"

int callback(void *p, int argc, char **argv, char **argvv)
{
     *(int *)p = 0;
     return 0;
}

int main()
{
     int rc;
     char *sql, *err;
     sqlite3 *db;
     int empty = 1;

     cgi_init();
     cgi_session_start();
     cgi_process_form();
     cgi_init_headers();

     if(!cgi_session_var_exists("Name")){
	  puts("<html><body>请先登录</body></html>");
	  cgi_end();
	  return 0;
     }

     if (!cgi_param("Name")) {
	  cgi_include("update.html");
	  cgi_end();
	  return 0;
     }

     rc = sqlite3_open("test.db", &db);
     if (rc) {
	  puts("<html><body>数据库连接错误</body></html>");
	  cgi_end();
	  return 1;
     }
     puts("<html><body>");
     sql = sqlite3_mprintf("select * from student where name=%Q", cgi_param("NameToUpdate"));
     rc = sqlite3_exec(db, sql, callback, &empty, &err);
     sqlite3_free(sql);
     if (rc != SQLITE_OK) {
	  printf("<p>[SQL错误] %s</p></body></html>", err);
	  sqlite3_free(err);
	  cgi_end();
	  return 1;
     }
     sqlite3_free(err);
     if (empty) {
	  printf("<p>姓名%s不存在</p></body></html>", cgi_param("NameToUpdate"));
	  cgi_end();
	  return 1;
     }
     sql = sqlite3_mprintf("update student set name=%Q, "
			   "gender=%Q, scoreA=%s, scoreB=%s, "
			   "scoreC=%s where name=%Q",
			   cgi_param("Name"), cgi_param("Gender"), 
			   cgi_param("ScoreA"), cgi_param("ScoreB"), 
			   cgi_param("ScoreC"), cgi_param("NameToUpdate"));
     rc = sqlite3_exec(db, sql, 0, 0, &err);
     sqlite3_free(sql);
     if (rc != SQLITE_OK){
	  printf("<p>[SQL错误] %s</p></body></html>", err);
	  sqlite3_free(err);
	  sqlite3_close(db);
	  cgi_end();
	  return 1;
     }
     puts("<p>更新成功</p></body></html>");
     sqlite3_free(err);
     sqlite3_close(db);
     cgi_end();
     return 0;
}
