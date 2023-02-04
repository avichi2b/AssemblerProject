#include "main_header.h"

int IC = 0;
int DC = 0;
int main(int argc, char const *argv[])
{
	char *buffer = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	char *name = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	int *prog_bin = malloc(sizeof(int)*MAX_PROG_LENGTH);
	FILE *file, *am, *ob;
	int i = 0;
	int j = 0;
	int prog_len = 0;
	
	
	if (argc < 2)/*check if one file was passed*/
	{
		printf("At least one file needed to run the program");
		return 1;
	}
	for	(i=1; i<argc; i++) /*all the given files*/
	{
		Line *err_head = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
		Line *prog_head = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
		Binary *bin_head = (Binary*)malloc(sizeof(Binary*)*MAX_PROG_LENGTH);
		Label *label_head = (Label*)malloc(sizeof(Label*)*MAX_PROG_LENGTH);
		strcpy(err_head->text, "Errors in the file:");
		strcpy(err_head->label, "err");
		strcpy(prog_head->label, "start");
		strcpy(prog_head->text, "");
		strcpy(label_head->label, "start");
		err_head->next = NULL;
		prog_head->next = NULL;
		bin_head->next = NULL;
		bin_head->line = -2;
		label_head->next = NULL;
		strcpy(label_head->label, "start");
		memset(name, '\0', sizeof(char)*(MAX_ROW_SIZE+1));
		strcpy(name, argv[i]);
		strcat(name,".as");
		if(name)
		{
			printf("\n\n -- File name: %s -- \n\n",name);
			file = fopen(name, "r");
			if (file == NULL)
			{
				printf("File does not exiset: %s\n\n", name);
				continue; 
			}
			pre_assembler(file, prog_head, err_head);
			if (err_head->next != NULL) /*stop if a mistake was found*/
			{
				addLineEnd(err_head, "mistakes found in pre-assembler. fix them to continue", "err");
				printList(err_head);
				clearList(prog_head);
				clearList(err_head);
				fclose(file);
				continue; 
			}
			strcpy(name, argv[i]);
			strcat(name,".am");
			am = fopen(name, "w+");
			saveam(prog_head, am);/*save am file*/
			find_labels(prog_head, err_head);
			orgnzID(prog_head); /*orginze the IC and DC*/
			first_set(prog_head, err_head, bin_head, label_head);
			sec_set(prog_head, err_head, bin_head, label_head);
			if (err_head->next != NULL) /*stop if a mistake was found*/
			{
				printList(err_head);
				clearList(prog_head);
				clearList(err_head);
				//clearListLabel(label_head);
				clearBin(bin_head);
				fclose(file);
				fclose(am);
				remove(name);
				continue;
			}
			strcpy(name, argv[i]);
			strcat(name,".ob");
			ob = fopen(name, "w+");
			saveExt_Ent(label_head, argv[i]);/*save ext, ent files if needed*/
			saveOb(bin_head, ob);
			printf("No errors found. All files were created successfully\n\n");
			/*printList(err_head);  
			printBin(bin_head);
			printLabels(label_head);*/
			clearList(prog_head);
			clearList(err_head);
			clearListLabel(label_head);
			clearBin(bin_head);
		}
		fclose(am);
		fclose(ob);
		fclose(file);
	}
	return 0;
}

