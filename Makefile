CC=gcc
CFLAGS=-Wall 

TARGET = cudp sudp
SRC=$(TARGET:=.c)
OBJ=$(SRC:.c=.o)

all: ${TARGET} 


%.o: %.c
	${CC} -c ${CFLAGS} $< 


%: %.o
	${CC} $< -o $@



clean:
	-rm -f *.o ${BIN} *~ cudp sudp
