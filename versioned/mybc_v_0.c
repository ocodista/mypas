/**@<mybc.c>::**/

#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>

int gettoken(FILE *);
int lookahead;
FILE *source;

int main(int argc, char const *argv[])
{
	source = fopen (argv[1], "r");

	switch (argc) {
		case 1:
			source = stdin;
			break;
		default:
			if (source == NULL) {
				fprintf (stderr, 
					"argv[1]: cannot open... exiting with error status\n", 
					argv[1]);
				exit (-1);
			}
	}

	lookahead = gettoken(source);

	switch (lookahead) {
		case ID:
			printf("o token identificado é ID\n");
			break;
		case UINT:
			printf("o token identificado é UINT\n");
			break;
		default:
			printf("invalid token\n");
	}

	exit(0);

}
