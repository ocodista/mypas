/**@<lexer.h>::**/
//TODO: declare all prototypes

extern int lookahead;
extern FILE *source;

int gettoken(FILE *source);
void skipunused(FILE *tape);
