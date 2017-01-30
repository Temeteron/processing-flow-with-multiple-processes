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
#include "libraries0.h"

	/* Prototypes of Functions */
void results(int fd);
int scan_check(char *str);
/**********
 *  main  *
 **********/
int main(int argc,char *argv[]) {
	int pipe1[2],pipe2[2],pipe3[2], len;
	char str[SIZE];
	ssize_t check;
	pid_t pid1, pid2;

	if (pipe(pipe1) < 0) { 
		perror("pipe"); 
		return 1; 	
	}
	if (pipe(pipe2) < 0) { 
		perror("pipe"); 
		return 1; 	
	}
	if (pipe(pipe3) < 0) { 
		perror("pipe"); 
		return 1; 	
	}
	
/* create processes */
		/* process p1 */
	if ((pid1 = fork()) < 0) { 
		perror("fork"); 
		return -1;
	}
	if (pid1 == 0) {

		dup2(pipe1[0],0); //redirect p1 reading
		close(pipe1[0]);
		
		dup2(pipe2[1],1); //redirect p1 writing
		close(pipe2[1]);

		execlp("./p1", "p1", NULL);
		perror("execlp");
		return 1;
	}
	/* process p2 */
	if ((pid2 = fork()) < 0) {
		perror("fork"); 
		return -1;
	}
	if (pid2 == 0) {
		dup2(pipe2[0],0); //redirect p2 writing
		close(pipe2[0]);
		
		dup2(pipe3[1],1); //redirect p2 reading
		close(pipe3[1]);
		

		execlp("./p2", "p2", NULL);
		perror("execlp");
		return 1;
	}

	while(1) {
		/* eisagogi dedomenon me elegxo */
		if(scan_check(str)) continue;
		if ((str[0] == 'q') || (str[0] == 'Q')) break;
		
		/* send data to p1 */
		len=strlen(str);
		check = write(pipe1[1], str, (size_t)len + 1);
		if (check < 0) {
			perror("write");
			close(pipe1[1]);
			exit(1);
		}
		results(pipe3[0]);
	}
	/* send q to p1 and wait for p1,p2 to terminate */
		/* send q to p1 */
	check = write(pipe1[1], "q", 1);
	if (check < 0) {
		perror("write");
		close(pipe1[1]);
		exit(1);
	}
	
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	
	
	close(pipe1[0]);
	close(pipe1[1]);
	close(pipe2[0]);
	close(pipe2[1]);
	close(pipe3[0]);
	close(pipe3[1]);
	
	return 0;
}

void results(int fd){
	int num;
	char str[SIZE];
	ssize_t check;
	
			/* check results */
		check = read(fd,  str, SIZE - 1);
		if (check < 0) {
			perror("Read error");
			close(fd);	
			exit(1);
		}
		if (check > 0 ) {
			if (str[0] == 'e') {
				printf("Diairesi me (0), aprosdioristia.\n");
			}
			else {
				str[check] = '\0';
				num = atoi(str);
				printf("The result is %d\n", num);
			}
		}
}

int scan_check(char *str) {
	int i = 0, flag = 0, flag1 = 0;
	char c;

	printf("GIVE EXPRESSION.\n");
	while(1) {
		c = getchar();
		if (isalpha(c)) {
			if ((c == 'q') || (c == 'Q')) {
				c = getchar();
				if (c == '\n') {
					str[i] = 'q';
					i++;
					flag = 1;
					printf("Quiting programm.\n");
					break;
				}
			}
			/* adeiasma buffer */
			while(1) {
				c = getchar();
				if (c == '\n') {
					break;
				}
			}
			i = 0;
			flag1 = 0;
			printf("EXPRESSION contained character from the alphabet. Give new expression.\n");
			continue;
		}
		else {
			if ((flag1) && ((c == '-') || (c == '+') || (c == '*') || (c == '/'))) {
				printf("sunexomenoi telestes. ERROR. GIVE EXPRESSION AGAIN.\n");
				while(1) {
					c = getchar();
					if (c == '\n') {
							break;
					}
				}
				i = 0;
				continue;
			}
			if ((c == '-') || (c == '+') || (c == '*') || (c == '/')) {
				flag1 = 1;
				if (i == 0) {
					printf("First character is: %c , but we want a number. GIVE expression AGAIN.\n", c);
					/* adeiasma buffer */
					while(1) {
						c = getchar();
						if (c == '\n') {
							break;
						}
					}
					i = 0;
					flag1 = 0;
					continue;
				}
			}
			else if (isblank(c)) {
				continue;
			}
			else if (c == '\n') {
				break;
			}
			else {
				flag1 = 0;
			}
		}
		str[i] = c;
		i++;
	}
	str[i] = '\0';
	if(flag1) {
		printf("Last character is not a number.\n");
		return 1;
	}
	if (!flag) {
		printf("EXPRESSION: %s\n", str);
	}	
	return 0;
}