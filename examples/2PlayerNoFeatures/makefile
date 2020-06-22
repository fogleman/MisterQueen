CC=gcc
CFLAGS=-I.
DEPS = ../../src/board.c ../../src/move.c ../../src/gen.c ../../src/bb.c ../../src/util.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

2PlayerNoFeatures : 2PlayerNoFeatures.c
	$(CC) $(CFLAGS) 2PlayerNoFeatures.c -o 2PlayerNoFeatures $(DEPS)

clean:
	rm 2PlayerNoFeatures