final: rotate.o suppl.o decompress.o compress.o main.o filters.o union.o
	cc rotate.o suppl.o decompress.o compress.o main.o filters.o union.o -o final -lm
rotate.o: rotate.c rotate.h
	cc -c rotate.c
suppl.o: suppl.c suppl.h
	cc -c suppl.c
decompress.o: decompress.c decompress.h
	cc -c decompress.c
compress.o: compress.c compress.h
	cc -c compress.c
main.o: main.c filters.h
	cc -c main.c
filters.o: filters.c filters.h
	cc -c filters.c
union.o: union.c union.h
	cc -c union.c
