// Logger
#include <stdlib.h>


int initLogger(char *logType);
int infof(char *path, const char *format, ...);
int warnf(const char *caller, char *path, const char *format, ...);
int errorf(const char *caller, char *path, const int Line, const char *format, ...);
int panicf(const char *caller, char *path, const int Line, const char *format, ...);
