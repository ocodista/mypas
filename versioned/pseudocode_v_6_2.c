#include <stdio.h>

void R_value(const char *name) {
	/**/printf("\tmov %s, Acc\n", name);/**/
}
void L_value(const char *name) {
	/**/printf("\tmov Acc, %s\n", name);/**/
}
void add(void) {
	printf("\tpop reg\n");
	printf("\tadd reg, Acc\n");
}
void subtract(void) {
	printf("\tmov Acc, reg\n");
	printf("\tpop Acc\n");
	printf("\tsub reg, Acc\n");
}
void multiply(void) {
	printf("\tpop reg\n");
	printf("\tmul reg, Acc\n");
}
void divide(void) {
	printf("\tmov Acc, reg\n");
	printf("\tpop Acc\n");
	printf("\tdiv reg, Acc\n");
}
void push(void) {
	/**/printf("\tpush Acc\n");/**/
}
