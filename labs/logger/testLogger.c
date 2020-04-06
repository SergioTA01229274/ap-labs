/*
Author: Sergio Ivan Tostado Nieto
Student ID: A01229274
Advanced Programming

*/


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int infof(char *path, const char *format, ...);
int warnf(const char *caller, char *path, const char *format, ...);
int errorf(const char *caller, char *path, const int Line, const char *format, ...);
int panicf(const char *caller, char *path, const int Line, const char *format, ...);

int main() {
	char *path_buf;
	path_buf = (char *)malloc(sizeof(char)*1000);
	getcwd(path_buf, sizeof(char) * 1000);
	infof(path_buf, "Testing infof%s", " ");
	warnf(__func__, path_buf, "Testing warnf");
	errorf(__func__, path_buf, __LINE__, "Testing errorf%s", " ");
	panicf(__func__, path_buf, __LINE__, "A terrible error has happened and it cannot be fixed%s", " ");
	printf("Proving panicf doesn't get to this point%s", "\n");
    return 0;
}
