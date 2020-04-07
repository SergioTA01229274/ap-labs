#include <stdio.h>
#include <unistd.h>
#include "logger.h"

int main(){

    // default logging

    // stdout logging
    	initLogger("stdout");
	char *path_buf;
	path_buf = (char *)malloc(sizeof(char)*1000);
	getcwd(path_buf, sizeof(char) * 1000);
	infof(path_buf, "INFO Message: %s", "Testing advanced infof");
	warnf(__func__, path_buf, "WARN Message: %s", "Testing advanced warnf");
    	errorf(__func__, path_buf, __LINE__,"ERROR Message: %s", "Testing advanced errorf");
//	panicf(__func__, path_buf, __LINE__, "PANIC Message: %s", "Testing advanced panicf");


    // syslog logging
    	initLogger("syslog");
        infof(path_buf, "INFO Message: %s", "Testing advanced infof");
        warnf(__func__, path_buf, "WARN Message: %s", "Testing advanced warnf");
        errorf(__func__, path_buf, __LINE__,"ERROR Message: %s", "Testing advanced errorf");
        panicf(__func__, path_buf, __LINE__, "PANIC Message: %s", "Testing advanced panicf");
    	return 0;
}
