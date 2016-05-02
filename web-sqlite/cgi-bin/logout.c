#include <stdio.h>
#include "cgi.h"

int main(int argc, char **argv)
{
     cgi_init();
     cgi_session_start();
     cgi_init_headers();

     if (cgi_session_var_exists("Name"))
	  cgi_session_unregister_var("Name");
     puts("<html><body>已注销</body></html>");
     cgi_end();
     return 0;
}
