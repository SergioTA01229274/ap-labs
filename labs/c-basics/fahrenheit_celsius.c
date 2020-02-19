#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
    int fahr;
    int beg = atoi(argv[1]);
    int upper = atoi(argv[2]);
    int step = atoi(argv[3]);
    for (fahr = beg; fahr <= upper; fahr = fahr + step)
	printf("Fahrenheit: %3d, Celcius: %6.1f\n", fahr, (5.0/9.0)*(fahr-32));

    return 0;
}
