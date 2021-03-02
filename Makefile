# At this line, we include all interfaces (.h files from folder include/)
CFLAGS=-g -I./include

# Here we define the name of the application and each file it'll use to build
mypas: mypas.o lexer.o parser.o pseudocode.o keywords.o symtab.o
	$(CC) -o $@ $^

# Used to delete all compiled files
clean:
	$(RM) *.o

mostlyclean: clean
	$(RM) *~ 
	$(RM) ./include/*~
