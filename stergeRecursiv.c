/*
D5. (4 puncte) Program care primeste ca argument in linia de comanda un
 director si sterge (recursiv) toata arborescenta cu originea in el.
*/

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int not_current_or_parent(const char name[]){
	return !(strcmp(name, ".") == 0 || strcmp(name, "..") == 0);
}

void sterge(const char dir_name[]){
	DIR *folder;
	struct dirent *entry;
	struct stat filestat;

	if((folder = opendir(dir_name)) == NULL){
		perror(dir_name);		
		return;
	}

	chdir(dir_name);
	while((entry = readdir(folder))){
		stat(entry->d_name, &filestat);
		if(S_ISDIR(filestat.st_mode)){
			if(not_current_or_parent(entry->d_name)){
				sterge(entry->d_name);
			}
		}
		else{
			remove(entry->d_name);
		}
	}

	chdir("..");
	closedir(folder);
	rmdir(dir_name);
}

void sterge_recursiv(char dir_name[]){
	char *working_directory = getcwd(NULL, 0);

	if(chdir(dir_name) == -1){
		perror(dir_name);
		return;
	}

	sterge(getcwd(NULL, 0));
	chdir(working_directory);
	free(working_directory);
}

int main(int argc, char **argv){
	if(argc == 1){
		printf("Utilizare: %s cale\n", argv[0]);
		return 1;
	}

	sterge_recursiv(argv[1]);
	return 0;
}


