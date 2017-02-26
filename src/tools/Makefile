CC = g++
LD = g++
CFLAGS = -std=c++11 -g -O3 -c
LDFLAGS = -g
INCLUDES = -I.. -I.
LIBS =

DUMP_OBJS = dump10.o

dump10: ${DUMP_OBJS}
	${CC} ${LDFLAGS} -o $@ ${DUMP_OBJS} ${LIBS}
	
.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} $<
	
clean:
	rm -f *.o dump10 dump10.exe
