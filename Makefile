# This line is used to include the interfaces of the program, located at folder /include
CFLAGS=-g -I./include

# This line names the program as mypas and points to every file used in the compilation process
mypas: mypas.o lexer.o parser.o pseudocode.o keywords.o symtab.o utils.o
	$(CC) -o $@ $^

# This command cleans the generated binary-files
clean:
	$(RM) *.o

# This command removes the files created by the editor when editting
mostlyclean: clean
	$(RM) *~
