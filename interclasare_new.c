/*
A10. (6 puncte) Scrieti un program care sa sorteze prin interclasare un fisier
 de caractere in maniera descrisa mai jos.
  Sortarea prin interclasare presupune impartirea sirului in doua jumatati,
 sortarea fiecarei parti prin aceeasi metoda (deci recursiv), apoi 
 interclasarea celor doua parti (care sunt acum sortate).
  Programul va lucra astfel: se imparte sirul in doua, se genereaza doua 
 procese fiu (fork) care sorteaza cele doua jumatati in paralel, tatal ii 
 asteapta sa se termine (wait sau waitpid), apoi interclaseaza jumatatile.
 Nu se vor folosi fisiere auxiliare. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


size_t lungime_fisier(int fd){
	size_t size = lseek(fd, (off_t) 0, SEEK_END);
	lseek(fd, (off_t) 0, SEEK_SET);
	return size;
}

char char_at_position(int i, int fd){
	char c;
	lseek(fd, (off_t) i, SEEK_SET);
	read(fd, &c, 1);
	lseek(fd, (off_t) 0, SEEK_SET);
	return c;
}

void update_file(int pos, int fd, char *buf){
	lseek(fd, (off_t) pos, SEEK_SET);
	write(fd, buf, strlen(buf));
	lseek(fd, (off_t) 0, SEEK_SET);
}

void combina(int fd, int st, int mij, int dr){
	int i = st;
	int j = mij + 1;
	int k = 0;
	int n = dr - st + 1;
	char *buf = (char*) malloc (n * sizeof(char));

	while (i <= mij && j <= dr){
		char c_i = char_at_position(i, fd);
		char c_j = char_at_position(j, fd);

		if (c_i < c_j){
			buf[k++] = c_i;
			i++;
		}	
		else{
			buf[k++] = c_j;
			j++;
		}
	}
	while (i <= mij){ 
		buf[k++] = char_at_position(i, fd);
		i++;
	}
	while (j <= dr){
		buf[k++] = char_at_position(j, fd);
		j++;
	}

	update_file(st, fd, buf);
	free(buf);
}


void asteapta_fii(pid_t *vector_procese){
	if(waitpid(vector_procese[0], NULL, 0) == -1){
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	if(waitpid(vector_procese[1], NULL, 0) == -1){
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
}


int doar_fiul(int fiu, pid_t *vector_procese){
	if((vector_procese[fiu] = fork()) == -1){
		perror("fork()");
		exit(EXIT_FAILURE);
	}
	if(vector_procese[fiu] == 0){
		return 1;
	}
	return 0;
}

void divide(int fd, int st, int dr){
	if(!(st < dr)) return;
	
	int mij = st + (dr-st)/2;
	pid_t vector_procese[2];

	if(doar_fiul(0, vector_procese)){
		divide(fd, st, mij);
		exit(EXIT_SUCCESS);
	}
	if(doar_fiul(1, vector_procese)){
		divide(fd, mij+1, dr);
		exit(EXIT_SUCCESS);
	}

	asteapta_fii(vector_procese);
	combina(fd, st, mij, dr);
}


void prelucreaza(int fd){
	size_t size = lungime_fisier(fd);
	divide(fd, 0, size-1);
}


int main(int argc, char **argv){
	if(argc != 2){
		printf("Utilizare: %s fisier\n", argv[0]);
		return 1;
	}
	
	int fd;
	if((fd = open(argv[1], O_RDWR|O_SYNC)) == -1){
		perror(argv[1]);
		close(fd);
		return 1;
	}

	prelucreaza(fd);
	close(fd);
	return 0;
}
