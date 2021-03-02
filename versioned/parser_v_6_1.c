/**@<parser.c>::**/

#include <keywords.h>
#include <parser.h>

/* E -> ['+''-'] T { (+) T } */
/**/int Acc_used = 0;/**/
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
	 	match (lookahead); T();
		/**/ 
		switch(oplus) {
		case '+':
			printf("\tpop reg\n"); 
			printf("\tadd reg, Acc\n"); break;
		default:
			printf("\tmov Acc, reg\n");
			printf("\tpop Acc\n");
			printf("\tsub reg, Acc\n");
		}
		/**/
	}
}

/* T -> F { (*) F } */
void T(void)
{ 
	F();
	while ( lookahead == '*' || lookahead == '/' ) {
		/**/
		int otimes = lookahead;
		printf("\tpush Acc\n");
		/**/
		match(lookahead); F();
		/**/ 
		switch(otimes) {
		case '*':
			printf("\tpop reg\n");
			printf("\tmul reg, Acc\n"); break;
		default:
			printf("\tmov Acc, reg\n");
			printf("\tpop Acc\n");
			printf("\tdiv reg, Acc\n");
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
			/**/
			if (Acc_used) {
				printf("\tpush Acc\n");
			} else {
				Acc_used = 1;
			}
			/**/
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
				/**/printf("\tmov Acc, %s\n", name);/**/
			} else {
				/**/
				if (Acc_used) {
					printf("\tpush Acc\n");
				} else {
					Acc_used = 1;
				}
				/**/
				/**/printf("\tmov %s, Acc\n", name);/**/
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
