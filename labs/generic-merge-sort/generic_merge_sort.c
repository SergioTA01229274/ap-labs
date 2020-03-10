/*
Author: Sergio Ivan Tostado Nieto
Student ID: A01229274
Generic MergeSort activity

*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int numcmp(char *str1, char *str2){
	int num1, num2;
	num1 = atoi(str1);
	num2 = atoi(str2);
	if (num1 > num2){
		return 1;
	}else if (num1 < num2){
		return -1;
	}else{
		return 0;
	}
}

void merge(char **elements, int first, int last, int (*comp)(char *, char*), int size){
	char **temp = (char **) malloc(sizeof(char *) * size);
	for(int i = first; i <= last;i++){
		temp[i] = elements[i];
	}
	int punt1 = first;
	int punt2 = ((first + last)/2) + 1;
	int punt3 = first; 
	while(punt1 <= ((first + last)/2) && punt2 <= last) {
		if(comp(temp[punt1], temp[punt2]) <= 0){
			elements[punt3] = temp[punt1];
			punt1++;
		}else {
			elements[punt3] = temp[punt2];
			punt2++;
		}
		punt3++;
	}
	while(punt1 <= ((first + last)/2)) {
		elements[punt3] = temp[punt1];
		punt1++;
		punt3++;
	}
	free(temp);
}


void mergeSrt(char **arr, int first, int last, int (*comp)(char *, char *), int size){
	if (first < last){
		int mid = (first + last)/2;
		mergeSrt(arr, first, mid, comp, size);
		mergeSrt(arr, mid + 1, last, comp, size);
		merge(arr, first, last, comp, size);
	}
}


int main (int argc, char **argv){
	int numeric = 0;
	if (argc > 1 && strcmp(argv[1], "-n") == 0){
		numeric = 1;
	}
	mergeSrt(argv + 1 + numeric, 0, argc -(numeric ? 3:2), numeric ? numcmp:strcmp, argc - (numeric ? 2:1));
	for(int i = 1 + numeric; i < argc; i++){
                printf("%s ", argv[i]);
       }
        printf("%s", "\n");
	return 0;
}
