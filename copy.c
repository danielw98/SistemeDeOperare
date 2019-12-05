/*
Scrieti un program "copy" care se va lansa sub forma:
     copy   f1 + ... + fn    f
 (unde f1, ..., fn, f sunt fisiere) si are ca efect crearea lui f continand
 concatenarea lui f1, ..., fn; daca n=1 se copiaza f1 in f. Se vor folosi
 doar functiile de la nivelul inferior de prelucrare a fisierelor.
*/

#include <stdio.h> 
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h> 


void prelucreaza(char *sursa, int d_destinatie){
	int d_sursa;
	long size, i;
	char c;

	if((d_sursa = open(sursa, O_RDONLY)) == -1){
		perror(sursa);
		return;
	}

	size = lseek(d_sursa, (off_t) 0, SEEK_END);
	lseek(d_sursa, (off_t) 0, SEEK_SET);
	for(i = 0; i < size; i++){
		write(d_destinatie, &c, read(d_sursa, &c, 1));
	}
	close(d_sursa);
}


int main(int argc, char **argv){
	if(argc < 3){
		printf("Utilizare: %s f1 + f2 + ... + fn f\n", argv[0]);
		return 1;
	}

	int d_destinatie = open(argv[argc-1], O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
	int i;
	for(i = 1; i < argc-1; i += 2){
		prelucreaza(argv[i], d_destinatie);
	}
	close(d_destinatie);

	return 0;
}

