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
#include <sys/mman.h>


void* create_shared_memory(size_t size){
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANONYMOUS;
	return mmap(NULL, size, protection, visibility, -1, 0);
}


size_t lungime_fisier(int fd){
	size_t size = lseek(fd, (off_t) 0, SEEK_END);
	lseek(fd, (off_t) 0, SEEK_SET);
	return size;
}

void combina(char* v, int st, int mij, int dr){
	int i = st;
	int j = mij + 1;
	int k = 0;
	int n = dr - st + 1;
	char *c = (char*) malloc (n * sizeof(char));

	while(i <= mij && j <= dr)
		if(v[i] < v[j])
			c[k++] = v[i++];		
		else
			c[k++] = v[j++];
	
	while(i <= mij)
		c[k++] = v[i++];	
	while(j <= dr)
		c[k++] = v[j++];
	
	for(i = 0; i < n; i++)
		v[st+i] = c[i];
	free(c);
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
		perror("\nfork()");
	}
	if(vector_procese[fiu] == 0){
		return 1;
	}
	return 0;
}

int doar_parintele(pid_t *vector_procese){
	return (vector_procese[0] > 0) && (vector_procese[1] > 0);
}


void divide(char* v, int st, int dr){
	if(!(st < dr)) return;
	int mij = st + (dr-st)/2;
	pid_t vector_procese[2];

	if(doar_fiul(0, vector_procese)){
		divide(v, st, mij);
		exit(EXIT_SUCCESS);
	}
	if(doar_fiul(1, vector_procese)){
		divide(v, mij+1, dr);
		exit(EXIT_SUCCESS);
	}
	if(doar_parintele(vector_procese)){
		asteapta_fii(vector_procese);
		combina(v, st, mij, dr);
	}
}


void write_buffer(char* buffer, char* path, size_t size){
	int out_d = open(path, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
	write(out_d, buffer, size);
	write(out_d, "\n", strlen("\n"));
	close(out_d);
}

void prelucreaza(int fd){
	size_t size = lungime_fisier(fd);
	char *v = (char*) create_shared_memory(size);
	read(fd, v, size);
	divide(v, 0, size-1);
	write_buffer(v, "f1.txt", size);
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
