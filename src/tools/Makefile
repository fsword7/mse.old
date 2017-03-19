CC = g++
LD = g++
CFLAGS = -std=c++11 -g -O3 -c
LDFLAGS = -g
INCLUDES = -I.. -I.
LIBS =

DUMP_OBJS =   dump.o
DUMP10_OBJS = dump10.o

dump: ${DUMP_OBJS}
	${CC} ${LDFLAGS} -o $@ ${DUMP_OBJS} ${LIBS}

dump10: ${DUMP10_OBJS}
	${CC} ${LDFLAGS} -o $@ ${DUMP10_OBJS} ${LIBS}
	
.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} $<
	
clean:
	rm -f *.o dump dump10 dump.exe dump10.exe
