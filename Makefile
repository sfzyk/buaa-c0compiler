CC       = gcc.exe -D__DEBUG__ 

OBJ      = absyn.o c0error.o c0lex.o env.o semant.o symbol.o table.o testsec.o util.o lev.o penv.o topcode.o
LINKOBJ  = absyn.o c0error.o c0lex.o env.o semant.o symbol.o table.o testsec.o util.o lev.o penv.o topcode.o
BIN      = sfti.exe
 
CFLAGS   = $(INCS) -g2 -std=c11 -g
 

.PHONY: all all-before all-after clean clean-custom

all: all-before $(BIN) all-after

simulator.exe : simulator simulator.c
	$(CC) simulator.c -o simulator.exe

clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LINKOBJ) -o $(BIN) $(LIBS)

absyn.o: absyn.c
	$(CC) -c absyn.c -o absyn.o $(CFLAGS)

c0error.o: c0error.c
	$(CC) -c c0error.c -o c0error.o $(CFLAGS)

c0lex.o: c0lex.c
	$(CC) -c c0lex.c -o c0lex.o $(CFLAGS)

env.o: env.c
	$(CC) -c env.c -o env.o $(CFLAGS)

semant.o: semant.c
	$(CC) -c semant.c -o semant.o $(CFLAGS)

symbol.o: symbol.c
	$(CC) -c symbol.c -o symbol.o $(CFLAGS)

table.o: table.c
	$(CC) -c table.c -o table.o $(CFLAGS)

testsec.o: testsec.c
	$(CC) -c testsec.c -o testsec.o $(CFLAGS)

util.o: util.c
	$(CC) -c util.c -o util.o $(CFLAGS)

lev.o: lev.c
	$(CC) -c lev.c -o lev.o $(CFLAGS)

penv.o: penv.c
	$(CC) -c penv.c -o penv.o $(CFLAGS)

topcode.o: topcode.c
	$(CC) -c topcode.c -o topcode.o $(CFLAGS)
