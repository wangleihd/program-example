#include <stdio.h>
#include <sqlite3.h>
#include "cgi.h"

int main()
{
     int rc;
     char *sql;
     sqlite3 *db;
     char **resultp;
     int rowcount;

     cgi_init();
     cgi_session_start();
     cgi_process_form();
     cgi_init_headers();
     
     if(cgi_session_var_exists("Name")){
	  puts("<html><body>您已登录，请先注销</body></html>");
	  cgi_end();
	  return 0;
     }

     if (!cgi_param("Name")) {
	  cgi_include("login.html");
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
     sql = sqlite3_mprintf("select * from login where name = %Q", cgi_param("Name"));
     rc = sqlite3_get_table(db, sql, &resultp, &rowcount, 0, 0);
     sqlite3_free(sql);
     if( rc!=SQLITE_OK ){
	  printf("<p>[SQL错误] %s</p></body></html>", sqlite3_errmsg(db));
	  sqlite3_close(db);
	  cgi_end();
	  return 1;
     }
     if(rowcount == 0)
	  puts("用户名不存在，请重新登录！");
     else if(strcmp(cgi_param("Passwd"), resultp[3])!=0)
	  puts("密码错误，请重新登录！");
     else {
	  cgi_session_register_var("Name", resultp[2]);
	  puts("登录成功！");
     }
     puts("</body></html>");
     sqlite3_free_table(resultp);
     sqlite3_close(db);
     cgi_end();
     return 0;
}
