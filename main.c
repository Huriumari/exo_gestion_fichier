#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define SIZE_MAX 255

char	get_prefix(int size){
	if (size < 10)
		return (char)size + '0';
	if (size < 36)
		return (char)size + 'a' - 10;
	if (size < 62)
		return (char)size + 'A' - 36;
	return 0;
}

void	compresse(char *buffer_read, char *buffer_prev, FILE *file_write){
	int		size;
	char	c[2];

	size = 0;
	c[1] = '\0';
	while(buffer_prev[size] == buffer_read[size] && buffer_read[size] != '\0')
		size++;
	c[0] = get_prefix(size);
	strcat(strcpy(buffer_prev, c), buffer_read + size);
	if (file_write != NULL){
		fwrite(buffer_prev, sizeof(char), strlen(buffer_prev), file_write);
		fwrite("\n", sizeof(char), 1, file_write);
	}
	else
		printf("%s\n", buffer_prev);
	strcpy(buffer_prev, buffer_read);
}

int		translate_prefix(char prefix){
	if (prefix >= '0' && prefix <= '9')
		return (int)prefix - '0';
	if (prefix >= 'a' && prefix <= 'z')
		return (int)prefix - 'a' + 10;
	if (prefix >= 'A' && prefix <= 'Z')
		return (int)prefix - 'A' + 36;
	return 0;
}

void	decompresse(char *buffer_read, char *buffer_prev, FILE *file_write){
	char	buffer[SIZE_MAX];
	int 	size;

	buffer[0] = '\0';
	size = translate_prefix(*buffer_read);
	strncpy(buffer, buffer_prev, size);
	buffer[size] = '\0';
	strcat(buffer, buffer_read + 1);
	if (file_write != NULL){
		fwrite(buffer, sizeof(char), strlen(buffer), file_write);
		fwrite("\n", sizeof(char), 1, file_write);
	}
	else
		printf("%s", buffer);
	strcpy(buffer_prev, buffer);
}

int		main(int ac, char **av){
	FILE	*file_read;
	FILE	*file_write;
	char	d;
	char	buffer_prev[SIZE_MAX];
	char	buffer_read[SIZE_MAX];

	if (ac < 2 || ac > 4){
		fprintf(stderr, "Erreur: invalides arguments. Utilisation ./pack [-option] inputfile [outputfile]\n");
		return 0;
	}
	if (strchr(av[1],'-') && strcmp(av[1], "-d")){
		fprintf(stderr, "Erreur: options inconnu. %s\n", av[1] + 1);
		return 0;
	}
	if (!strcmp(av[1], "-d")){
		file_read = fopen(av[2], "r+b");
		d = 1;
	}
	else{
		file_read = fopen(av[1], "r+b");
		d = 0;
	}
	if (file_read == NULL){
		fprintf(stderr, "Erreur: Aucun fichier trouve.\n");
		return 0;	
	}
	if (ac == 3 && !d){
		file_write = fopen(av[2], "w+");
	}
	else if(d && ac == 4){
		file_write = fopen(av[3], "w+");
	}
	else
		file_write = NULL;
	buffer_prev[0] = '\0';
	while (fgets(buffer_read, SIZE_MAX, file_read), !feof(file_read)){
		if (buffer_read[strlen(buffer_read) - 1] == '\n')
			buffer_read[strlen(buffer_read) - 1] = '\0';
		if (!d)
			compresse(buffer_read, buffer_prev, file_write);
		else
			decompresse(buffer_read, buffer_prev, file_write);
	}
	if (file_write != NULL)
		fclose(file_write);
	fclose(file_read);
}