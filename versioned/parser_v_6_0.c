/**@<parser.c>::**/

#include <keywords.h>
#include <parser.h>

/* E -> ['+''-'] T { (+) T } */
void E(void) 
{
	if (lookahead == '+' || lookahead == '-') {
		/** if lookahead == '-' then neg := '-' else neg := 0 **/
		match(lookahead);
	}

	T();
	/** if neg then Acc := - Acc **/

	while ( lookahead == '+' || lookahead == '-' ) { 
		/** oplus := lookahead **/
		/** push(Acc) **/
	 	match (lookahead); T();
		/** reg := pop(); if oplus = '+' then Acc := reg + Acc else Acc := reg - Acc **/
	}
}

/* T -> F { (*) F } */
void T(void)
{ 
	F();
	while ( lookahead == '*' || lookahead == '/' ) {
		/** otimes := lookahead **/
		/** push(Acc) **/
		match(lookahead); F();
		/** reg := pop(); if otimes = '*' then Acc := reg * Acc else Acc := reg / Acc **/
	}
}

/*  F ->  ( E )
 *      | n
 *      | v [ = E ] */

void F(void) 
{ 
	switch (lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case UINT:
			/** move(Acc, lexeme) **/
			match(UINT);
			break;
		case FLOAT:
			match(FLOAT);
			break;
		default:
			/** ID.name := lexeme **/
			match(ID);
			if (lookahead == '=') {
				match('='); E();
				/** move(ID.name, Acc) **/
			} else {
				/** move(Acc, ID.name) **/
				;
			}
	}
}

void match(int expected)
{
	if (lookahead == expected) {
		lookahead = gettoken(source);
	} else {
		fprintf(stderr, "token mismatch: expected %d whereas found %d\n",
		expected, lookahead);
		exit(-2);
	}
}
