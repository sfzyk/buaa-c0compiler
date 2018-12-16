#语法分析的部分 
sec : semant.o c0lex.o c0error.o util.o absyn.o table.o symbol.o table.o env.o print.o 
	gcc -g -c -o testsec.o testsec.c 
	gcc -g -o sec testsec.o semant.o  c0lex.o c0error.o util.o absyn.o symbol.o table.o env.o  print.o
### 

semant.o : semant.c semant.h c0lex.h c0error.h
	gcc -g -c -o semant.o semant.c 

absyn.o : absyn.c absyn.h symbol.h table.h util.h
	gcc -g  -o absyn.o -c absyn.c 
	 
# 测试词法分析器的正常工作 
lex: lextest.o c0lex.o c0error.o util.o
	gcc -g -o lex lextest.o c0lex.o c0error.o util.o
lextest.o : lextest.c c0lex.h c0error.h util.h
	gcc -g -o test.o -c test.c 
####
c0lex.o: c0lex.c c0lex.h c0error.h
	gcc -g -c c0lex.c -o c0lex.o
c0error.o: c0error.c c0error.h
	gcc -g -c c0error.c -o c0error.o
util.o: util.c util.h
	gcc -g -c util.c -o util.o
symbol.o : symbol.c symbol.h
	gcc -g -c symbol.c -o symbol.o
table.o : table.c table.h
	gcc -g -c table.c -o table.o
env.o : env.c env.h
	gcc -g -c env.c -o env.o
print.o : print.c print.h
	gcc -g -c print.c -o print.o

clean :
	rm *.o
	rm *.exe
	
