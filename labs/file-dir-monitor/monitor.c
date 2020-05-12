//Author: Sergio Ivan Tostado Nieto
//Student ID: A01229274
//Lab: File-Dir-Monitor

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include "logger.h"
#include <ftw.h>
#include <unistd.h>
#include <string.h>

int filesCont;
int fd;
int *wd;
char **paths;

void wdZeros(int index){
        int i;
        for(i = index; i < 750; i++){
                wd[i] = 0;
        }
}

int handle_input(const char *fpath, const struct stat *sb, int typeflag)  {
    if (typeflag == FTW_D){
        wd[filesCont] = inotify_add_watch(fd, fpath, IN_CREATE | IN_DELETE);
    }else{
        wd[filesCont] = inotify_add_watch(fd, fpath, IN_MODIFY);
    }
    char *str = (char *) calloc(250, sizeof(char));
    int tmpC = 0;
    while(*(fpath + tmpC) != '\0'){
	*(str + tmpC) = *(fpath + tmpC);
	tmpC++;
    }
    *(str + (tmpC + 1)) = '\0';
    paths[filesCont] = str;
    filesCont++;
//    free(str);
    return 0;           /* To tell nftw() to continue*/
}

void countFiles(char const *args[]){
        filesCont = 0;
	char *message_buf;
	message_buf = (char *)malloc(sizeof(char)*300);
        if (ftw(args[1], handle_input, 20) == -1){
                errorf(__func__, message_buf, __LINE__,"Error: An error occured when walking through directory %s \n", args[1]);
                exit(EXIT_FAILURE);
        }
	free(message_buf);
}

void handle_events(int fd, int *wd, int argc, char const *argv[])
{

	char *buf;
	buf = (char *)malloc(sizeof(char)*500);
	const struct inotify_event *event;
	int i;
	ssize_t len;
	char *ptr;
	char *message_buf;
	message_buf = (char *)malloc(sizeof(char)*300);

	for (;;) {
		/* Read events */
		len = read(fd, buf, sizeof(char)*500);
		if (len == -1 && errno != EAGAIN) {
			errorf(__func__, message_buf, __LINE__,"ERROR: Unexpected error when trying to read file descriptor buffer%s", "\n");
			exit(EXIT_FAILURE);
		}

		if (len <= 0)
			break;

		/* Loop over all events in the buffer */

		for (ptr = buf; ptr < buf + len; ptr += sizeof(struct inotify_event) + event->len) {
		        char message[800] = "";
                	strcat(message, "Action performed on ");

			event = (const struct inotify_event *) ptr;
			i = 0;
			while (wd[i] > 0) {
				if (wd[i] == event->wd) {
					strcat(message, paths[i]);
					break;
				}
				i++;
			}
			strcat(message, ". ");

			/* Print the name of the file */
			if (event->len)
				strcat(message, event->name);

			if (event->mask & IN_CREATE){
				if (event->mask & IN_ISDIR)
        	                        strcat(message, " directory ");
	                        else
                                	strcat(message, " file ");
				strcat(message, "was created");
				infof(message_buf, message);
				countFiles(argv);
				continue;
			}else if (event->mask & IN_DELETE){
				if (event->mask & IN_ISDIR)
                                	strcat(message, " directory ");
                        	else
                          	      strcat(message, " file ");
				strcat(message, "was deleted");
				infof(message_buf, message);
				wdZeros(filesCont + 1);
			}else {
				strcat(message, "Was modified");
				infof(message_buf, message);
			}
		}
		countFiles(argv);
	}
	free(buf);
	free(message_buf);
}

int main(int argc, char const *argv[]) {
	initLogger("stdout");
	char *output_buf;
	output_buf = (char *)malloc(sizeof(char)*300);
	if (argc < 2){
		errorf(__func__, output_buf, __LINE__,"Usage: %s", "./monitor [path of directory to monitor]");
		exit(EXIT_FAILURE); //may be is needed to add another library
	}
	fd = inotify_init1(IN_NONBLOCK);
	if (fd == -1){
		errorf(__func__, output_buf, __LINE__,"Error: %s", "An error occured when trying to initialize inotify_init1()\n");
		exit(EXIT_FAILURE);
	}
	//wd = inotify_add_watch(fd, argv[1], IN_CREATE | IN_DELETE);
	wd = calloc(750, sizeof(int));
	paths = (char **)calloc(750, sizeof(char)*250);
	filesCont = 0;
	countFiles(argv);
	while(1){
		handle_events(fd, wd, argc, argv);
	}
	int j = 0;
	while(wd[j] != 0){
		inotify_rm_watch( fd, wd[j]);
		j++;
	}
	//inotify_rm_watch(fd, wd);
	close(fd);
	free(output_buf);
	free(paths);
	free(wd);
	return 0;
}


