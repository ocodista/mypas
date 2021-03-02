/**@<parser.h>::**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <constants.h>
#include <tokens.h>

extern char lexeme[];

void E(void);
void T(void);
void F(void);

void match(int expected);
int gettoken(FILE *);
extern int lookahead;
extern FILE *source;
