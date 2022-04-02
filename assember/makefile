########################################################################
####################### Makefile Nadav and Malak #######################
########################################################################

assembler: main.o pre_processor.o first_scan.o second_scan.o
	gcc -g -ansi -Wall -pedantic main.o pre_processor.o first_scan.o second_scan.o -lm -o assembler
main.o: main.c pre_processor.h structs.h extern.h utils.h
	gcc -c -ansi -Wall -pedantic main.c -o main.o
pre_processor.o: pre_processor.c pre_processor.h utils.h
	gcc -c -ansi -Wall -pedantic pre_processor.c -o pre_processor.o
first_scan.o: first_scan.c structs.h extern.h utils.h
	gcc -c -ansi -Wall -pedantic first_scan.c -o first_scan.o
second_scan.o: second_scan.c structs.h extern.h utils.h
	gcc -c -ansi -Wall -pedantic second_scan.c -o second_scan.o