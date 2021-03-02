/**@<parser.c>::**/

/*
 * simplified recursive LL(1) parser for expressions.
 *
 * LL(1)-grammar:
 *
 * E -> T R
 * void E(void) {
 * 	T(); R();
 * }
 * R -> oplus T R | <>
 * void R(void) {
 * 	if (lookahead == '+' || lookahead == '-') {
 * 		match (lookahead); T(); R();
 * 	} else {
 * 		;
 * 	}
 * }
 * T -> F Q
 * Q -> otimes F Q | <>
 * F -> (E) | UINT | ID
 *
 * oplus = '+' | '-'
 * otimes = '*' | '/'
 * UINT = [1-9][0-9]* | 0
 * ID = [A-Za-z][A-Za-z0-9]*
 *
 */

#include <parser.h>

void E(void) 
{
	T(); R();
}
void R(void)
{
	if (lookahead == '+' || lookahead == '-') {
		match (lookahead); T(); R();
	} else {
		/** reached this line because lookahead \in FOLLOW(R) **/
		/** FOLLOW(R) = { EOF, ')' } **/
		if ( lookahead == EOF ) {
			match (EOF);
		} else {
			;
		}
	}
}
void T(void)
{ 
	/** FOLLOW(T) = FIRST(R) - {<>} \cup FOLLOW(E)
	 *            = { '+', '-', EOF, ')' } 
	 *            = FOLLOW(Q) **/
	F(); Q();
}
void Q(void)
{
	if (lookahead == '*' || lookahead == '/') {
		match(lookahead); F(); Q();
	} else {
		/** FOLLOW(Q) = { '+', '-', EOF, ')' } **/
		switch (lookahead) {
		case '+':
		case '-':
			break;
		case EOF: 
			match (lookahead); break;
		default:
			;
		}
	}
}
void F(void) 
{ 
	switch (lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case UINT:
		case HEX:
		case OCT:
			match(lookahead);
			break;
		default:
			match(ID);
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
