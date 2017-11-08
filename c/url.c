#include <stdio.h>

int main(void){
    char ch;
    while((ch = getchar()) != EOF) {
        if(ch == ' '){
            printf("%%20");
        } else {
            putchar(ch);
        }
    }
    return 0;
}
