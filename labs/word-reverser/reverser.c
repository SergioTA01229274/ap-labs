#include <stdio.h>
#include <stdlib.h>
//Author: Sergio Ivan Tostado Nieto A01229274

void inverse_word(char *word, int len){
        char str_temp[len];
	for(int i = 0; i < len; i++){
                str_temp[i] = word[len - i - 1];
        }
        for(int j = 0; j < len; j++){
                putchar(str_temp[j]);
        }
	printf("\n");
}

int main(){
	int c, n = 0;
	char *str = (char*)calloc(100, sizeof(char));

	while((c = getchar()) != EOF && n < 1000){
		if (c == '\n'){
			inverse_word(str, n);
			n = 0;
			free(str);
			str = (char*)calloc(100, sizeof(char));
			continue;
		}
		str[n] = c;
                n++;

	}
	printf("\n");
	return 0;
}
