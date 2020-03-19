#include <stdlib.h>

int mystrlen(char *str){
	int i = 0;
	if(str == NULL){
		return -1;
	}
	while(*(str + i) != '\0'){
		i++;
	}
	return i;
}

char * mystradd(char *origin, char *addition){
	if(origin == NULL || addition == NULL){
		return '\0';
	}
	int i = 0;
	int j = 0;
	while(*(origin + i) != '\0'){
		i++;
	}
	while(*(addition + j) != '\0'){
                j++;
        }
	char *res = (char *)malloc(sizeof(char) * (i + j + 1));
	int k = 0;
	while(k < i){
		res[k] = *(origin + k);
		k++;
	}
	k = 0;
        while(k < j){
                res[k + i] = *(addition + k);
                k++;
        }
	res[i + j] = '\0';
	return res;
}

int mystrfind(char *origin, char *substr){
	int or_ptr = 0;
	int sub_ptr = 0;
	int flag = 1;
	while(*(origin + or_ptr) != '\0'){
		if(*(origin + or_ptr) == *(substr)){
			flag = 1;
			while(*(substr + sub_ptr) != '\0' && *(origin + or_ptr) != '\0'){
				if(*(substr + sub_ptr) != *(origin + or_ptr)){
					flag = 0;
					break;
				}
				sub_ptr++;
				or_ptr++;
			}
		}
		or_ptr++;
	}
	return (flag)? 1:0;
}
