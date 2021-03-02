/**@<parser.h>::**/
#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
void E(void) ;
void match(int expected);
int gettoken(FILE *);
extern int lookahead;
extern FILE *source;
