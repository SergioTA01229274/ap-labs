/*
Author: Sergio Ivan Tostado nieto
Student ID: A01229274
Advanced Programming
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>



#define RESET		0
#define DIM		2

#define BLACK		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define WHITE		7



void textColor(int attr, int fg, int bg){
	char command[13];

        sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
        int cont = 0;
        while(*(command + cont) != '\0'){
                putc(*(command + cont), stdout);
                cont++;
        }
}

int infof(char *path, const char *format, ...){
	textColor(RESET, WHITE, BLACK);

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dic"};

	int i = 0;
	while(*(path + i) != '\0'){
		i++;
	}

	char output[i + 29];
	sprintf(output, "%s\t%d %d:%d:%d %s Message: [ ", months[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, path);
	output[i + 28] = '\0';
	int j = 0;
	while(*(output + j) != '\0'){
		putc(*(output + j), stdout);
		j++;
	}
	va_list arg;
	va_start (arg, format);
	vfprintf (stdout, format, arg);
	va_end (arg);
	putc(']', stdout);
	putc('\n', stdout);
	textColor(RESET, WHITE, BLACK);
	return 0;
}


int warnf(const char *caller, char *path, const char *format, ...){
        textColor(RESET, BLACK, YELLOW);
	time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dic"};
	int i = 0;
        while(*(path + i) != '\0'){
                i++;
        }
	int k = 0;
	while(*(caller + k) != '\0'){
		k++;
	}


        char output[i + 50 + k];
        sprintf(output, "%s\t%d %d:%d:%d %s Message: Warning at method [%s] [ ", months[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, path, caller);
        output[i + k + 49] = '\0';
        int j = 0;
        while(*(output + j) != '\0'){
                putc(*(output + j), stdout);
                j++;
        }
        va_list arg;
        va_start (arg, format);
	int out;
        out = vfprintf (stdout, format, arg);
        va_end (arg);
	putc(']', stdout);
        textColor(RESET, WHITE, BLACK);
	putc('\n', stdout);
        return (out == 0)? 1:out;
}

int errorf(const char *caller, char *path, const int Line, const char *format, ...){
        textColor(RESET, RED, BLACK);
	time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dic"};
        int i = 0;
        while(*(path + i) != '\0'){
                i++;
        }
        int k = 0;
        while(*(caller + k) != '\0'){
                k++;
        }


        char output[i + k + 69];
        sprintf(output, "%s\t%d %d:%d:%d %s Message: Error reported at method [%s] in line [%d] [ ", months[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, path, caller, Line);
        output[i + k + 68] = '\0';
        int j = 0;
        while(*(output + j) != '\0'){
                putc(*(output + j), stdout);
                j++;
        }

	int out;
        va_list arg;
        va_start (arg, format);
        out = vfprintf (stdout, format, arg);
        va_end (arg);
	putc(']', stdout);
        textColor(RESET, WHITE, BLACK);
        putc('\n', stdout);
        return (out == 0)? 2:out;
}

void panicf(const char *caller, char *path, const int Line, const char *format, ...){
        textColor(DIM, BLACK, RED);
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dic"};
        int i = 0;
        while(*(path + i) != '\0'){
                i++;
        }
        int k = 0;
        while(*(caller + k) != '\0'){
                k++;
        }


        char output[i + k + 73];
        sprintf(output, "%s\t%d %d:%d:%d %s Message: Fatal Error occured at method [%s] in line [%d] [ ", months[tm.tm_mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, path, caller, Line);
        output[i + k + 72] = '\0';
        int j = 0;
        while(*(output + j) != '\0'){
                putc(*(output + j), stdout);
                j++;
        }
        int out;
        va_list arg;
        va_start (arg, format);
        out = vfprintf (stdout, format, arg);
        va_end (arg);
	putc(']', stdout);
        textColor(RESET, WHITE, BLACK);
        putc('\n', stdout);
        exit(3);
}
