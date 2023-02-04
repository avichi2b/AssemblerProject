#include "main_header.h"

void pre_assembler(FILE *f, Line *prog_head, Line *err_head)
{
	Line *line = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	char *buffer = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	char *mac = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	char *err = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	int cnt = 0;
	int g;
	while (fgets(buffer, MAX_ROW_SIZE+1, f))/*read the next line*/ 
	{
		if(buffer[0] == ';' || buffer[0] == '\n') continue;/*check if the line is relevent*/
		if (strlen(buffer) == MAX_ROW_SIZE)/*check if the line is too long*/
		{
			cnt++;
			g = sprintf(err, "Line is too long. line: %d", cnt);
			addLineEnd(err_head, err, "err");
			continue;
		}
        	clean_tab(buffer, buffer);/*remove all spaces and tabs from beginning*/
        	line = findMacro(prog_head, buffer);/*check if a macro was called*/
        	if(line != NULL && strcmp(prog_head->label,"start")) 
        	{
        		line = line->next;
        		while(strncmp(line->text, "endmacro", 8))
        		{
        			addLineEnd(prog_head, line->text, "NULL");
        			cnt++;
        			line = line->next;
        		}
			continue;
        	}
		strncpy(mac,buffer,5);
		mac[5] = '\0';
		if(!strncmp(mac, "macro", 5))/*check if a new macro is made and save it*/
		{
			strcpy(buffer, buffer + 6);
			line = findMacro(prog_head, buffer);
			addLineEnd(prog_head, buffer, "macro");
			cnt++;
			if(line != NULL)
        		{
        			g = sprintf(err, "More than one macro with the same name. line: %d", cnt);
        			addLineEnd(err_head, err, "err");
        			break;
        		}
			while(1)
			{
				fgets(buffer, MAX_ROW_SIZE, f);
				if(buffer[0] == ';' || buffer[0] == '\n') continue;
				if (strlen(buffer) == MAX_ROW_SIZE)/*check if the line is too long*/
				{
					g = sprintf(err, "Line is too long. line: %d", cnt);
					addLineEnd(err_head, err, "err");
					continue;
				}
				clean_tab(buffer, buffer);
				if (!strncmp(buffer, "endmacro", 8))
				{
					break;
				}
				else
				{
					addLineEnd(prog_head, buffer, "macro");
					cnt++;
				}
			}
		}
		if(!strncmp(buffer, "endmacro", 8))
		{
			addLineEnd(prog_head, buffer, "macro");
			cnt++;
		}
		else
		{
			addLineEnd(prog_head, buffer, "NULL");
			cnt++;
		}
		if (sizeList(prog_head) == MAX_PROG_LENGTH)
		{
			addLineEnd(err_head, "The program is too long", "err");
			return;
		}
	}
	if (cnt == 0) addLineEnd(err_head, "Empty file", "err");
}

void orgnzID(Line *prog_head) /*orginze the Data in the end of the list*/
{
	Line *line = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	Line *prev = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	Line *new = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	int start = 0;
	line = prog_head;
	prev = line;
	addLineEnd(prog_head, "end", "NULL");
	while (strcmp(line->text,"end"))
	{
		if (strstr(line->text, ".data") || strstr(line->text, ".string") || strstr(line->text, ".struct") || strstr(line->text, ".entry") || strstr(line->text, ".extern"))
		{
			addLineEnd(prog_head, line->text, line->label);
			prev->next = line->next;
			if (line == prog_head) start = 1;
			line = line->next;
			continue;
		}
		prev = line;
		line = line->next;
	}
	prev->next = line->next;
	if (start) strcpy(prog_head->text, "");
	else addLineStart(prog_head, "", "NULL");
}

void first_set(Line* prog_head, Line* err_head, Binary* bin_head, Label* label_head) /*make a binary list in the relevent size based on the operands*/
{
	int i = START_ADRS - 1;
	int bin = 0;
	int h = 0;
	int g;
	int adrs1 = -1;
	int adrs2 = -1;
	Line *current = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	Binary *crnt_bin = (Binary*)malloc(sizeof(Binary*)*MAX_PROG_LENGTH);
	char *adrs_1 = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	char *adrs_2 = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	char *err = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	current = prog_head;
	crnt_bin = bin_head;
	int cnt = -1;
	while (1) 
	{
		if (!strcmp(current->label,"macro"))
		{
			if (current->next != NULL) current = current->next;
			else break;
			continue;
		}
		cnt++;
		if (strcmp(current->label,"NULL"))/*save label address if found*/
		{
			ligalLabel(current->label, err_head, cnt);
			addLabelEend(label_head, current->label, i+1); 
		}
		adrs1 = -1;
		adrs2 = -1;
		bin = -1;
		/*add the opcode to the relevent part of the int*/
		if(!strncmp(current->text,"mov",3)) bin += 1;
		else if(!strncmp(current->text,"cmp",3)) bin += 65;
		else if(!strncmp(current->text,"add",3)) bin += 129;
		else if(!strncmp(current->text,"sub",3)) bin += 193;
		else if(!strncmp(current->text,"not",3)) bin += 257;
		else if(!strncmp(current->text,"clr",3)) bin += 321;
		else if(!strncmp(current->text,"lea",3)) bin += 385;
		else if(!strncmp(current->text,"inc",3)) bin += 449;
		else if(!strncmp(current->text,"dec",3)) bin += 513;
		else if(!strncmp(current->text,"jmp",3)) bin += 577;
		else if(!strncmp(current->text,"bne",3)) bin += 641;
		else if(!strncmp(current->text,"get",3)) bin += 705;
		else if(!strncmp(current->text,"prn",3)) bin += 769;
		else if(!strncmp(current->text,"jsr",3)) bin += 833;
		else if(!strncmp(current->text,"rts",3)) bin += 897;
		else if(!strncmp(current->text,"hlt",3)) bin += 961;
		/*add the relevent addrese code to the int of the row and jump to the next row with space rows as needed*/
		if(bin != -1)
		{
			/*check addresses*/
			strcpy(current->text, current->text+3);
			adrs1 = strcspn(current->text, ",");
			adrs2 = strcspn(current->text, "\0") - adrs1;
			strncpy(adrs_1, current->text, adrs1);
			adrs_1[adrs1] = '\0';
			strncpy(adrs_2, (current->text)+adrs1+1, adrs2);
			adrs_2[adrs2] = '\0';
			adrs1 = address_type(adrs_1);
			adrs2 = address_type(adrs_2);
			if(adrs2 == 4)
			{
				adrs2 = adrs1;
				adrs1 = 4;
			}
			/*check for  source address and operand mistakes */
			if (bin != 384 && bin> 192 && adrs1 != 4)
			{
				g = sprintf(err, "Irrelevant source operand. line: %d", cnt);
				addLineEnd(err_head, err, "err"); 
			}
			if (bin == 384  && (adrs1 == 0 || adrs1 == 3))
			{
				g = sprintf(err, "Unidentifide source operand address. line: %d", cnt);
				addLineEnd(err_head, err, "err");
			}
			/*check for  destination address and operand mistakes*/
			if (bin >= 896 && adrs2 != 4)
			{
				g = sprintf(err, "Irrelevant destination operand. line: %d", cnt);
				addLineEnd(err_head, err, "err");
			}
			if (bin != 64 && bin != 768 && adrs2 == 0)
			{
				g = sprintf(err, "Unknown destination operand. line: %d", cnt);
				addLineEnd(err_head, err, "err");
			}
			/*add the relevent addrese code*/
			if(adrs1%2 == 1) bin += 16;
			if(adrs1 == 2 || adrs1 == 3) bin += 32;
			if(adrs2%2 == 1) bin += 4;
			if(adrs2 == 2 || adrs2 == 3) bin += 8;
			/*A, R, E for these commandes is 00*/
			/*add extra lines if needed*/
			bin_addLineEnd(bin_head, bin, ++i);
			if (adrs1 == 0 || adrs1 == 1)
			{
				bin_addLineEnd(bin_head, -2, ++i);
			}
			if (adrs2 == 0 || adrs2 == 1)
			{
				bin_addLineEnd(bin_head, -2, ++i);
			}
			if (adrs1 == 3 || adrs2 == 3)
			{
				bin_addLineEnd(bin_head, -2, ++i);
			}
			if (adrs1 == 2)
			{
				bin_addLineEnd(bin_head, -2, ++i);
				bin_addLineEnd(bin_head, -2, ++i);
			}
			if (adrs2 == 2)
			{
				bin_addLineEnd(bin_head, -2, ++i);
				bin_addLineEnd(bin_head, -2, ++i);
			}
		}
		if(current->text[0] == '.')
		{
			if(!strncmp(current->text,".data",5))
			{
				strcpy(current->text, (current->text)+5);
				/*find all the numbers in the data*/
				char *tok = strtok(current->text, ",");
    				while (tok != NULL)
    				{
        				bin = atoi(tok);
       					bin_addLineEnd(bin_head, bin, ++i);/*save the numbers in lines*/
        				tok = strtok(NULL, ",");
				}
				strcpy(current->text, "");
			}
			if(!strncmp(current->text,".string",7))
			{
				strcpy(current->text, (current->text)+7);
				if (current->text[h] == '"') strcpy(current->text, (current->text)+1);
				else
				{
					g = sprintf(err, "undifined text in command .string. line: %d", cnt);
					addLineEnd(err_head, err, "err");
				}
				while(current->text[h] != '"' && h < strlen(current->text))
				{
					bin_addLineEnd(bin_head, current->text[h], ++i);/*save the characters ascii in lines*/
					h++;
				}
				if (h >= strlen(current->text))
				{
					g = sprintf(err, "undifined text in command .string. line: %d", cnt);
					addLineEnd(err_head, err, "err");
				}
				h = 0;
				bin_addLineEnd(bin_head, 0, ++i);/*save the \0 for end of string*/
				strcpy(current->text, "");
			}
			if(!strncmp(current->text,".struct",7))
			{
				strcpy(current->text, current->text+7);
				char *tok = strtok(current->text, ",");
				bin = atoi(tok);
				bin_addLineEnd(bin_head, bin, ++i);/*save the number ascii of struct*/
				tok = strtok(NULL, "\"");
				while(tok[h] != '"' && h < strlen(tok))
				{
					bin_addLineEnd(bin_head, tok[h], ++i);/*save the characters ascii in lines*/
					h++;
				}
				if (h > strlen(tok))
				{
					g = sprintf(err, "undifined text in command .struct. line: %d", cnt);
					addLineEnd(err_head, err, "err");
				}
				h = 0;
				bin_addLineEnd(bin_head, 0, ++i);/*save the \0 for end of string*/
				strcpy(current->text, "");
			}
			if(!strncmp(current->text,".entry",6))
			{
				if (strcmp(current->label,"NULL"))
				{
					rmvLabelEend(label_head);
					g = sprintf(err, "unrelevant LABEL for .entry command. line: %d", cnt);
					addLineEnd(err_head, err, "err");
				}
				strcpy(current->text, (current->text)+6);
				addLabelEend(label_head, current->text, 0); /*all entry are adrs 0*/
				strcpy(current->text, "");
			}
			if(!strncmp(current->text,".extern",7))
			{
				if (strcmp(current->label,"NULL"))
				{
					rmvLabelEend(label_head);
					g = sprintf(err, "unrelevant LABEL for .extern command. line: %d", cnt);
					addLineEnd(err_head, err, "err");
				}
				strcpy(current->text, current->text+7);
				addLabelEend(label_head, current->text, -1);/*all extr are negative adrs*/
				strcpy(current->text, "");
			}
		}
		if (current->next != NULL)
		{
			current = current->next;
		}
		else break;
	}
}

void sec_set(Line* prog_head, Line* err_head, Binary* bin_head, Label* label_head)
{
	Line *line = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	Binary *bin = (Binary*)malloc(sizeof(Binary*)*MAX_PROG_LENGTH);
	Label *label1 = (Label*)malloc(sizeof(Label*)*MAX_PROG_LENGTH);
	char *reg;
	char *first = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	char *second= malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	char *err = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	int reg1;
	int reg2;
	int num;
	int g;
	int cnt = -1;
	strcpy(reg, "");
	line = prog_head;
	bin = bin_head;
	while (1)
	{
		if (!strcmp(line->label,"macro"))
		{
			if (line->next != NULL) line = line->next;
			else break;
			continue;
		}
		cnt++;
		if (!strcmp(line->text, ""))/*check if line is empty - move to next line*/
		{
			if (line->next == NULL) break;
			line = line->next;
			if (bin->next != NULL) bin = bin->next;
			continue;
		}
		char *tok = strtok(line->text, ","); /*find first adrs*/
		strcpy(first, tok);
		tok = strtok(NULL, "\0");
		if (tok) /*find second adrs if there is one*/
		{
			strcpy(second, tok);
       		}
       		/*check if the adrs is a register*/
       		reg1 = matchReg(first);
       		if (strcmp(second, "")) reg2 = matchReg(second);
       		if (bin->next != NULL) bin = bin->next;
       		if (reg1 > -1 && reg2 > -1 )/*two registers are saved in one line*/
       		{
       			bin->line = (reg1<<6) + (reg2<<2);
  		}
       		else if (reg1 > -1 && reg2 == -2)/*only one address that is regester*/
       		{
       			bin->line = (reg1<<6);
       		}
       		else if (reg1 == -1 && reg2 == -2)/*only one address that is not register*/
       		{
       			tok = strtok(first, ".");
       			if (reg1 >= 0)
       			{
       				bin->line = (reg1<<6);
       			}
       			else if(first[0] == '#')/*number*/
       			{
       				reg1 = atoi(first+1);
       				bin->line = reg1<<2;
       			}
       			else if (label1 = findLabel(label_head, tok))
       			{
       				if (label1->adrs > 0)/*intern label*/
       					bin->line = ((label1->adrs) << 2) + 2;
       				else /*extern label*/
       				{
       					bin->line = 1;
       					if (label1->adrs == -1) /*first call of an extern label*/
       						label1->adrs = (-1) * bin->adrs;
       					else
       						addLabelEend(label_head, first, (-1) * bin->adrs);
       				}
       				if (tok = strtok(NULL, "."))
	       			{
       					num = atoi(tok);
       					bin = bin->next;
       					bin->line = num<<2;
       				}
       			}
       			else
       			{
       				g = sprintf(err, "unknown operand. line: %d", cnt);
       				addLineEnd(err_head, err, "err");
       			}
       		}
       		else /*two diffrent addreses*/
       		{
       			/*first address*/
       			tok = strtok(first, ".");
       			if (reg1 >= 0)
       			{
       				bin->line = (reg1<<6);
       			}
       			else if(first[0] == '#')/*number*/
       			{
       				reg1 = atoi(first+1);
       				bin->line = reg1<<2;
       			}
       			else if (label1 = findLabel(label_head, tok))
       			{
       				if (label1->adrs > 0)/*intern label*/
       					bin->line = ((label1->adrs) << 2) + 2;
       				else /*extern label*/
       				{
       					bin->line = 1;
       					if (label1->adrs = -1) /*first call of an extern label*/
       						label1->adrs = (-1) * bin->adrs;
       					else
       						addLabelEend(label_head, first, (-1) * bin->adrs);
       				}
       				if (tok = strtok(NULL, "."))
	       			{
       					num = atoi(tok);
       					bin = bin->next;
       					bin->line = num<<2;
       				}
       			}
       			else
       			{
       				g = sprintf(err, "unknown operand. line: %d", cnt);
       				addLineEnd(err_head, err, "err");
       			}
       			bin = bin->next;
       			/*second address*/
       			tok = strtok(second, ".");
       			if (reg2 >= 0)
       			{
       				bin->line = (reg2<<2);
       			}
       			else if(second[0] == '#')/*number*/
       			{
       				reg2 = atoi(second+1);
       				bin->line = reg2<<2;
       			}
       			else if (label1 = findLabel(label_head, tok))
       			{
       				if (label1->adrs > 0)/*intern label*/
       					bin->line = ((label1->adrs) << 2) + 2;
       				else /*extern label*/
       				{
       					bin->line = 1;
       					if (label1->adrs == -1) /*first call of an extern label*/
       						label1->adrs = (-1) * bin->adrs;
       					else
       						addLabelEend(label_head, second, (-1) * bin->adrs);
       				}
       				if (tok = strtok(NULL, "."))
	       			{
       					num = atoi(tok);
       					bin = bin->next;
       					bin->line = num<<2;
       				}
       			}
       			else
       			{
       				g = sprintf(err, "unknown operand. line: %d", cnt);
       				addLineEnd(err_head, err, "err");
       			}
       		}
		if (line->next == NULL) break;
		if (bin->next != NULL) bin = bin->next;
		line = line->next;
		strcpy(first, "");
		strcpy(second, "");
		reg1 = -2;
		reg2 = -2;
		label1 = NULL;
	}
	return;
}

void clean_tab(char* cln_txt, const char* old_txt) 
{
	int cnt = 0, i, j;
	while (old_txt[cnt] == ' ' || old_txt[cnt] == '\t') cnt++;
	for (i = cnt, j = 0; old_txt[i] != '\0'; i++, j++)
	{ 
        cln_txt[j] = old_txt[i]; 
	} 
	cln_txt[j] = '\0'; 
}

void clean_txt(char* cln_txt, const char* old_txt) 
{
	while (*old_txt != '\n')
	{
		if(*old_txt != '\t' && *old_txt != ' ')
		{
			*cln_txt = *old_txt; 
			cln_txt++;
		}
		old_txt++;
	}
	*cln_txt = '\0';
}

int matchReg(char *tok)
{
	if (!tok) return -2;
	if (tok[0]=='r' && (tok[1]=='0' || tok[1]=='1' || tok[1]=='2' || tok[1]=='3' || tok[1]=='4' || tok[1]=='5' || tok[1]=='6' || tok[1]=='7'))
		return (tok[1]-'0');
	return -1;
}

int address_type(char *buffer)/*check what type of address is given. if not '#' or register then label*/
{
	int i = 0;
	if (buffer[0] == '\0') return 4; /*null address*/
	if (buffer[0]== '#') return 0;
	if (buffer[0]=='r' && (buffer[1]=='0' || buffer[1]=='1' || buffer[1]=='2' || buffer[1]=='3' || buffer[1]=='4' || buffer[1]=='5' || buffer[1]=='6' || buffer[1]=='7')) return 3;
	if(strcspn(buffer, ".") != strlen(buffer)) return 2;
	return 1;
	
}

void ligalLabel(char *name, Line *err_head, int cnt) /*check if label name is leagel*/
{
	char *err = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	int g;
	if (strlen(name) > 30)
	{
		g = sprintf(err, "too long of a label. line: %d", cnt);
		addLineEnd(err_head, err, "err");
	}
	if (name[0] > 'z' || name[0] < 'A')
	{
		g = sprintf(err, "Illegal label. line: %d", cnt);
		addLineEnd(err_head, err, "err");
	}
	return;
}


void find_labels(Line *prog_head, Line *err_head)
{
	Line *line = (Line*)malloc(sizeof(Line*)*MAX_ROW_SIZE);
	char *buffer = malloc(sizeof(char)*(MAX_ROW_SIZE+1));
	int i = 0;
	line = prog_head;
	while (1)/*find all labels and clean them out*/
	{
		clean_txt(line->text, line->text);
		if(strcspn(line->text, ":") != strlen(line->text))
		{
			i = strcspn(line->text, ":");
			memset(line->label,0,strlen(line->label));
			strncpy(line->label, line->text, i+1);
			line->label[i] = '\0';
			strcpy(buffer, line->text);
			buffer = buffer + i + 1;
			strcpy(line->text, buffer);
		}
		if (line->next == NULL) break;
		line = line->next;
	}
}


