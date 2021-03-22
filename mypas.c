/**@<mypas.c>::**/
#include <mypas.h>

/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

/**********************************************************************
 * This file is used as the entry point of our application.
 *
 * The object of the project is to have a simplified compiler
 * that generates intermediate code in the form of pseudocode assembly 
 * 
 * It allows the input to be stdin or a physical file on disk
 **********************************************************************/

// This int will be used as a one-character buffer
int lookahead;

// The source is where the program will read .pas code
FILE *source;

int main(int argc, char const *argv[])
{
	// Look at arg count to see where it shoul load the source
	switch (argc)
	{
	case 1:
		// Uses stdin in case file path isn't provided
		source = stdin;
		break;
	default:
		// Populates source with fopen in case file path is provided
		source = fopen(argv[1], "r");
		// Checks if it was able to open file
		if (source == NULL)
		{
			char message[50];
			snprintf(message, sizeof(message), "Cannot open file at %s", argv[1]);
			show_error(message);
			exit(-1);
		}
	}

	// Read first character from source
	lookahead = gettoken(source);

	// Call mypas at parser.c
	mypas();

	// Exit program without error
	exit(0);
}
