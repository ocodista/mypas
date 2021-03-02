/**@<lexer.c>::**/
// Tue Nov 17 07:55:34 PM -03 2020
/*
 * this is the start project for lexical analyser to be used
 * as the interface to the parser of the project mybc (my basic
 * calculator).
 *
 * In order to have a pattern scan we have first to implement
 * a method to ignore spaces.
 */

#include <stdio.h>
#include <ctype.h>
#include <tokens.h>
void skipunused(FILE *tape)
{
	int head;

	while (isspace(head = getc(tape)))
		;

	ungetc(head, tape);
}
/* Now we need a predicate function to recognize a string
 * begining with a letter (alpha) followed by zero or more
 * digits and letters.
 *
 * REGEX: [A-Za-z][A-Za-z0-9]*
 *
 * isalpha(x) returns 1 if x \in [A-Za-z]
 *            returns 0 otherwise
 * isalnum(x) returns 1 if x \in [A-Za-z0-9]
 *            returns 0 otherwise
 */
int isID(FILE *tape)
{
	int head;

	if (isalpha(head = getc(tape)))
	{
		while (isalnum(head = getc(tape)))
			;
		ungetc(head, tape);
		return ID;
	}

	ungetc(head, tape);

	return 0;
}
/* Next, we have to implement a method to recognize decimal
 * pattern (unsigned int)
 * 
 * REGEX: [1-9][0-9]* | 0
 */
int isUINT(FILE *tape)
{
	int head;

	if (isdigit(head = getc(tape)))
	{
		if (head == '0')
		{
			;
		}
		else
		{
			while (isdigit(head = getc(tape)))
				;
			ungetc(head, tape);
		}
		return UINT;
	}

	ungetc(head, tape);

	return 0;
}
/* Octal pattern is defined as
 * REGEX: 0[0-7]+
 */
int isOCT(FILE *tape)
{
	int head;

	if ((head = getc(tape)) == '0')
	{

		if (isdigit(head = getc(tape)) && head <= 7)
		{

			while (isdigit(head = getc(tape)) && head <= 7)
				;

			ungetc(head, tape);

			return OCT;
		}
		else
		{

			ungetc(head, tape);

			ungetc('0', tape);

			return 0;
		}
	}

	ungetc(head, tape);

	return 0;
}
/* Hexadecimalpattern is defined as
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE *tape)
{
	int head;
	int x;

	if ((head = getc(tape)) == '0')
	{

		if ((x = getc(tape)) == 'X' || x == 'x')
		{

			if (isxdigit(head = getc(tape)))
			{

				while (isxdigit(head = getc(tape)))
					;

				ungetc(head, tape);

				return HEX;
			}
			else
			{

				ungetc(head, tape);

				ungetc(x, tape);

				ungetc('0', tape);

				return 0;
			}
		}

		ungetc(x, tape);

		ungetc('0', tape);

		return 0;
	}

	ungetc(head, tape);

	return 0;
}

int gettoken(FILE *source)
{
	int token;

	skipunused(source);

	if ((token = isID(source)))
		return token;

	if ((token = isOCT(source)))
		return token;

	if ((token = isHEX(source)))
		return token;

	if ((token = isUINT(source)))
		return token;

	token = getc(source);

	return token;
}
