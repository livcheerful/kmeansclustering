all: kmeans.c
	gcc -g -Wall -o kmeans kmeans.c -lm -std=c99

clean:
	$(RM) dist
