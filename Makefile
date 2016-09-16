all: ppmrw.c
	gcc -o ppmrw ppmrw.c

clean:
	rm -rf ppmrw *~

