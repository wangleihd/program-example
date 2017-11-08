#include <string.h>
#include <stdio.h>

int main(void){
    char buf[4096];
    char *p1, *p2;
    while(fgets(buf,4096,stdin)){
        p1 = buf;
        while(( p2 = strstr(p1,"%20"))){
            *p2='\0';
            printf("%s_",p1);
            p1=p2+3;
        }
        printf("%s",p1);
    }
    return 0;
}
