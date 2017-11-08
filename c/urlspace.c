#include <stdio.h>
#include <stdlib.h>

int main(void){
    char ch;
    int s = 0;
    while((ch = getchar()) != EOF) {
    //    printf("s = %d\tch=%c\n", s, ch );
        if(s == 0){
            if(ch == '%'){
                s = 1;
            } else {
                putchar(ch);
            }
        }
        else if(s == 1){
            if(ch == '2'){
                s = 2;
    //            printf("%s\n", "stat = 2");
            } else {
                putchar('%');
                putchar(ch);
                s = 0;
            }
        }
        else if(s == 2){
            if(ch == '0'){
                putchar(' ');
            } else {
                putchar('%');
                putchar('2');
                putchar(ch);
            }
            s = 0;
        } else {
            printf("wrong state !\n");
            exit(1);
        }
    }
    return 0;
}
