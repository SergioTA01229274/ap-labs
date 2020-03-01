#include <stdio.h>
#include <stdlib.h>
//Author: Sergio Ivan Tostado Nieto A01229274

void month_day(int year, int yearday, int* pyear, int* pday){
	int months_values[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dic"};
	if (year % 4 == 0)
		months_values[1] = 29;
	int month_day;
	int tmp_yday = *pday;
	int cont = 0;
	for(int i = 0; i < 12; i++){
		if (tmp_yday > months_values[i]){
			tmp_yday = tmp_yday - months_values[i];
			cont ++;
		}else{
			break;
		}
	}
	printf("%s %d, %d\n", months[cont], tmp_yday, *pyear);
}

int main(int argc, char* argv[]) {
	int year = atoi(argv[1]);
	int day = atoi(argv[2]);
	int* p_year = &year;
	int* p_day = &day;
	month_day(year, day, p_year, p_day);
	return 0;
}
