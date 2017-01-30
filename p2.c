/********************************************************
 *                                                      *
 *      			Dimitris Greasidis                  *
 *                                                      *
 *				Email: temeteron@gmail.com				*
 *														*
 *      		Github Username: Temeteron         		*
 *                                                      *
 *      			Calculate a mathematical       		*
 *      		  	  expression using 3				*
 * 						  processes						*
 *            			                       			*
 *                                                      *
 ********************************************************/
/* LIBRARIES */
#include "libraries1.h"


void calculate(char str[]) ;
void send_data(int fd, char str[]);

int main(int argc,char *argv[]) {
	char str[SIZE], msg[SIZE];

	fcntl(0, F_SETFL, fcntl(0, F_GETFL,0) | O_NONBLOCK);
	
	sprintf(msg,"\tP2 is executing\n");
	write(2,msg,strlen(msg));
	
	while (1) {
		/* read data from p1 */
		do {} while (scanf("%s",str)<=0) ;
		if(str[0] == 'q') {
			sprintf(msg,"\tP2 is terminating\n");
			write(2,msg,strlen(msg));			
			return 1;
		}
		calculate(str);
		printf("%s",str);
		fflush(stdout);
		sprintf(msg,"\tP2 sends to P0\n");
		write(2,msg,strlen(msg));
	}
	
	return 0;
}

void calculate(char str[]) {
	int  num1, num2, num, i, j, pos, start, end;
	char temp[NUMBER_SIZE], format[10], msg[SIZE];
	
	sprintf(msg,"\tCalculating expresion\n");
	write(2,msg,strlen(msg));
			
	while (1) {
		for(i = 1; (str[i] != '+') && (str[i] != '-') ; i++) {
			if(str[i]== '\0') {
				return ;
			}
		}
			
		pos = i;
		/* diavasma arithmou prin to sumvolo tis praksis kai metatropi tou*/
		for(j = pos - 1;  (j > 0 ) && ((*(str + j) >= '0') && (*(str + j) <= '9')); j--);
		start = j;
		if(start == 0) {
			strncpy(temp, str , pos );
			temp[pos]='\0';
		}
		else {
			strncpy(temp, str + start+1, pos - start-1);
			temp[pos - start-1]='\0';
		}
		num1 = atoi(temp);
		/*diavasma arithmou meta to sumvolo tis praksis kai metatropi tou*/
		for(j = pos + 1; ((*(str + j) >= '0') && (*(str + j) <= '9')) && (*(str + j) != '~' ); j++);
		end = j;
		strncpy(temp, str + pos + 1, end - pos -1);
		temp[end - pos-1]='\0';
		num2 = atoi(temp);
		if (str[pos] == '+') {
			num = num1 + num2;
		}
		else {
			num = num1 - num2;
		}
		if(start == 0) {
			sprintf(format,"%%0%dd",end-start);
			sprintf(temp,format,num);
			for(i=start,j=0;temp[j]!='\0';i++,j++) {
				str[i]=temp[j];
			}
		}
		else {
			sprintf(format,"%%0%dd",end-start-1);
			sprintf(temp,format,num);
			for(i=start+1,j=0;temp[j]!='\0';i++,j++) {
				str[i]=temp[j];
			}
		}
	}
}