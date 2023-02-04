#include "main_header.h"

void bin_addLineEnd(Binary *head, int num, int adrs) /*add a binary line to the end of the list*/
{
	Binary *current = (Binary*)malloc(sizeof(Binary*)*MAX_PROG_LENGTH);
	Binary *new = (Binary*)malloc(sizeof(Binary*)*MAX_PROG_LENGTH);
	current = head;
	if(head->line == -2)
	{
		current->line = num; 
		current->adrs = adrs;
	}
	while (current->next != NULL)
	{
		current = current->next;
	}
	new->line = num;
	new->adrs = adrs;
	current->next = new;
	new->next = NULL;
}

void addLineStart(Line *head, char* buffer, char *label) /*add a line to the start of the list*/
{
	Line *new = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	strcpy(new->text, head->text);
	strcpy(new->label, head->label);
	new->next = head->next;
	head->next = new;
	strcpy(head->text, buffer);
	strcpy(head->label, label);
}

void addLineEnd(Line *head, char* buffer, char *label) /*add a line to the end of the list*/
{
	Line *current = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE); 
	Line *new = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	current = head;
	if(!strcmp(head->label, "start"))
	{
		strcpy(head->text, buffer);
		strcpy(head->label, label);
		head->next = NULL;
		return;
	}
	while (current->next != NULL)
	{
		current = current->next;
	}
	strcpy(new->text, buffer);
	strcpy(new->label, label);
	current->next = new;
	new->next = NULL;
}

void addLabelEend(Label *head, char* buffer, int adrs) /*add a Label to the end of the list*/
{
	Label *current = (Label*)malloc(sizeof(Label*)*MAX_ROW_SIZE); 
	Label *new = (Label*)malloc(sizeof(Label*)*MAX_ROW_SIZE);
	current = head;
	if(!strcmp(head->label, "start"))
	{
		strcpy(head->label, buffer);
		head->adrs = adrs;
		head->next = NULL;
		return;
	}
	while (current->next != NULL)
	{
		current = current->next;
	}
	strcpy(new->label, buffer);
	new->adrs = adrs;
	current->next = new;
	new->next = NULL;
}

void addLineNext(Line *line, char* buffer) /*add a line after a given object*/
{
	Line *current = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	current->next = line->next;
	strcpy(current->text, buffer);
	line->next = current;
}

void rmvLabelEend(Label *head) /*remove a Label from the end of the list*/
{
	Label *current = (Label*)malloc(sizeof(Label*)*MAX_ROW_SIZE); 
	Label *new = (Label*)malloc(sizeof(Label*)*MAX_ROW_SIZE);
	current = head;
	if(!strcmp(head->label, "start")) return;
	while (((current->next)->next) != NULL)
	{
		current = current->next;
	}
	current->next = NULL;
}

int sizeList(Line* line) /*find the size of the list*/
{
	int cnt = 0;
	while (line != NULL)
	{
		cnt++;
		line = line->next;
	}
	return cnt;
}

Line *findMacro(Line *head, char* n) /*find a macro name in the list*/
{
	Line *current = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	char *name = malloc(sizeof(char)*(MAX_ROW_SIZE));
	int i;
	strcpy(name, n);
	current = head;
	clean_txt(name, name);
	i = strlen(name);
	while (current->next != NULL)
	{
		if(!strncmp(current->text, name, i)) return current;
		current = current->next;
	}
	if(!strcmp(current->text, name)) return current;
	return NULL;
}

Label *findLabel(Label *head, char* name) /*find a label in the list*/
{
	if (!name) return NULL;
	Label *current = (Label*)malloc(sizeof(Label*)*MAX_ROW_SIZE);
	current = head;
	while (1)
	{
		if(!strcmp(current->label, name) && current->adrs != 0) return current;
		if (current->next == NULL) return NULL;
		current = current->next;
	}
}

void printList(Line* head) /*prints the list*/
{
	Line *current = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	current = head;
	int cnt = 1;
	while (current->next != NULL)
	{
		printf("%s\n", current->text);
		current = current->next;
		cnt++;
	}
	printf("%s\n", current->text);
	if (!strcmp(head->text, "Errors in the file:"))
	{
		cnt--;
		printf("Total errors: %d\n\n", cnt);
		printf("Fix all error and try again\n");
	}
	else printf("Total: %d\n\n", cnt);
}

void printBin(Binary* head) /*prints the list of binary*/
{
	Binary *current = (Binary*)malloc(sizeof(Binary*)*MAX_PROG_LENGTH);
	int i;
	current = head;
	while (current->next != NULL)
	{
		for(i=WORD_SIZE; i; i--) putchar('0'+((current->line>>(i-1))&1));
		printf("	%d\n", current->adrs);
		current = current->next;
	}
	for(i=WORD_SIZE; i; i--) putchar('0'+((current->line>>(i-1))&1));
	printf("	%d\n", current->adrs);
}

void printLabels(Label *head) /*prints a list*/
{
	Label *current = (Label*)malloc(sizeof(Label*)*MAX_PROG_LENGTH);
	printf("\n");
	current = head;
	while (current->next != NULL)
	{
		printf("%s	%d\n",current->label, current->adrs);
		current = current->next;
	}
	printf("%s	%d\n",current->label, current->adrs);
}

void clearList(Line *head) /*clear the list*/
{
	Line *current = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	current = head;
	while (current->next != NULL)
	{
		Line *del = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
		del = current;
		free(del);
		current = current->next;
	}
}

void clearListLabel(Label *head) /*clear the list*/
{
	Label *current = (Label*)malloc(sizeof(Label*)*MAX_ROW_SIZE);
	current = head;
	while (1)
	{
		Label *del = (Label*)malloc(sizeof(Label*)*MAX_ROW_SIZE);
		del = current;
		if (del != NULL) free(del);
		del = NULL;
		if (current->next != NULL) current = current->next;
		else break;
	}
}

void clearBin(Binary *head) /*clear the list*/
{
	Binary *current = (Binary*)malloc(sizeof(Binary*)*MAX_ROW_SIZE);
	current = head;
	while (current->next != NULL)
	{
		Binary *del = (Binary*)malloc(sizeof(Binary*)*MAX_ROW_SIZE);
		del = current;
		free(del);
		current = current->next;
	}
}




