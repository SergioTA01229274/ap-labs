#include "logger.h"
/*
Author: Sergio Ivan Tostado nieto
Student ID: A01229274
Advanced Programming
*/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <syslog.h>


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

int log_flag;


int initLogger(char *logType) {
	printf("Initializing Logger on: %s\n", logType);
	if (strcmp(logType, "stdout") == 0){
		log_flag = 1;
	}else if (strcmp(logType, "syslog")){
		log_flag = 0;
	}else{
		log_flag = 0;
	}
	return 0;
}

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
	if(log_flag){
		time_t t = time(NULL);
        	struct tm tm = *localtime(&t);

        	char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dic"};
       		int i = 0;
        	while(*(path + i) != '\0'){
                	i++;
        	}
        	char *output = (char *) malloc(sizeof(char) * (i + 29));
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
	}else{
                va_list arg;
                va_start (arg, format);
                openlog(path, LOG_CONS | LOG_PID | LOG_ODELAY, LOG_USER);
		vsyslog(LOG_INFO, format, arg);
		closelog();
                va_end (arg);

	}
	textColor(RESET, WHITE, BLACK);
	return 0;
}


int warnf(const char *caller, char *path, const char *format, ...){
        textColor(RESET, BLACK, YELLOW);
	if(log_flag){
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
        	vfprintf (stdout, format, arg);
        	va_end (arg);
		putc(']', stdout);
		putc('\n', stdout);
	}else{
		va_list arg;
                va_start (arg, format);
                openlog(path, LOG_CONS | LOG_PID | LOG_ODELAY, LOG_USER);
		syslog(LOG_WARNING, caller);
                vsyslog(LOG_WARNING, format, arg);
                closelog();
                va_end (arg);
	}
        textColor(RESET, WHITE, BLACK);
        return 1;
}

int errorf(const char *caller, char *path, const int Line, const char *format, ...){
        textColor(RESET, RED, BLACK);
	if(log_flag){
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
        	va_list arg;
        	va_start (arg, format);
        	vfprintf (stdout, format, arg);
        	va_end (arg);
		putc(']', stdout);
		putc('\n', stdout);
	}else{
		va_list arg;
                va_start (arg, format);
                openlog(path, LOG_CONS | LOG_PID | LOG_ODELAY, LOG_USER);
                syslog(LOG_ERR, caller, Line);
                vsyslog(LOG_ERR, format, arg);
                closelog();
                va_end (arg);
	}
        textColor(RESET, WHITE, BLACK);
        return 2;
}

int panicf(const char *caller, char *path, const int Line, const char *format, ...){
        textColor(DIM, BLACK, RED);
	if(log_flag){
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
        	va_list arg;
        	va_start (arg, format);
        	vfprintf (stdout, format, arg);
        	va_end (arg);
		putc(']', stdout);
        	putc('\n', stdout);
	}else{
		va_list arg;
                va_start (arg, format);
                openlog(path, LOG_CONS | LOG_PID | LOG_ODELAY, LOG_USER);
                syslog(LOG_CRIT, caller, Line);
                vsyslog(LOG_CRIT, format, arg);
                closelog();
                va_end (arg);

	}
	textColor(RESET, WHITE, BLACK);
        return 3;
}
