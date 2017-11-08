#include <stdio.h>

int main(void){
	int data[100] = {1};
	int n = 10;
	int i, j, k;
	for(i = 0; i < n; i++){
		for(k = i; k >= 1; k-- )
			data[k] += data[k - 1];
		for(j = 0; j <= i; j++){
			printf("%d ", data[j]);
		}
		printf("\n");
	}



	return 0;
}
