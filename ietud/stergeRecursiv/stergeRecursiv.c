/*
D5. (4 puncte) Program care primeste ca argument in linia de comanda un
 director si sterge (recursiv) toata arborescenta cu originea in el.
*/

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

_Bool isCurrentOrParent(char *name)
{
	return strcmp(name, ".") == 0 || strcmp(name, "..") == 0;
}

void sterge(char dirName[], int indent)
{
	DIR *folder;
	struct dirent *entry;
	struct stat filestat;
	

	if((folder = opendir(dirName)) == NULL)
	{ 
		printf("Name: %s\n", dirName);
		perror("Unable to read directory");		
		return;
	}
	else
	{
		chdir(dirName);
	}
	while((entry = readdir(folder)))
	{

		stat(entry->d_name, &filestat);
		
		
		if(S_ISDIR(filestat.st_mode))
		{
			if(isCurrentOrParent(entry->d_name))
			{
				continue;
			}

			printf("%*s%4s: %s\n", indent * 3, " ", "Dir", entry->d_name);
			
			sterge(entry->d_name, ++indent);
		}
		else
		{
			printf("%*s%4s: %s\n", indent * 3, " ", "File", entry->d_name);
			remove(entry->d_name);
		}
		
	}
	chdir("..");
	closedir(folder);
	rmdir(dirName);

}



int main(int argc, char **argv)
{

	sterge(argv[1], 0);

	return 0;
}
