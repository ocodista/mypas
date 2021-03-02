/**@<parser.c>::**/

#include <keywords.h>
#include <pseudocode.h>
#include <parser.h>

/* E -> ['+''-'] T { (+) T } */
void E(void) 
{
	/**/int signal = 0, oplus;/**/
	if (lookahead == '+' || lookahead == '-') {
		/**/signal = lookahead;/**/
		match(lookahead);
	}

	T();
	/**/
	if (signal == '-') printf("\tnegate Acc\n");
	/**/

	while ( lookahead == '+' || lookahead == '-' ) { 
		/**/oplus = lookahead;/**/
		/**/push();/**/

	 	match (lookahead); T();

		/**/
		switch(oplus) {
		case '+':
			add();
			break;
		default:
			subtract();
		}
		/**/
	}
}

/* T -> F { (*) F } */
void T(void)
{ 
	F();
	while ( lookahead == '*' || lookahead == '/' ) {
		/**/int otimes = lookahead;/**/
		/**/push();/**/

		match(lookahead); F();

		/**/
		switch(otimes) {
		case '*':
			multiply();
			break;
		default:
			divide();
		}
		/**/
	}
}

/*  F ->  ( E )
 *      | n
 *      | v [ = E ] */

void F(void) 
{ 
	/**/char name[MAXIDLEN+1];/**/
	switch (lookahead) {
		case '(':
			match('('); E(); match(')');
			break;
		case UINT:
			/**/printf("\tmov %s, Acc\n", lexeme);/**/
			match(UINT);
			break;
		case FLOAT:
			match(FLOAT);
			break;
		default:
			/**/strcpy(name, lexeme);/**/
			match(ID);
			if (lookahead == ASGN) {
				match(ASGN); E();
				/**/L_value(name);/**/
			} else {
				/**/R_value(name);/**/
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
