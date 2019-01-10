# Directories and macros definitions

CC=gcc

IDIR = ../include
CFLAGS= -g -Wall -std=c99 -Wextra -pedantic -I$(IDIR)
ODIR=obj
LDIR =../lib

_OBJ = main.o utils.o editor.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

####################################################

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

../exe/kiloed: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


.PHONY: clean

# For removing object files
clean:
	rm -f -r $(ODIR)/*.o

# $@ means the left side of :. $^ means the right side of : . $< means the first part on the right side 
