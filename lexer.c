/**@<lexer.c>::**/
/***********************************************************
 * This file has the purpose of classifying and validating
 * user entrances according to the lexicals rules of pascal
 ***********************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <tokens.h>
#include <constants.h>
#include <keywords.h>
#include <lexer.h>

int column = 1;
char lexeme[MAXIDLEN + 1];
int linenumber = 1;
int lookahead;

FILE *source;

/******************************************************
 * This method is used to classify the entrance the 
 * following categories:
 * 
 * ID -> in case follows the rule of ID
 *	  -> [A-Za-z][A-Za-z0-9]*
 *
 * OCT -> in case number is in format of octal
 * HEX -> in case number is in format of hex
 * NUM -> in case of a number not HEX not OCT
 * isASGN -> searchs for assignment (:=)
 * isRELOP -> checks if its comparative(<, <=, >, >=)
 *****************************************************/
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

	if ((token = isNUM(source)))
		return token;

	if ((token = isASGN(source)))
		return token;

	if ((token = isRELOP(source)))
		return token;

	token = getc(source);

	return token;
}

/**********************************************
 * In this method, we need a predicate 
 * function to recognize a string
 * begining with a letter (alpha) followed 
 * by zero or more digits and letters.
 *
 * REGEX: [A-Za-z][A-Za-z0-9]*
 *
 * isalpha(x) returns 1 if x \in [A-Za-z]
 *            returns 0 otherwise
 * isalnum(x) returns 1 if x \in [A-Za-z0-9]
 *            returns 0 otherwise
 **********************************************/
int isID(FILE *tape)
{
	int i = 0;
	if (isalpha(lexeme[i] = getc(tape)))
	{
		i++;
		while (isalnum(lexeme[i] = getc(tape)))
		{
			i++;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		int token = iskeyword(lexeme);
		if (token)
			return token;
		return ID;
	}
	ungetc(lexeme[i], tape);
	lexeme[i] = 0;
	return 0;
}

/**********************************************
 * Next, we have to implement a method 
 * to recognize decimal pattern (unsigned int)
 * 
 * REGEX: [1-9][0-9]* | 0
 ***********************************************/
int isUINT(FILE *tape)
{
	int i = 0;
	if (isdigit(lexeme[i] = getc(tape)))
	{
		if (lexeme[i] == '0')
		{
			i++;
			lexeme[i] = 0;
			;
		}
		else
		{
			i++;
			while (isdigit(lexeme[i] = getc(tape)))
			{
				i++;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
		return UINT;
	}
	ungetc(lexeme[i], tape);
	return 0;
}

/******************************************************
 * Validating if input is number
 *
 * REGEX:
 * ( uint '.' [0-9]*  |  '.' [0-9]+ ) ee?  |  uint ee
 * uint = [1-9][0-9]* | 0
 * ee = [eE] ['+''-']? [0-9]+
 ******************************************************/
int isNUM(FILE *tape)
{
	int token = 0, i = 0;
	if ((token = isUINT(tape)))
	{
		i += strlen(lexeme);
		if ((lexeme[i] = getc(tape)) == '.')
		{
			i++;
			token = FLOAT;
			while (isdigit(lexeme[i] = getc(tape)))
			{
				i++;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
		else
		{
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
	}
	else if ((lexeme[i] = getc(tape)) == '.')
	{
		i++;
		/** decimal point has been already read **/
		if (isdigit(lexeme[i] = getc(tape)))
		{
			i++;
			token = FLOAT;
			while (isdigit(lexeme[i] = getc(tape)))
			{
				i++;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
		}
		else
		{
			/** after decimal point another char, non-digit has been read **/
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			ungetc('.', tape);
		}
	}
	else
	{
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
	}
	int eE;
	if (token > 0)
	{
		if (toupper(lexeme[i] = eE = getc(tape)) == 'E')
		{
			i++;
			int plusminus;
			if ((lexeme[i] = plusminus = getc(tape)) == '+' || plusminus == '-')
			{
				i++;
			}
			else
			{
				ungetc(plusminus, tape);
				plusminus = 0;
			}
			if (isdigit(lexeme[i] = getc(tape)))
			{
				i++;
				token = FLOAT;
				while (isdigit(lexeme[i] = getc(tape)))
				{
					i++;
				}
				ungetc(lexeme[i], tape);
				lexeme[i] = 0;
			}
			else
			{
				ungetc(lexeme[i], tape);
				i--;
				if (plusminus)
				{
					i--;
					ungetc(plusminus, tape);
				}
				i--;
				ungetc(eE, tape);
			}
		}
		else
		{
			ungetc(eE, tape);
		}
	}
	lexeme[i] = 0;
	return token;
}

/*******************************
 * Octal pattern is defined as
 * REGEX: 0[0-7]+
 ********************************/
int isOCT(FILE *tape)
{
	int i = 0;

	if ((lexeme[i] = getc(tape)) == '0')
	{
		i++;
		if (isdigit(lexeme[i] = getc(tape)) && lexeme[i] <= 7)
		{
			i++;
			while (isdigit(lexeme[i] = getc(tape)) && lexeme[i] <= 7)
			{
				i++;
			}
			ungetc(lexeme[i], tape);
			lexeme[i] = 0;
			return OCT;
		}
		else
		{
			ungetc(lexeme[i], tape);
			i--;
			ungetc('0', tape);
			lexeme[i] = 0;
			return 0;
		}
	}
	ungetc(lexeme[i], tape);
	lexeme[i] = 0;
	return 0;
}

/* Hexadecimalpattern is defined as
 * REGEX: 0[xX][0-9A-Fa-f]+
 */
int isHEX(FILE *tape)
{
	int i = 0;
	if ((lexeme[i] = getc(tape)) == '0')
	{
		i++;
		if (toupper(lexeme[i] = getc(tape)) == 'X')
		{
			i++;
			if (isxdigit(lexeme[i] = getc(tape)))
			{
				i++;
				while (isxdigit(lexeme[i] = getc(tape)))
				{
					i++;
				}
				ungetc(lexeme[i], tape);
				lexeme[i] = 0;
				return HEX;
			}
			else
			{
				ungetc(lexeme[i], tape);
				i--;
				ungetc(lexeme[i], tape);
				i--;
				ungetc(lexeme[i], tape);
				lexeme[i] = 0;
				return 0;
			}
		}
		ungetc(lexeme[i], tape);
		ungetc('0', tape);
		return 0;
	}
	ungetc(lexeme[i], tape);
	lexeme[i] = 0;
	return 0;
}

/*******************************
 * Checks if sequence is assign
 * ASGN -> :=
 *******************************/
int isASGN(FILE *tape)
{
	int i = 0;
	if ((lexeme[i] = getc(tape)) == ':')
	{
		i++;
		if ((lexeme[i] = getc(tape)) == '=')
		{
			i++;
			lexeme[i] = 0;
			return ASGN;
		}
		ungetc(lexeme[i], tape);
		i--;
	}
	ungetc(lexeme[i], tape);
	return 0;
}

int isRELOP(FILE *tape)
{
	int i = 0;
	switch ((lexeme[i] = getc(tape)))
	{
	case '=':
		i++;
		lexeme[i] = 0;
		return '=';
	case '>':
		i++;
		if ((lexeme[i] = getc(tape)) == '=')
		{
			i++;
			lexeme[i] = 0;
			return GEQ;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '>';
	case '<':
		i++;
		if ((lexeme[i] = getc(tape)) == '=')
		{
			i++;
			lexeme[i] = 0;
			return LEQ;
		}
		if (lexeme[i] == '>')
		{
			i++;
			lexeme[i] = 0;
			return NEQ;
		}
		ungetc(lexeme[i], tape);
		lexeme[i] = 0;
		return '<';
	}
	ungetc(lexeme[i], tape);
	return lexeme[i] = 0;
}

/**************************************
 * Ignores unused characters from tape
 * 
 * REGEX: [ {\s} | \n | EOF ]
 **************************************/
void skipunused(FILE *tape)
{
	int head;
_skipspaces:
	while (isspace(head = getc(tape)))
	{
		if (head == '\n')
			linenumber++;
	}

	// Ignoring comments that starts with '{' and ends with '}'
	if ((head = getc(tape)) == '{')
	{
		// match('{');
		while ((head = getc(tape)) != '}' || head != EOF)
			if (head == '\n')
				linenumber++;
		;
		if (head == '}')
		{
			// match('}');
			goto _skipspaces;
		}
	}
	ungetc(head, tape);
}