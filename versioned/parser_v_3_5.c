/**@<parser.c>::**/

#include <keywords.h>
#include <parser.h>

/* mybc -> cmd { sepcmd cmd } end
 * cmd -> E | <>
 * end -> QUIT | EXIT | EOF 
 *
 * sepcmd = '\n' | ';'
 */
void cmd(void)
{
	/**/double E_val;/**/
	switch(lookahead) {
	case'\n':
	case';':
	case QUIT:
	case EXIT:
	case EOF: break;
	default:
		/**/E_val = /**/E();
		/**/printf("%lg\n", E_val);/**/
	}
}
void end(void)
{
	switch(lookahead) {
	case EOF:
		match(EOF); break;
	case EXIT:
		match(EXIT); break;
	default:
		match(QUIT); break;
	}
}
void mybc(void)
{
	cmd();
	while (lookahead == '\n' || lookahead == ';') {
		match(lookahead);
		cmd();
	}
	end();
}

/* E -> ['+''-'] T { (+) T } */
double E(void) 
{
	/**/int signal = 0; double E_val, T_val;/**/
	if (lookahead == '+' || lookahead == '-') {
		/**/signal = lookahead;/**/
		match(lookahead);
	}

	/**/T_val = /**/T();
	/**/if(signal=='-'){ T_val = - T_val; }/**/

	/**/E_val = /**/T_val;
	while ( lookahead == '+' || lookahead == '-' ) { 
		/*(1a)*/int oplus = lookahead;/**/
	 	match (lookahead); /**/T_val = /**/T();
		/*(1)*/if(oplus == '+') E_val += T_val; else E_val -= T_val;/**/
	}
	/**/return E_val;/**/
}

/* T -> F { (2a) (*) F (2) } */
double T(void)
{ 
	/**/double T_val, F_val;/**/
	/**/F_val = /**/F(); 
	/**/T_val = F_val;/**/
	while ( lookahead == '*' || lookahead == '/' ) {
		/**/int otimes = lookahead;/**/
		match(lookahead); /**/F_val = /**/F();
		/**/if(otimes == '*') T_val *= F_val; else T_val /= F_val;/**/
	}
	/**/return T_val;/**/
}

/*  F ->  ( E )
 *      | n
 *      | v [ = E ] */

double memory[MAXSTBENTRIES];
typedef
struct _symtab_ {
	char name[MAXIDLEN+1];
	int  pos;
} SYMTAB;
SYMTAB symtab[MAXSTBENTRIES];
int symtab_nextentry = 0;
double recall(const char *name)
{
	int i;
	for (i = 0; i < symtab_nextentry; i++) {
		if ( strcmp(name, symtab[i].name) == 0 ) {
			return memory[i];
		}
	}
	strcpy(symtab[i].name, name);
	symtab_nextentry++;
	return 0;
}
void   store(const char *name, double val){
	int i;
	for (i = 0; i < symtab_nextentry; i++) {
		if ( strcmp(name, symtab[i].name) == 0 ) break;
	}
	if (i == symtab_nextentry) {
		strcpy(symtab[i].name, name);
		symtab_nextentry++;
	}
	memory[i] = val;
}

double F(void) 
{ 
	/**/double F_val;char name[MAXIDLEN+1];/**/
	switch (lookahead) {
		case '(':
			match('('); /**/F_val = /**/E(); match(')');
			break;
		case UINT:
		case FLOAT:
			/**/F_val = atof(lexeme);/**/
			match(lookahead);
			break;
		default:
			/**/strcpy(name, lexeme);/**/
			match(ID);
			if (lookahead == '=') {
				match('='); /**/F_val = /**/E();
				/**/store(name, F_val);/**/
			} else {
				/**/F_val = recall(name);/**/
				;
			}
	}
	/**/return F_val;/**/
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
