//Author: Sergio Ivan Tostado Nieto
//Student ID: A01229274
//Delivery: Firs partial challenge, Pacman Log analyzer in C

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


#define REPORT_FILE "packages_report.txt"

void analizeLog(char *logFile, char *report);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

char ** getPackageInfo(char *line){
	int cont = 0;
	int flag = 1;
	int c_char = 0;
	int str_len = 0;
	while(flag){
		if (*(line + cont) == ']'){
			c_char++;
		}
		if (c_char == 2){
			switch (*(line + cont + 2)){
				case 'i':
					str_len = 10; // installed package
					break;
				case 'u':
					str_len = 9; // upgraded package
					break;
				case 'r':
					if (*(line + cont + 3) == 'u' || *(line + cont + 4) == 'i'){
						return NULL;
					}
					str_len = 8; // removed package
					break;
				default:
					return NULL; // useless line
			}
			flag = 0;
		}
		cont++;
	}
	char *packageDate = malloc(sizeof(char) * 17);
	char *key_word = malloc(sizeof(char) * str_len);
	char *packageName = malloc(sizeof(char) * 75);
	for(cont = 0; cont < 16; cont++){
		packageDate[cont] = *(line + cont + 1);
	}
	packageDate[16] = '\0';

	flag = 1;
	c_char = 0;
	while(flag){
		if (*(line + cont) == ']'){
			c_char++;
		}
		cont++;
		if (c_char == 2){
                	break;
                }
	}

	int j = 0;
	while(j < str_len){
		if (*(line + cont + 1) == ' '){
			break;
		}
		key_word[j] = *(line + cont + 1);
		cont++;
		j++;
	}
	key_word[j + 1] = '\0';

	cont++;
	j = 0;
	while(j < 75){
                if (*(line + cont + 1) == ' '){
                        break;
                }
                packageName[j] = *(line + cont + 1);
                cont++;
                j++;

	}
	packageName[j + 1] = '\0';

	static char *res[3];
	res[0] = packageDate;
	res[1] = packageName;
	res[2] = key_word;
	return res;
}



char * getLine(char *lines, int lineStart){
	int cont = lineStart;
	static char res[500];
	int arr_cont = 0;
	while ( *(lines + cont) != '\n'){
	      	res[arr_cont] = *(lines + cont);
		arr_cont++;
		cont++;
        }
	res[arr_cont] = '\0';
	return res;
}

struct packageInfo {
        char *Name;
        char *install_d;
        char *last_u;
        int updates;
        char *removal_d;
};

void analizeLog(char *logFile, char *report) {
	printf("Generating Report from: [%s] log file\n\n", logFile);

	int file_d = open(logFile, O_RDONLY);
	if (file_d == -1){
		printf("Error Number %d\n", errno);
		perror("Failure when trying to open log file");
		exit(1);
	}

        int ls = lseek(file_d, 10, SEEK_END);
	char *content = (char *) calloc(1000, ls * sizeof(char));
	if (close(file_d) < 0){
		perror("Failure when trying to close log file");
		exit(1);
	}

	int sz;
	file_d = open(logFile, O_RDONLY);
        sz = read(file_d, content, ls);

	int c = 0;
	int y = 0;
	struct packageInfo packages[1000];
	int t = 0;

	int t_installed = 0;
	int t_upgraded = 0;
	int t_removed = 0;
	while (y < 3862){
		char *line;
		if (c == 0){
			line = getLine(content, c);
		}else{
			//char *line_tmp = getLine(content, c + 1);
			line = getLine(content, c + 1);
		}
		int u = 0;
		while(*(line + u) != '\0'){
			u++;
                }
		if (c != 0){
			if (*line != '['){
				c += u + 1;
				y++;
				continue;
			}
		}
		c += u + 1;
		char **line_results = getPackageInfo(line);
		if(line_results == NULL){
			y++;
			continue;
		}

		if(strcmp(line_results[2], "installed") == 0){
			struct packageInfo tmp_package = {line_results[1], line_results[0], line_results[0], 0, "-"};
			packages[t] = tmp_package;
			t_installed++;
			t++;
		}else if(strcmp(line_results[2], "upgraded") == 0){
			int tmp_cont = 0;
			while(strcmp(line_results[1], packages[tmp_cont].Name) != 0){
				tmp_cont++;
			}
			packages[tmp_cont].last_u = line_results[0];
			packages[tmp_cont].updates += 1;
                        t_upgraded++;
                }else if(strcmp(line_results[2], "removed") == 0){
                        int tmp_cont = 0;
                        while(strcmp(line_results[1], packages[tmp_cont].Name) != 0){
                                tmp_cont++;
                        }
                        packages[tmp_cont].removal_d = line_results[0];
                        t_removed++;
                }
		y++;
	}
	printf("Pacman Packages general report%s", "\n\n");
	printf("----------------------%s", "\n");
        printf("- Installed packages : %d\n", t_installed);
        printf("- Removed  packages  : %d\n", t_removed);
        printf("- Upgraded packages  : %d\n", t_upgraded);
        printf("- Current installed  : %d\n\n", t_installed - t_removed);

	close(file_d);
	free(content);

	printf("Report is generated at: [%s]\n", report);

	packages[t + 1].Name  = NULL;
	FILE *fptr;
	fptr = fopen(REPORT_FILE, "w+");
	t = 0;
	while(packages[t].Name != NULL){
		fputs("- Package Name\t : " ,fptr);
		fputs(packages[t].Name, fptr);
		fputs("\n", fptr);
		fputs("  - Install date\t : ", fptr);
		fputs(packages[t].install_d, fptr);
                fputs("\n", fptr);
                fputs("  - Last update date\t : ", fptr);
                fputs(packages[t].last_u, fptr);
                fputs("\n", fptr);

		char tmp_up[10];
                fputs("  - How many updates\t : ", fptr);
                sprintf(tmp_up, "%d", packages[t].updates);
		fputs(tmp_up, fptr);
                fputs("\n", fptr);

                fputs("  - Removal date\t : ", fptr);
                fputs(packages[t].removal_d, fptr);
                fputs("\n\n", fptr);
		t++;

	}


	fclose(fptr);
}

