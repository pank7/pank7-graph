BINDIR = bin
LIBDIR = lib
INCDIR = include

TARGETS = ${BINDIR}/buildugraph ${BINDIR}/printugraph \
		  ${BINDIR}/builddgraph ${BINDIR}/printdgraph \
		  ${BINDIR}/and ${BINDIR}/da ${BINDIR}/cc ${BINDIR}/rcc
OBJECTS = graph.o common.o membox.o rbtree.o

INCLUDE = -I./${INCDIR}
LIBPATH = -L./${LIBDIR}

LIBS = 

# CFLAGS = ${INCLUDE} -Wall -g -c
CFLAGS = ${INCLUDE} -Wall -O3 -c
OFLAGS = ${LIBPATH} ${LIBS} -o

# CC = gcc
CC = clang
# CC = cc

%.o : %.c
	${CC} ${CFLAGS} $^ -o $@

all: ${TARGETS}

clean :
	-rm -rf ${TARGETS} ${OBJECTS} *.o

tags :
	ctags -R

${BINDIR}/buildugraph : buildugraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

${BINDIR}/builddgraph : builddgraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

${BINDIR}/printugraph : printugraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

${BINDIR}/printdgraph : printdgraph.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

${BINDIR}/and : and.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

${BINDIR}/da : da.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

${BINDIR}/cc : cc.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^

${BINDIR}/rcc : rcc.o ${OBJECTS}
	${CC} ${OFLAGS} $@ $^
