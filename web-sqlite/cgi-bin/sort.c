#include <stdio.h>
#include <sqlite3.h>
#include "cgi.h"

int callback(void *p, int argc, char **argv, char **argvv)
{
     int i;
     static int num=1;
     
     printf("<tr><td>%d</td>", num++);
     for(i=0; i<argc; i++)
	  printf("<td>%s</td>", argv[i]? argv[i]: "NULL");
     puts("</tr>");
     return 0;
}

int main()
{
     int rc;
     char *err;
     sqlite3 *db;

     cgi_init();
     cgi_init_headers();

     rc = sqlite3_open("test.db", &db);
     if (rc) {
	  puts("<html><body>数据库连接错误</body></html>");
	  cgi_end();
	  return 1;
     }

     puts("<html><body><table border='1' cellspacing='0'>"
	  "<tr><td>No.</td><td>Name</td><td>Gender</td>"
	  "<td>ScoreA</td><td>ScoreB</td><td>ScoreC</td>"
	  "<td>Sum</td></tr>");

     rc = sqlite3_exec(db, 
		       "select name, gender, scoreA, "
		       "scoreB, scoreC, scoreA+scoreB+scoreC "
		       "from student order by scoreA+scoreB+scoreC desc",
		       callback, 0, &err);
     if( rc!=SQLITE_OK ){
	  printf("</table><p>[SQL错误] %s</p></body></html>", err);
	  sqlite3_free(err);
	  cgi_end();
	  return 1;
     }

     puts("</table></body></html>");
     cgi_end();
     return 0;
}
