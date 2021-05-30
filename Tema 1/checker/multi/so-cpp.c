#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	char *inputfile = NULL;
	char *outputfile = NULL;
	char *symbol;
	char *mapping;
	char *director;
	int i;
	char *token;
	FILE *infile;
	FILE *outfile;
	char buffer[257];
	char *cheie;
	char *valoare;
	char *getmyvalue;

	for (i = 1; i < argc; ++i) {
		if (argv[i][0] == '-' && argv[i][1] == 'D' &&
		strlen(argv[i]) != 2) {
			token = strtok(&argv[i][2], "=");
			if (token == NULL)
				symbol = "";
			else {
				symbol = token;
				token = strtok(NULL, "=");
				mapping = token;
			}
		} else if (argv[i][0] == '-' && argv[i][1] == 'D') {
			token = strtok(&argv[i + 1][0], "=");
			if (token == NULL)
				symbol = "";
			else {
				symbol = token;
				token = strtok(NULL, "=");
				mapping = token;
			}
			i++;
		}

		if (argv[i][0] == '-' && argv[i][1] == 'I' && strlen(argv[i]) != 2)
			director = &argv[i][2];
		else if (argv[i][0] == '-' && argv[i][1] == 'I') {
			director = argv[i + 1];
			i++;
		}

		if (argv[i][0] == '-' && argv[i][1] == 'o' &&
		strlen(argv[i]) != 2)
			outputfile = &argv[i][2];
		else if (argv[i][0] == '-' && argv[i][1] == 'o') {
			outputfile = argv[i + 1];
			i++;
		} else if (argv[i][0] != '-') {
			if (inputfile == NULL)
				inputfile = argv[i];
			else if (outputfile == NULL)
				outputfile = argv[i];
			else if (inputfile != NULL && outputfile != NULL)
				exit(-1);
		} else if (argv[i][0] == '-')
			exit(-1);
	}

	if (inputfile == NULL)
		infile = stdin;
	else
		infile = fopen(inputfile, "r");

	if (infile == NULL)
		exit(-1);

	if (outputfile == NULL)
		outfile = stdout;
	else
		outfile = fopen(outputfile, "w");

	while (fgets(buffer, 257, infile) != NULL) {
		/* in search of #define */
		if (buffer[0] == '#' && buffer[1] == 'd') {
			token = strtok(&buffer[8], " ");
			cheie = token;
			token = strtok(NULL, " ");
			valoare = token;
		} else
			fprintf(stdout, "%s", buffer);
	}

	fclose(outfile);
	fclose(infile);
	return 0;
}
