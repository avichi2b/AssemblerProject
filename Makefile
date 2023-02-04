assembler: main.o text_func.o database_func.o text_func.o save_func.o main_header.h
	gcc -g -Wall -ansi -pedantic main.o text_func.o database_func.o save_func.o main_header.h -o assembler
main: main.c main_header.h
	gcc -c -Wall -ansi -pedantic main.c -o main.o
text_func: text_func.c main_header.h
	gcc -c -Wall -ansi -pedantic text_func.c -o text_func.o
database_func: database_func.c main_header.h
	gcc -c -Wall -ansi -pedantic database_func.c -o database_func.o
save_func: save_func.c main_header.h
	gcc -c -Wall -ansi -pedantic save_func.c -o save_func.o
