#include <stdio.h>

int main(void){
    int i, j;
    int n = 3;
    for(i = 0; i < n; i++){
        for(j = 0; j < i; j++ ){
            printf("  ");
        }
        for(j = 0; j < (2 * n - 2 * i - 1); j++)
            printf("* ");
        printf("\n");
    }
    return 0;
}
