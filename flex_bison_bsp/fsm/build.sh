flex -B8 --header-file=lex.yy.h scanner.l
bison -dtv parser.y
gcc -g -oparser *.c
