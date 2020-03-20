#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* filecopy:  copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);

}

void catFile(int fptr, FILE *ofp, char *fileName){
	int ls = lseek(fptr, 10, SEEK_END);
	char *content_buf = (char *) calloc(1000, ls * sizeof(char) + sizeof(char));
	close(fptr);
	int fptr_tmp = open(fileName, O_RDONLY);
	read(fptr, content_buf, ls);
	content_buf[ls + 1] = '\0';
	int cont = 0;
	while(*(content_buf + cont) != '\0'){
		putc(*(content_buf + cont), ofp);
		cont++;
	}
}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    	char *prog = argv[0];   /* program name for errors */

	int fp;
    	if (argc == 1){  /* no args; copy standard input */
        	filecopy(stdin, stdout);
		if (ferror(stdout)){
			fprintf(stderr, "%s: error writting stdout\n", prog);
			return 2;
		}
		return 0;
	}
	else{
		int c = 1;
        	while (--argc > 0){
            		if ((fp = open(*(argv + c), O_RDONLY)) == -1) {
                		fprintf(stderr, "%s: can′t open %s\n", prog, *argv);
                		return 1;
            		} else {
				catFile(fp, stdout, *(argv + c));
                		close(fp);
            		}
			c++;
		}
	}
    	if (ferror(stdout)) {
        	fprintf(stderr, "%s: error writing stdout\n", prog);
        	return 2;
    	}
    	return 0;
}
