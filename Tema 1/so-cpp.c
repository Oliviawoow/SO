#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashMap.c"

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

int main (int argc, char *argv[]) {
    char *inputfile = NULL;
    char *outputfile = NULL;
    char *symbol;
    char *mapping;
    char *director;
    int i, k, j;
    char *token;
    FILE *infile;
    FILE *outfile;
    char buffer[257];
    char *cheie;
    char *valoare;
    char *getmyvalue;
    char delimitatori[] = " \(\)\;";
    char *auxtoken;
    char *auxdelim;
    myhash *myhashmap = hashmap_create();

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] == '-' && argv[i][1] == 'D' && strlen(argv[i]) != 2) {
            token = strtok(&argv[i][2], "=");
            if (token == NULL) {
                symbol = "";
            } else {
                symbol = token;
                token = strtok(NULL, "=");
                mapping = token; 
            }
        } else if (argv[i][0] == '-' && argv[i][1] == 'D') {
            token = strtok(&argv[i + 1][0], "=");
            if (token == NULL) {
                symbol = "";
            } else {
                symbol = token;
                token = strtok(NULL, "=");
                mapping = token; 
            }
            i++;
        }

        if (argv[i][0] == '-' && argv[i][1] == 'I' && strlen(argv[i]) != 2) {
            director = &argv[i][2];
        } else if (argv[i][0] == '-' && argv[i][1] == 'I') {
            director = argv[i + 1];
            i++;
        }

        if (argv[i][0] == '-' && argv[i][1] == 'o' && strlen(argv[i]) != 2) {
            outputfile = &argv[i][2];
        } else if (argv[i][0] == '-' && argv[i][1] == 'o') {
            outputfile = argv[i + 1];
            i++;
        } else if (argv[i][0] != '-') {
            inputfile = argv[i];
        }
    }
    
    if (inputfile == NULL) {
        infile = stdin;
    } else {
        infile = fopen(inputfile, "r");
    }
    
    if (outputfile == NULL) {
        outfile = stdout;
    } else {
        outfile = fopen(outputfile, "w");
    }

    while (fgets(buffer, 257, infile) != NULL) {
        /* in search of #define */
        if (buffer[0] == '#' && buffer[1] == 'd') {
            token = strtok(&buffer[8], " ");
            cheie = token;
            token = strtok(NULL, " ");
            valoare = token;
            hashmap_set(myhashmap, cheie, valoare);

        }
        /* in search for main */
        if (buffer[0] == 'i' && buffer[4] == 'm' && buffer[8] == '(') {
            token = strtok(&buffer[0], " ");
            while (token != NULL) {
                fprintf(outfile, "%s ", token);
                token = strtok(NULL, " ");
            }
            while (fgets(buffer, 257, infile) != NULL) {
                token = strtok(&buffer[0], " ");
                while (token != NULL) {
                    getmyvalue = hashmap_get(myhashmap, token);
                    if (getmyvalue != NULL) {
                        token = getmyvalue;
                        if(token[strlen(token) - 1] == '\n') {
                            token[strlen(token) - 1] = 0;
                        }
                    }
                    fprintf(outfile, "%s ", token);
                    token = strtok(NULL, " ");
                }
            }
        }
    }

    fclose(outfile);
    fclose(infile);
    return 0;
}
