#include <stdio.h>
#include <ctype.h>


int main(void){
    char ch;
    char prev = ' ';
    int lines = 0, chars = 0, words = 0;
    while ((ch = getchar()) != EOF) {
        if(ch =='\n'){
            lines++;
        }
        if(isspace(prev) && !isspace(ch)){
            words ++;
        }

        chars ++;
        prev = ch;
    }
    printf("\t%d\t%d\t%d\n", lines, words, chars);


    return 0;

}
