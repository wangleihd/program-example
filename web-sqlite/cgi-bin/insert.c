#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "cgi.h"

int main()
{
     int rc;
     char *sql, *err;
     sqlite3 *db;

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
	  cgi_include("insert.html");
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
     sql = sqlite3_mprintf("insert into student values(%Q, %Q, %s, %s, %s)", 
				  cgi_param("Name"), cgi_param("Gender"), 
				  cgi_param("ScoreA"), cgi_param("ScoreB"), cgi_param("ScoreC"));
     rc = sqlite3_exec(db, sql, 0, 0, &err);
     sqlite3_free(sql);
     if( rc!=SQLITE_OK ){
	  printf("<p>[SQL错误] %s</p></body></html>", err);
	  sqlite3_free(err);
	  sqlite3_close(db);
	  cgi_end();
	  return 1;
     }
     puts("<p>插入成功</p></body></html>");
     sqlite3_free(err);
     sqlite3_close(db);
     cgi_end();
     return 0;
}
