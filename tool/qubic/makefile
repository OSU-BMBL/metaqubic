VER=1.0
DIST=qubic$(VER)
PROGS=qubic
SRCS=struct.c read_array.c make_graph.c get_options.c fib.c write_block.c cluster.c main.c expand.c
OBJS=$(SRCS:.c=.o) 
CC=gcc


LDFLAGS=-static  -lm
CFLAGS=-O3 -Wall -ansi -I.  -DVER=$(VER)

all: $(PROGS)

${PROGS}: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

.o:
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -f $(PROGS)
	rm -f *.o
	rm -f *.rules
	rm -f *.chars
	rm -f *.blocks
	rm -f *.expansion

dist:
	$(MAKE) clean
	cd .. && tar czvf $(DIST).tar.gz $(DIST)/

Ecoli.chars:
	./${PROGS} -i Ecoli

CRP.blocks:
	./${PROGS} -i CRP 

test: Ecoli.chars CRP.blocks
	./${PROGS} -i Ecoli.chars -b CRP.blocks -s
