/**@<parser.h>::**/
#include <stdio.h>
#include <stdlib.h>
#include <tokens.h>
void match(int expected);
int gettoken(FILE *);
extern int lookahead;
extern FILE *source;
void T(void);
void R(void);
void F(void);
void Q(void);
