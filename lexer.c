/**@<lexer.c>::**/
#include <lexer.h>
/****************************************
 * Project MyPas - Group 6
 * 
 * Adriano Yoshimoto
 * Bruno Domene
 * Caio Borghi
 * Gabriel Habberman
 ****************************************/

/***********************************************************
 * This file has the purpose of classifying and validating
 * user entrances according to the lexicals rules of pascal
 ***********************************************************/

// Variable used to count columns
int column = 1;

// Variable used as buffer for string entries
char lexeme[MAXIDLEN + 1];

// Variable to count line_number of current execution
int line_number = 1;

/******************************************
 * Ignores spaces and comments from input
 ******************************************/
void skipunused(FILE *tape)
{
	int head;
_skipspaces:
	while (isspace(head = getc(tape)))
	{
		// With this line we're able to identify the line of an error if any one happens
		if (head == '\n')
			line_number++;
	}

	// Ignoring comments that starts with '{' and ends with '}'
	if (head == '{')
	{
		while ((head = getc(tape)) != '}' && head != EOF)
		{
			if (head == '\n')
				line_number++;
		}

		if (head == '}')
		{
			goto _skipspaces;
		}
	}
	ungetc(head, tape);
}

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
 * isCOMMA -> checks if its ','
 * isCOLON -> checks if its ':'
 * isSEMICOLON -> checks if its ';'
 * isOPEN_PARENTHESES -> checks if its '('
 * isCLOSED_PARENTHESES -> checks if its ')'
 * isDOT -> checks if its '.'
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

	if ((token = isCOMMA(source)))
		return token;

	if ((token = isCOLON(source)))
		return token;

	if ((token = isSEMICOLON(source)))
		return token;

	if ((token = isOPEN_PARENTHESES(source)))
		return token;

	if ((token = isCLOSE_PARENTHESES(source)))
		return token;

	if ((token = isDOT(source)))
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
 * This method validates if input is number using
 * the following rule
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

/***************************************
 * Checks if the next sequence is assign
 * ASGN -> :=
 ***************************************/
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
/**********************************************
 * In this method, we check if the next sequence
 * of the tape is = | > | >= | < | <= | <>
 **********************************************/
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

/**********************************************
 * In this method, we check if the next char
 * of the tape is ','
 **********************************************/
int isCOMMA(FILE *tape)
{
	int head;
	if ((head = getc(tape)) == ',')
		return COMMA;

	ungetc(head, tape);
	return 0;
}

/**********************************************
 * In this method, we check if the next char
 * of the tape is ':'
 **********************************************/
int isCOLON(FILE *tape)
{
	int head;
	if ((head = getc(tape)) == ':')
		return COLON;

	ungetc(head, tape);
	return 0;
}

/**********************************************
 * In this method, we check if the next char
 * of the tape is ';'
 **********************************************/
int isSEMICOLON(FILE *tape)
{
	int head;
	if ((head = getc(tape)) == ';')
		return SEMICOLON;

	ungetc(head, tape);
	return 0;
}

/**********************************************
 * In this method, we check if the next char
 * of the tape is '('
 **********************************************/
int isOPEN_PARENTHESES(FILE *tape)
{
	int head;
	if ((head = getc(tape)) == '(')
		return OPEN_PARENTHESES;

	ungetc(head, tape);
	return 0;
}

/**********************************************
 * In this method, we check if the next char
 * of the tape is ')'
 **********************************************/
int isCLOSE_PARENTHESES(FILE *tape)
{
	int head;
	if ((head = getc(tape)) == ')')
		return CLOSE_PARENTHESES;

	ungetc(head, tape);
	return 0;
}

/**********************************************
 * In this method, we check if the next char
 * of the tape is '.'
 **********************************************/
int isDOT(FILE *tape)
{
	int head;
	if ((head = getc(tape)) == '.')
		return DOT;

	ungetc(head, tape);
	return 0;
}