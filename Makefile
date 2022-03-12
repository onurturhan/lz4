IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =

#LIBS=-lm
all: lz4

_DEPS = lz4.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = lz4.o main.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lz4: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
