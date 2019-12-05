/*
Scrieti un program care primeste ca argument in linia de comanda un intreg si afiseaza descompunerea sa in factori primi
*/
#include <stdio.h>
#include <stdlib.h>


void factorizeaza(int number){
	int i = 2;

	printf("Factorizare:\t%d = ", number);
	if (number == 1)
		printf("1 nu poate fi factorizat");

	while(number > 1){
		int grad = 0;

		while(number % i == 0){
			grad++;
			number /= i;
		}
		if(grad != 0){
			printf("%d^%d ", i, grad);
			if(number != 1)
				printf("* ");
		}
		i++;
	}
	printf("\n");
}

void doStuff(char *v){
	int number = atoi(v);
	if(!number){
		printf("Numarul nu este intreg nenul\n");
		return;
	}
    factorizeaza(abs(number));
}

int main(int argc, char **argv){
	if(argc == 1){
		printf("Utilizare: %s nr\n", argv[0]);
		return 1;
	}

	doStuff(argv[1]);
	return 0;
}
