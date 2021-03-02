/**@<parser.c>::**/

#include <keywords.h>
#include <pseudocode.h>
#include <parser.h>

/******************************
 * Type token convenction table
 * ----------------------------
 * bool		==	1
 * int32	==	2
 * int64	==	3
 * flt32	==	4
 * flt64	==	5
 ******************************/
 enum {
	 BOOL = 1,
	 INT32,
	 INT64,
	 FLT32,
	 FLT64,
 };

#define ILGLTYP	-1

 /***************************************************************************************
  *	   bool  int32  int64  flt32 flt64    +      -      *      /     OR   AND   NOT
  * bool   bool  -----  -----  ----- -----  -----  -----  -----  -----  bool  bool  bool
  * int32  ----  int32  int64  flt32 flt64  int32  int32  int32  int32  ----  ----  ----
  * int64  ----  int64  int64  flt32 flt64  int64  int64  int64  int64  ----  ----  ----
  * flt32  ----  flt32  flt32  flt32 flt64  flt32  flt32  flt32  flt32  ----  ----  ----
  * flt64  ----  flt64  flt64  flt64 flt64  flt64  flt64  flt64  flt64  ----  ----  ----
  ***************************************************************************************/

int iscompat(int acc_type, int sel_type);

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
