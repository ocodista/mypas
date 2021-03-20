/**@<lexer.h>::**/
extern int lookahead;
extern FILE *source;

int gettoken(FILE *source);
void skipunused(FILE *tape);

int isID(FILE *tape);
int isUINT(FILE *tape);
int isNUM(FILE *tape);
int isOCT(FILE *tape);
int isHEX(FILE *tape);
int isASGN(FILE *tape);
int isRELOP(FILE *tape);
