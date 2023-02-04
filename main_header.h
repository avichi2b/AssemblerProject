#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_ROW_SIZE 78 
#define MAX_PROG_LENGTH 40
#define MAX_MACRO 5
#define MAX_MACRO_LENGTH 5
#define MAX_LABEL_LEN 30
#define START_ADRS 100
#define WORD_SIZE 10
#define START_ADRS 100
 /*
 the program uses 3 struct:
 Line for holding the text lines and saving each lines label
 Binary for holding the binary lines with an address and the special simbols 
 Label to keep a list of all the labels and their addresses
 */
typedef struct Line
{
	char text[MAX_ROW_SIZE+1]; 
	char label[MAX_ROW_SIZE+1];
	struct Line *next;
}Line;

typedef struct Binary
{
	int line;
	int adrs;
	char code[2];
	char codeadrs[2];
	struct Binary *next;
}Binary;

typedef struct Label
{
	char label[MAX_ROW_SIZE+1];
	int adrs;
	struct Label *next;
}Label;

/*
all the functions that are needed for the structers that are above
*/
void addLineEnd();
void addLineStart();
void addLineNext();
void addLabelEend();
void rmvLabelEend();
void bin_addLineEnd();

void printLabels();
void printBin();
void printList();

void rmvMac();
int sizeList();
Line *findMacro();
Label *findLabel();

void clearBin();
void clearListLabel();
void clearList();



/*
all the functions that are needed to read the text and procces it
*/
void pre_assembler();
void first_set();
void orgnzID();
void sec_set();

void clean_txt();
void clean_tab();
void find_labels();
void ligalLabel();
int address_type();
int matchReg();



/*
all the functions that are needed to save the information in the files
*/
void saveam();
void saveExt_Ent();
void saveOb();

char specialCode();




