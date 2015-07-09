#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char newtime[9] = "20150601";
    char y[5] = {0}, m[3] = {0}, n[3] = {0};
    time_t	 timep;
    struct tm *p;
    int year, mon, date;
    int sys_year, sys_mon, sys_date;
    int totle;

    time(&timep);
    p = gmtime(&timep);
    printf("%d%d%d\n", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);
    //printf("%s %d:%d:%d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
    sys_year = (1900+p->tm_year);
    sys_mon  = (1+p->tm_mon);
    sys_date = p->tm_mday;

    memcpy(y, newtime, 4);
    year = atoi(y);

    memcpy(m, newtime + 4, 2);
    mon = atoi(m);

    memcpy(n, newtime + 6, 2);
    date = atoi(n);

    totle = (sys_year - year - 2000) * 365 + (sys_mon - mon) * 30 + (sys_date - date);

    if(totle < 0){
    	printf("totle = -1\n");
    }else{
    	printf("totle = %d\n", totle);
    }


    printf("new = %s \t atoi = %d\n", y, year);
    printf("new = %s \t atoi = %d\n", m, mon);
    printf("new = %s \t atoi = %d\n", n, date);
    return 0;
}