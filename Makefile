CC=clang
CFLAGS=-Wall -Wextra -Werror -Ofast
LIBS=

ODIR=obj
SDIR=src

_OBJ=time.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

time: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	$(RM) $(ODIR)/*.o time
