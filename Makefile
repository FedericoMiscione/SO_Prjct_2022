CC=gcc
CCOPTS=--std=gnu99 -g -Wall
AR=ar

OBJS=libs.o

HEADERS=libs.h

LIBS=libs.a

BINS=top

.phony:	clean all

all: $(BINS) $(LIBS)

%.o: %.c $(HEADERS)
	$(CC) $(CCOPTS) -c -o $@  $<

libs.a:	$(OBJS)
	@echo "Generazione della libreria..."
	$(AR) -rcs $@ $^
	rm -rf $(OBJS)

top: main.c $(LIBS)
	@echo "Compilazione del main in corso..."
	$(CC) $(CCOPTS) -g -o $@ $^ -lm

clean:
	@echo "Pulizia in corso..."
	rm -rf *.o *~ $(LIBS) $(BINS)
