#include "main_header.h"


void saveam(Line *head, FILE *file) /*save the list in am*/ 
{
	Line *current = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	current = head;
	while (1)
	{
		if(strcmp(current->label, "macro"))
		{
			fputs(current->text, file);
		}
		if (current->next == NULL) return;
		current = current->next;
	}
}

void saveExt_Ent(Label *head, char *argv) /*save the ext, ent in there files if exist*/
{
	Label *current = (Label*)malloc(sizeof(Label*)*MAX_PROG_LENGTH);
	Label *entry = (Label*)malloc(sizeof(Label*)*MAX_PROG_LENGTH);
	char *name = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	FILE *ext, *ent;
	int cntx = 0;
	int cntn = 0;
	current = head;
	strcpy(name, argv);
	strcat(name,".ent");
	ent = fopen(name, "w+");
	strcpy(name, argv);
	strcat(name,".ext");
	ext = fopen(name, "w+");
	int adrs;
	while(1)
	{
		if (current->adrs < 0)
		{
			cntx++;
			adrs = current->adrs * -1;
			fputs(current->label, ext);
			fputs("\t", ext);
			fputc(specialCode(adrs/32), ext);
			fputc(specialCode(adrs%32), ext);
			fputs("\n", ext);
		}
		else if (current->adrs == 0)
		{
			cntn++;
			entry = findLabel(head, current->label);
			fputs(entry->label, ent);
			fputs("\t", ent);
			fputc(specialCode((entry->adrs)/32), ent);
			fputc(specialCode((entry->adrs)%32), ent);
			fputs("\n", ent);
		}

		if (current->next == NULL) break; 
		current = current->next;
	}
	fclose(ext);
	fclose(ent);
	if (cntx == 0) cntx = remove(name); 
	strcpy(name, argv);
	strcat(name,".ent");
	if (cntn == 0) cntn = remove(name);		
}

void saveOb(Binary *head, FILE * ob) /*save the code in Object*/
{
	Binary *current = (Binary*)malloc(sizeof(Binary*)*MAX_PROG_LENGTH);
	current = head->next;
	int num, dec;
	while (1)
	{
		current->line = current->line&1023; /*the number 1111111111 in dec*/
		current->adrs = current->adrs&1023; /*the number 1111111111 in dec*/
		(current->code)[0] = specialCode((current->line)/32);
		(current->code)[1] = specialCode((current->line)%32);
		(current->codeadrs)[0] = specialCode((current->adrs)/32);
		(current->codeadrs)[1] = specialCode((current->adrs)%32);
		fputc((current->codeadrs)[0], ob);
		fputc((current->codeadrs)[1], ob);
		fputs("\t", ob);
		fputc((current->code)[0], ob);
		fputc((current->code)[1], ob);
		fputs("\n", ob);
		/*printf("%c%c", (current->code)[0], (current->code)[1]);
		printf("	%c%c\n", (current->codeadrs)[0], (current->codeadrs)[1]);*/
		if (current->next == NULL) return;
		current = current->next;
	}
}

char specialCode(int num) /*convert a number to special code*/ 
{
	if (num == 0) return '!';
	else if (num == 1) return '@';
	else if (num == 2) return '#';
	else if (num == 3) return '$';
	else if (num == 4) return '%';
	else if (num == 5) return '^';
	else if (num == 6) return '&';
	else if (num == 7) return '*';
	else if (num == 8) return '<';
	else if (num == 9) return '>';
	else if (num == 10) return 'a';
	else if (num == 11) return 'b';
	else if (num == 12) return 'c';
	else if (num == 13) return 'd';
	else if (num == 14) return 'e';
	else if (num == 15) return 'f';
	else if (num == 16) return 'g';
	else if (num == 17) return 'h';
	else if (num == 18) return 'i';
	else if (num == 19) return 'j';
	else if (num == 20) return 'k';
	else if (num == 21) return 'l';
	else if (num == 22) return 'm';
	else if (num == 23) return 'n';
	else if (num == 24) return 'o';
	else if (num == 25) return 'p';
	else if (num == 26) return 'q';
	else if (num == 27) return 'r';
	else if (num == 28) return 's';
	else if (num == 29) return 't';
	else if (num == 30) return 'u';
	else if (num == 31) return 'v';
}

