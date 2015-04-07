flex --header-file=lex.yy.h scanner.l
bison -dtv parser.y
g++ -g -c *.cpp *.c
g++ -g -o ppfx *.o
