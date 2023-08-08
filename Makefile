CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LFLAGS = -lm

all: encode decode error entropy

encode: encode.o bm.o bv.o hamming.o constants.o encode.c bm.c bv.c hamming.c constants.c
	$(CC) $(CFLAGS) -o encode encode.o bm.o bv.o hamming.o constants.o

decode: decode.o bm.o bv.o hamming.o encode.c constants.o bm.c bv.c hamming.c constants.c
	$(CC) $(CFLAGS) -o decode decode.o bm.o bv.o hamming.o constants.o

error: error.o error.c
	$(CC) $(CFLAGS) -o error error.o

entropy: entropy.o entropy.c
	$(CC) $(CFLAGS) -o entropy entropy.o $(LFLAGS)

encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c

decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

bm.o: bm.c
	$(CC) $(CFLAGS) -c bm.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

hamming.o: hamming.c
	$(CC) $(CFLAGS) -c hamming.c

constants.o: constants.c
	$(CC) $(CFLAGS) -c constants.c

error.o: error.c
	$(CC) $(CFLAGS) -c error.c

entropy.o: entropy.c
	$(CC) $(CFLAGS) -c entropy.c

clean:
	rm -f *.o encode decode error entropy

format:
	clang-format -i -style=file *.h *.c
