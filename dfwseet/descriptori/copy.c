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
#include <stdlib.h>

void prelucreaza(char *sursa, int d_destinatie)
{
	int d_sursa = open(sursa, O_RDONLY);
	printf("d_sursa = %d\n", d_sursa);

	int size = lseek(d_sursa, (off_t) 0, SEEK_END);
	printf("Size = %d\n", size);
	lseek(d_sursa, (off_t) 0, SEEK_SET);

	char *buf = (char*) malloc (size * sizeof(char));
	
	//read(d_sursa, buf, size);
	//printf("buffer = %s\n", buf);

	write(d_destinatie, buf, read(d_sursa, buf, size));
	close(d_sursa);
}


int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("Too few arguments.\n");
		return -1;
	}

	printf("argc = %d\n", argc);
	
	if(argc == 3)
	{
		int d_destinatie = open(argv[2], O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
		prelucreaza(argv[1], d_destinatie);
		close(d_destinatie);
		return 0;
	}


	char *fisier;
	
	int d_destinatie = open(argv[argc-1], O_RDWR | O_CREAT | O_APPEND, S_IRWXU);
	int i;

	for(i = 1; i < argc - 1; i+=2)
	{
		fisier = argv[i];
		printf("Sursa = %s\n", fisier);
		prelucreaza(fisier, d_destinatie);
	}
	
	close(d_destinatie);

	return 0;

}

