#include <stdio.h>


int mystrlen(char *str);
char *mystradd(char *origin, char *addition);
int mystrfind(char *origin, char *substr);


int main(int argc, char **argv) {
	if(argc < 4){
		printf("Usage: ./main2 [original str] [str to add] [substring to find]%s", "\n");
		return 1;
	}
	char *str_add = mystradd(argv[1], argv[2]);
	printf("Initial Length: %d\n", mystrlen(argv[1]));
	printf("New String: %s\n", str_add);
	printf("%d\n", mystrfind(str_add, argv[3]));
	return 0;
}
