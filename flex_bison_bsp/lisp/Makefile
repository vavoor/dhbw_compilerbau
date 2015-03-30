PRG=lisp
OBJ=parser.tab.o lex.yy.o list.o

CC=gcc
CFLAGS=-g -I.
LFLAGS=
YFLAGS=-dtv

.PHONY: all clean

all: $(PRG)
clean:; rm -f $(OBJ) $(PRG) lex.yy.c lex.yy.h parser.tab.c parser.tab.h parser.tab.output

%.o: %.c
	$(CC) -c $(CFLAGS) $<

%.o : %l
%.c : %.l
%.o : %.y
%.c : %.y

$(PRG): $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^

lex.yy.c lex.yy.h: scanner.l
	flex --header-file=lex.yy.h scanner.l

parser.tab.c parser.tab.h: parser.y
	bison $(YFLAGS) parser.y

###
parser.tab.o: parser.tab.c lex.yy.h list.h
lex.yy.o: lex.yy.c parser.tab.h
list.o: list.c list.h
