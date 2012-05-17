TARGETS = buildugraph printugraph builddgraph printdgraph and da cc rcc
OBJECTS = graph.o common.o membox.o rbtree.o

INCLUDE = -I./include
LIBPATH = -L./lib

LIBS = 

# CFLAGS = ${INCLUDE} -Wall -g -c
CFLAGS = ${INCLUDE} -Wall -O3 -c
OFLAGS = ${LIBPATH} ${LIBS} -o

CC = gcc

%.o : %.c
	${CC} ${CFLAGS} $^ -o $@

all: ${TARGETS}

clean :
	-rm -rf ${TARGETS} ${OBJECTS} *.o

tags :
	ctags -R

buildugraph : buildugraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

builddgraph : builddgraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

printugraph : printugraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

printdgraph : printdgraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

and : and.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

da : da.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

cc : cc.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

rcc : rcc.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^
