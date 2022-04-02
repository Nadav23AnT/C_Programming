#include "extern.h"
#include "utils.h"

int error = 0;

#define action_block4 \
	{                 \
		1, 1, 1, 1    \
	}
#define action_block3 \
	{                 \
		0, 1, 1, 1    \
	}
#define action_block2 \
	{                 \
		0, 1, 1, 0    \
	}
#define action_block0 \
	{                 \
		0, 0, 0, 0    \
	}
/*array of actions , action[i] contain the information about action number i*/
action actions[NUMBER_ACTIONS] = {{"mov", 0, 0, action_block4, action_block3, 2},
								  {"cmp", 0, 1, action_block4, action_block4, 2},
								  {"add", 10, 2, action_block4, action_block3, 2},
								  {"sub", 11, 2, action_block4, action_block3, 2},
								  {"lea", 0, 4, action_block2, action_block3, 2},
								  {"clr", 10, 5, action_block0, action_block3, 1},
								  {"not", 11, 5, action_block0, action_block3, 1},
								  {"inc", 12, 5, action_block0, action_block3, 1},
								  {"dec", 13, 5, action_block0, action_block3, 1},
								  {"jmp", 10, 9, action_block0, action_block2, 1},
								  {"bne", 11, 9, action_block0, action_block2, 1},
								  {"jsr", 12, 9, action_block0, action_block2, 1},
								  {"red", 0, 12, action_block0, action_block3, 1},
								  {"prn", 0, 13, action_block0, action_block4, 1},
								  {"rts", 0, 14, action_block0, action_block0, 0},
								  {"stop", 0, 15, action_block0, action_block0, 0},
								  {"NONE", 0, 0, action_block0, action_block0, 0}};
/*this function reads the lines from the file and it calls to other functions in order to build the binary code*/
int first_pass(FILE *file, machineCode *code_import, machineCode *data_import, char *filename)
{
	int IC = IC_INIT;
	int DC = DC_INIT;
	int count_line = 0;
	int symbolDef = 0;
	symbol_item *head = add_symbol(NULL, "", -1);
	char symbol_name[SYMBOL_MAXLEN];
	symbol_item *temp_symbol;
	machineCode *code = code_import;
	machineCode *dataImage = data_import;
	action temp_action;
	int *ptr_ic = &IC;
	int *ptr_dc = &DC;
	line *pointer_line = (line*)malloc(sizeof(line));
	char corrent_line[LINE_LEN];
	head->next = NULL;
	while (fgets(corrent_line, LINE_LEN, file))
	{
		count_line++;
		printf("DEGUG: corrent line:\t %s \n", corrent_line);
		pointer_line = create_line(corrent_line, " ", count_line);
		if (!pointer_line || is_comment_first(pointer_line->first_word))
		{
			free(pointer_line);
			continue;
		}
		symbolDef = is_symbol_def(pointer_line->first_word);
		if (symbolDef == 1)
		{
			strcpy(symbol_name, strtok(pointer_line->first_word, ":")); /* Save symbol name*/
			pointer_line = split_line(pointer_line, " ", count_line);	/* skipping label name*/
		}

		if (symbolDef == 1 && symbol_exist(pointer_line->first_word, head))
		{
			printf("ERROR - Symbol already exist %s\n", pointer_line->first_word);
			error = 1;
			free(pointer_line);
			continue;
		}
		/* TYPE OF SENTENCE : it guidance sentence */
		/* .string  */
		if (!strcmp(".string", pointer_line->first_word))
		{
			if (symbolDef == 1) /* if symbol defined */
			{
				temp_symbol = add_symbol(head, symbol_name, *ptr_ic); /* Save symbol is symbol table*/
				init_symbol(temp_symbol, DATA_SYMBOL, NONE, *ptr_ic + *ptr_dc); /* configure symbol */
				add_stringCode(pointer_line, dataImage, ptr_dc, count_line);
			}
			else
			{
				pointer_line = split_line(pointer_line, " ", count_line); /* points to the content of string*/
				add_stringCode(pointer_line, dataImage, ptr_dc, count_line);
			}
			free(pointer_line);
			continue;
		}
		/* .data  */
		if (!strcmp(".data", pointer_line->first_word))
		{
			if (symbolDef == 1) /* if symbol defined */
			{
				temp_symbol = add_symbol(head, symbol_name, *ptr_ic); /* Save symbol is symbol table*/
				init_symbol(temp_symbol, DATA_SYMBOL, NONE, *ptr_ic + *ptr_dc); /* configure symbol */
				add_dataCode(pointer_line->content, dataImage, ptr_dc, count_line);
			}
			else
			{
				add_dataCode(pointer_line->content, dataImage, ptr_dc, count_line);
			}
			free(pointer_line);
			continue;
		}
		/* .entry  */
		if (!strcmp(".entry", pointer_line->first_word))
		{
			if (!(pointer_line->content))
			{
				printf("ERROR - there is no words after .entry \n");
				error = 1;
			}
			if (isalpha(*(pointer_line->content)) == 0)
			{
				printf("Error: First char is not alpha %d\n", count_line);
				error = 1;
			}
			free(pointer_line);
			continue;
		}
		/* .extern  */
		if (!strcmp(".extern", pointer_line->first_word))
		{
			if (!(pointer_line->content))
			{
				printf("ERROR - there is no words after .extern \n");
				error = 1;
				free(pointer_line);
				continue;
			}
			if (symbol_exist(pointer_line->content, head) &&
				symbol_exist(pointer_line->content, head)->attribute != EXTERNAL_SYMBOL &&
				symbol_exist(pointer_line->content, head)->attribute2 != EXTERNAL_SYMBOL)
			{
				printf("Warning - Symbol already exist %s\n", pointer_line->first_word);
				end_of_line(pointer_line->content, count_line); /*   too many arguments   */
				free(pointer_line);
				continue;
			}
			else
			{
				if (is_symbol_def(pointer_line->content) == 2)
				{
					if (pointer_line->content[strlen(pointer_line->content) - 1] == '\n')
					{
						pointer_line->content[strlen(pointer_line->content) - 1] = '\0';
					}
					temp_symbol = add_symbol(head, pointer_line->content, 0);
					init_symbol(temp_symbol, NONE, EXTERNAL_SYMBOL, 0 );
					free(pointer_line);
					continue;
				}
				else
				{
					printf("Error: First char is not alpha line: %d\n", count_line);
					error = 1;
					end_of_line(pointer_line->content, count_line); /*   too many arguments   */
					free(pointer_line);
					continue;
				}
			}
		}
		temp_action = is_action(pointer_line->first_word);
		if (!strcmp(temp_action.name, "NONE"))
		{
			printf("is not an order sentance \n");
			error = 1;
		}
		/* TYPE OF SENTENCE : it order sentence */
		else
		{
			if (symbolDef == 1)
			{
				temp_symbol = add_symbol(head, symbol_name, *ptr_ic);
				init_symbol(temp_symbol, CODE_SYMBOL, NONE, *ptr_ic);
				add_actionCode(pointer_line, code, ptr_ic, count_line, temp_action, head);
			}
			else
			{
				if (symbolDef == 0)
				{
					printf("symbol is not a legal symbol\n");
					error = 1;
					free(pointer_line);
					continue;
				}
				add_actionCode(pointer_line, code, ptr_ic, count_line, temp_action, head);
			}
		}
		free(pointer_line);
	}

	if (error == 1 || count_line == 0)
	{
        fclose(file);
		free(pointer_line);
		return 0;
	}
    second_scan(file, *ptr_ic, *ptr_dc, code, dataImage, head,filename);
	
    fclose(file);
    
	return 1;
}

void type_start(action act, machineCode *code, int *IC) /*addind the first "word" in to the array code & initalize funct in the second word*/
{
	init_code_unsigned(code, IC, 1);
	code[*IC].chosen_word.data.opcode = movebits(act.opcode);
	code[*IC].chosen_word.data.A = 1; /*initalize the first word*/
	print_stat(code, *IC);
	(*IC)++;
	if (!strcmp(act.name, "stop") || !strcmp(act.name, "rts")) /* the function is stop or rts so in this point we finished building the memory words for the current line */
		return;
	init_code_unsigned(code, IC, 2);
	code[*IC].chosen_word.code.funct = act.func; /*act.name NOT equal to rts , stop , NONE*/
	code[*IC].chosen_word.code.A = 1;
}
/*this function initalize (1)sourceAddress,(2)sourceRegister and (3)destinationAddress,(4)destinationRegister according to the number of operands.
if num_of_operands=1 we initalize (1),(2) ,else if num_of_operands=2 we initalize (1),(2),(3),(4) */
void type_to_SD(int address_source, int address_destination, operands *ready, int *IC, machineCode *code, symbol_item *head)
{
	symbol_item *symbol = NULL;
	char *word = ready->first_operand;
	char* word_second = NULL;
	int is_imm = 0;
	int is_direct = 0;
	int is_index = 0;
	if (ready->num_of_operands == 1)
	{
		switch (address_source)
		{
		case 0:		/*immediate address*/
			word++; /*because word[0] = '#'*/
			code[(*IC)].chosen_word.code.destinationAddress = 0;
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			code[(*IC)].chosen_word.data.opcode = atoi(word);
			code[*IC].chosen_word.data.A = 1;
			print_stat(code, *IC);
			(*IC)++;
			break;
		case 1: /*  direct address */
			code[(*IC)].chosen_word.code.destinationAddress = 1;
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			if ((symbol = symbol_exist(word, head)) == NULL)
			{
				code[(*IC)].undefinedSymbol = (*IC == 0) ? -1 : *IC;
				strcpy(code[(*IC)].missing_symbol_name, word);
				print_stat(code, *IC);
				(*IC)++;
				init_code_unsigned(code, IC, 1);
				code[(*IC)].undefinedSymbol = *IC;
				strcpy(code[(*IC)].missing_symbol_name, word);
				print_stat(code, *IC);
				(*IC)++;
			}
			else /*  word: Is symbol */
			{	 /* adding two memory words , the first word = symbol.base , and the second word = symbol.offset */
				code[(*IC)].chosen_word.data.opcode = symbol->baseAddress;
				if (symbol->attribute == EXTERNAL_SYMBOL || symbol->attribute2 == EXTERNAL_SYMBOL)
				{
					code[(*IC)].chosen_word.data.E = 1; /*EXTERNAL_SYMBOL*/
					symbol->extline = (*IC-1);
				}
				else
				{
					code[(*IC)].chosen_word.data.R = 1;
				}
				print_stat(code, *IC);
				(*IC)++;
				init_code_unsigned(code, IC, 1);
				code[(*IC)].chosen_word.data.opcode = symbol->offset;
				if (symbol->attribute == EXTERNAL_SYMBOL || symbol->attribute2 == EXTERNAL_SYMBOL)
				{
					code[(*IC)].chosen_word.data.E = 1; /*EXTERNAL_SYMBOL*/
					symbol->extline = *IC-1;
				}
				else
				{
					code[(*IC)].chosen_word.data.R = 1;
				}
				print_stat(code, *IC);
				(*IC)++;
			}
			break;
		case 2: /* index address */
			code[(*IC)].chosen_word.code.destinationRegister = ready->inside_brackets;
			code[(*IC)].chosen_word.code.destinationAddress = 2;
			is_direct = 1;
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			symbol = symbol_exist(word, head);
			type_second_term(0, is_direct, 0, symbol, ready->first_operand, IC, code);
			break;
		case 3:		/* register address */
					/*skip spaces??*/
			word++; /*because word[0] = 'r'*/
			code[(*IC)].chosen_word.code.destinationRegister = atoi(word);
			code[(*IC)].chosen_word.code.destinationAddress = 3;
			print_stat(code, (*IC));
			(*IC)++;
			break;
		default:
			break;
		} /*----end if(num of operands == 1)------*/
	}
	if (ready->num_of_operands == 2)
	{
		switch (address_source)
		{
		case 0:		/*immediate address*/
			word++; /*because word[0] = '#'*/
			code[(*IC)].chosen_word.code.sourceAddress = 0;
			is_imm = atoi(word);
			break;
		case 1: /*  direct address */
			code[(*IC)].chosen_word.code.sourceAddress = 1;
			symbol = symbol_exist(word, head);
			is_direct = 1;
			break;
		case 2: /* index address */
			code[(*IC)].chosen_word.code.sourceRegister = ready->inside_brackets;
			code[(*IC)].chosen_word.code.sourceAddress = 2;
			is_index = 1;
			break;
		case 3:		/* register address */
			word++; /*because word[0] = 'r'*/
			code[(*IC)].chosen_word.code.sourceRegister = atoi(word);
			code[(*IC)].chosen_word.code.sourceAddress = 3;
			break;
		default:
			break;
		}
		word_second = ready->second_operand;
		switch (address_destination) /*----second operand----*/
		{
		case 0:		/*immediate address*/
			word_second++; /*because word[0] = '#'*/
			code[(*IC)].chosen_word.code.destinationAddress = 0;
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			if (is_direct == 1) {
				type_second_term(0, is_direct,0, symbol, word, IC, code);
			}
			if (is_imm != 0) {
				type_second_term(is_imm, 0,0, NULL, word, IC, code);
			}
			if(is_index !=0){
				symbol = symbol_exist(word, head);
				type_second_term(is_imm, is_direct, is_index, symbol, ready->first_operand, IC, code);
			}
			code[(*IC)].chosen_word.data.opcode = atoi(word);
			code[*IC].chosen_word.data.A = 1;
			(*IC)++;
			break;
		case 1: /*  direct address */
			code[(*IC)].chosen_word.code.destinationAddress = 1;
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			if (is_direct == 1) {
				type_second_term(0, is_direct,0, symbol, word, IC, code);
			}
			if (is_imm != 0) {
				type_second_term(is_imm, 0,0, NULL, word, IC, code);
			}
			if(is_index !=0){
				symbol = symbol_exist(word, head);
				type_second_term(is_imm, is_direct, is_index, symbol, ready->first_operand, IC, code);
			}
			init_code_unsigned(code, IC, 1);
			if ((symbol = symbol_exist(word_second, head)) == NULL)
			{
				code[(*IC)].undefinedSymbol = *IC;
				strcpy(code[(*IC)].missing_symbol_name, word_second);
				print_stat(code, *IC);
				(*IC)++;
				init_code_unsigned(code, IC, 1);
				code[(*IC)].undefinedSymbol = *IC;
				strcpy(code[(*IC)].missing_symbol_name, word_second);
				print_stat(code, *IC);
				(*IC)++;
			}
			else /*  word: Is symbol */
			{	 /* adding two memory words , the first word = symbol.base , and the second word = symbol.offset */
				code[(*IC)].chosen_word.data.opcode = symbol->baseAddress;
				if (symbol->attribute == EXTERNAL_SYMBOL || symbol->attribute2 == EXTERNAL_SYMBOL)
				{
					code[(*IC)].chosen_word.data.E = 1; /*EXTERNAL_SYMBOL*/
					symbol->extline = *IC-1;
				}
				else
				{
					code[(*IC)].chosen_word.data.R = 1;
				}
				print_stat(code, *IC);
				(*IC)++;
				init_code_unsigned(code, IC, 1);
				code[(*IC)].chosen_word.data.opcode = symbol->offset;
				if (symbol->attribute == EXTERNAL_SYMBOL || symbol->attribute2 == EXTERNAL_SYMBOL)
				{
					code[(*IC)].chosen_word.data.E = 1; /*EXTERNAL_SYMBOL*/
					symbol->extline = *IC-1;
				}
				else
				{
					code[(*IC)].chosen_word.data.R = 1;
				}
				print_stat(code, *IC);
				(*IC)++;
			}
			break;
		case 2: /* index address */
			code[(*IC)].chosen_word.code.destinationRegister = ready->inside_brackets;
			code[(*IC)].chosen_word.code.destinationAddress = 2;
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			if (is_direct == 1) {
				type_second_term(0, is_direct , 0, symbol, word, IC, code);
			}
			if (is_imm != 0) {
				type_second_term(is_imm, 0,0, NULL, word, IC, code);
			}
			if(is_index !=0){
				symbol = symbol_exist(word, head);
				type_second_term(is_imm, is_direct, is_index, symbol, ready->first_operand, IC, code);
			}
				break;
		case 3:		/* register address */
			word_second++; /*because word[0] = 'r'*/
			code[(*IC)].chosen_word.code.destinationRegister = atoi(word_second);
			code[(*IC)].chosen_word.code.destinationAddress = 3;
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			if (is_direct == 1) {
				type_second_term(0, is_direct,0, symbol, word, IC, code);
			}
			if (is_imm != 0) {
				type_second_term(is_imm, 0,0, NULL, word, IC, code);
			}
			if(is_index !=0){
				symbol = symbol_exist(word, head);
				type_second_term(is_imm, is_direct, is_index, symbol, ready->first_operand, IC, code);
			}
				break;
		default:
			break;
		}
	}
}
void type_second_term(int is_imm_value, int is_direct,int is_index, symbol_item *symbol, char *word, int *IC, machineCode *code)
{
	if (is_direct == 1 || is_index == 1) /* metod address for the first operand is direct address */
	{
		if (!symbol)
		{
			code[(*IC)].undefinedSymbol = *IC;
			strcpy(code[(*IC)].missing_symbol_name, word);
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			code[(*IC)].undefinedSymbol = *IC;
			strcpy(code[(*IC)].missing_symbol_name, word);
			print_stat(code, *IC);
			(*IC)++;
		}
		else /*  word: Is symbol */
		{	 /* adding two memory words , the first word = symbol.base , and the second word = symbol.offset */
			code[(*IC)].chosen_word.data.opcode = symbol->baseAddress;
			if (symbol->attribute == EXTERNAL_SYMBOL || symbol->attribute2 == EXTERNAL_SYMBOL)
			{
				code[(*IC)].chosen_word.data.E = 1; /*EXTERNAL_SYMBOL*/
				symbol->extline = *IC-1;
			}
			else
			{
				code[(*IC)].chosen_word.data.R = 1;
			}
			print_stat(code, *IC);
			(*IC)++;
			init_code_unsigned(code, IC, 1);
			code[(*IC)].chosen_word.data.opcode = symbol->offset;
			if (symbol->attribute == EXTERNAL_SYMBOL || symbol->attribute2 == EXTERNAL_SYMBOL)
			{
				code[(*IC)].chosen_word.data.E = 1; /*EXTERNAL_SYMBOL*/
				symbol->extline = *IC-1;
			}
			else
			{
				code[(*IC)].chosen_word.data.R = 1;
			}
			print_stat(code, *IC);
			(*IC)++;
		}
	}			   /*end if flag2==1*/
	if (is_imm_value != 0) /*metod address for the first operand is immediate address*/
	{
      if(is_imm_value<0)
	  {
		code[(*IC)].chosen_word.data.opcode = complete_to_two(is_imm_value);
	  }
	 else{
		code[(*IC)].chosen_word.data.opcode = is_imm_value;
	  }
	 code[*IC].chosen_word.data.A = 1;
  	 print_stat(code, (*IC));
	 (*IC)++;
   }

}

int detect_action(char *word) /* return 0 if word="MOV",and 1 if word="CMP" , etc...*/
{
	int i;
	for (i = 0; i < NUMBER_ACTIONS; i++)
	{
		if (!strcmp(word, actions[i].name))
		{
			return i;
		}
	}
	return -1;
}
int address_is_valid(int address_me, action corrent, int count_line, char *value) /*checkes if the address method is valid according to the param value and the function "corrent"*/
{
	int address0[4] = action_block0;
	int address2[4] = action_block2;
	int address4[4] = action_block4;
	if (!strcmp(value, "destination")) /*if value = "destination"*/
	{
		switch (address_me)
		{
		case 0: /*immediate address*/
			if (!isEqual(corrent.destination, address4))
			{
				printf("ERROR - unvalid method address ,in line:%d \n", count_line);
				error = 1;
				return 0;
			}
			return 1;
			break;
		case 1: /* direct address */
		case 2: /* index address */
			if (isEqual(corrent.destination, address0))
			{
				printf("ERROR - unvalid method address ,in line:%d \n", count_line);
				error = 1;
				return 0;
			}
			return 1;
			break;
		case 3: /*register address*/
			if (isEqual(corrent.destination, address0) || isEqual(corrent.destination, address2))
			{
				printf("ERROR - unvalid method address ,in line:%d \n", count_line);
				error = 1;
				return 0;
			}
			return 1;
			break;
		default:
			break;
		}
		return 0;
	}
	else /*   value = "Source"   */
	{
		switch (address_me)
		{
		case 0: /*immediate address*/
			if (!isEqual(corrent.source, address4))
			{
				printf("ERROR - unvalid method address ,in line:%d \n", count_line);
				error = 1;
				return 0;
			}
			return 1;
			break;
		case 1: /* for direct address */
		case 2: /*2 for index address*/
			if (isEqual(corrent.source, address0))
			{
				printf("ERROR - unvalid method address ,in line:%d \n", count_line);
				error = 1;
				return 0;
			}
			return 1;
			break;
		case 3: /*register address*/
			if (isEqual(corrent.source, address0) || isEqual(corrent.source, address2))
			{
				printf("ERROR - unvalid method address ,in line:%d \n", count_line);
				error = 1;
				return 0;
			}
			return 1;
			break;
		default:
			break;
		}
		return 0;
	}
}

int add_actionCode(line *source_line, machineCode *code, int *IC, int count_line, action corrent, symbol_item *head) /*add memory words to the array code , in this point the first word of the current line is a name of function*/
{
	operands *ready_operands;
	int function;
	line *temp_line;
	int address_source;
	int address_destination;
	temp_line = copy_line(source_line);
	function = detect_action(temp_line->first_word);

	switch (function)
	{
	case MOV: /*    with two operand    */
	case CMP:
	case ADD:
	case SUB:
	case LEA:
		type_start(corrent, code, IC);
		ready_operands = build_operands(temp_line, 2);
		if (ready_operands->num_of_operands == -1)
		{
			return 0;
		}
		address_source = address_meth(ready_operands->first_operand, temp_line->line_number, head);
		address_destination = address_meth(ready_operands->second_operand, temp_line->line_number, head);
		if (address_is_valid(address_source, corrent, count_line, "source") &&
			address_is_valid(address_destination, corrent, count_line, "destination"))
		{
			type_to_SD(address_source, address_destination, ready_operands, IC, code, head);
		}

		break;
	case CLR: /*    with one operand    */
	case NOT:
	case INC:
	case DEC:
	case JMP:
	case BNE:
	case JSR:
	case RED:
	case PRN:
		type_start(corrent, code, IC);
		ready_operands = build_operands(temp_line, 1);
		if (ready_operands->num_of_operands == -1)
		{
			return 0;
		}
		address_source = address_meth(ready_operands->first_operand, temp_line->line_number, head);
		if (address_is_valid(address_source, corrent, count_line, "destination"))
		{
			type_to_SD(address_source, 0, ready_operands, IC, code, head);
		}
		break;
	case RTS: /*    with zero operand    */
	case STOP:
		type_start(corrent, code, IC);
		break;
	default:
		break;
	}
	return 1;
}
/* Reinit symbol values with the new ICF */
int re_init_all_symbols(symbol_item *head, int ICF)
{
	symbol_item *temp = head;
	while (temp->next && temp)
	{
		if (temp->attribute == DATA_SYMBOL && temp != NULL)
		{
			init_symbol(temp, temp->attribute, temp->attribute2, ICF);
		}
		temp = temp->next;
	}
	return 0;
}

int init_symbol_step2(symbol_item *head, int IC) /*initalize the symbol - step2*/
{
	int offset;
	int base;
	base = init_base(IC);
	offset = init_offset(base, IC);
	head->offset = offset;
	head->baseAddress = base;
	return 1;
}

int init_symbol(symbol_item *head, int att1, int att2, int IC) /*initalize the symbol - step1*/
{
	head->value = IC;
	head->attribute = att1;
	head->attribute2 = att2;
	
	init_symbol_step2(head, IC);
	return 1;
}

/*initalize the base of the symbol */
int init_base(int x)
{
	unsigned int temp = ((x / 16) * 16);
	return temp;
}
/*initalize the offset of the symbol*/
int init_offset(int x, int IC)
{
	unsigned int temp = IC - x;
	return temp;
}

int isEqual(int a1[4], int a2[4]) /* checks if a1 and a2 are equal , a1 and a2 are equal if a1[i]=a2[i] for each i (0<=i<=4)*/
{
	int i;
	for (i = 0; i < 4; i++)
	{
		if (a1[i] != a2[i])
			return 0;
	}
	return 1;
}

int add_stringCode(line *checked_line, machineCode *dataImage, int *DC, int count_line) /*adding memory words to the array dataImage , in this point the current line contain the word ".string"*/
{
	int len = sizeof(checked_line->content);
	int i = 1;
	char *ptr = strchr(checked_line->content, '\"');
	if (!ptr)
	{
		error = 1;
		printf("ERROR - missing charactor \" in line  %d \n", count_line);
		return 0;
	}
	while (i < (len - 1) && ptr[i] != '\"')
	{
		if (isalpha(ptr[i]) == 0) /*check if the string is valid*/
		{
			error = 1;
			printf("ERROR - invalid string in line \" %d \n", count_line);
			return 0;
		}
		else
		{
			init_code_unsigned(dataImage, DC, 1);
			dataImage[*DC].chosen_word.data.opcode = ptr[i];
			dataImage[*DC].chosen_word.data.A = 1;
			print_stat(dataImage, *DC);
			(*DC)++;
			i++;
		}
		if ((i == (len - 1)) && (ptr[i] != '\"'))
		{
			error = 1;
			printf("ERROR - missing charactor \" at the end of string in line %d \n", count_line);
			return 0;
		}
	}
	ptr = ptr + i + 1;
	/* if there is extra words after the string */
	if (!end_of_line(ptr, checked_line->line_number))
	{
		error = 1;
		printf("ERROR - unnecessary words after \" in line %d \n", count_line);
		return 0;
	}
	/* adding the \0 charactor after the end of string */
	init_code_unsigned(dataImage, DC, 1);
	dataImage[*DC].chosen_word.data.opcode = 0;
	dataImage[*DC].chosen_word.data.A = 1;
	print_stat(dataImage, *DC);
	(*DC)++;
	i++;
	return 1;
}

int is_register(char *str) /* checks if it is registerd ,if str="ri" and (i>=0 , i<=16) the function returns i else it returns -1  */
{
	int num;

	if (str[0] != 'r')
	{
		return -1;
	}
	num = atoi(++str);
	if (num == 0 && str[0] != '0')
	{
		return -1;
	}
	if (num < 0 || num > 16)
	{
		return -1;
	}
	switch (num)
	{
	case 0:
		return 0;
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 3;
	case 4:
		return 4;
	case 5:
		return 5;
	case 6:
		return 6;
	case 7:
		return 7;
	case 8:
		return 8;
	case 9:
		return 9;
	case 10:
		return 10;
	case 11:
		return 11;
	case 12:
		return 12;
	case 13:
		return 13;
	case 14:
		return 14;
	case 15:
		return 15;
	case 16:
		return 16;
	default:
		return -1;
	}
	return 1;
}

int add_dataCode(char *checked_line, machineCode *dataImage, int *DC, int count_line) /*adding memory words to the array dataImage , in this point the current line contain the word ".data"*/
{
	int comma = 1;
	int onepara = 0;
	char *ptr = checked_line;
	int code_num;
	ptr = skip_spaces(ptr);
	if (!ptr)
	{
		printf("Error , no parameters after .data , in line: %d \n", count_line);
		error = 1;
		return 0;
	}
	while (ptr)
	{
		ptr = skip_spaces(ptr);
		if (!ptr) /* ptr=NULL - end of line*/
		{
			if (comma == 0)
			{
				return 1;
			}
			else
			{
				printf("Error , comma at the end , in line: %d \n", count_line);
				error = 1;
				return 0;
			}
		}
		if (*ptr == ',')
		{
			if (comma == 1 && onepara != 0)
			{
				printf("Error 2 commas , in line: %d \n", count_line);
				error = 1;
				return 0;
			}
			else
			{
				if (comma == 1 && onepara == 0)
				{
					printf("Error  comma before the first para , in line: %d \n", count_line);
					error = 1;
					return 0;
				}
				comma = 1;
			}
			ptr++;
		}
		else /* not a comma */
		{
			if (*ptr == '+')
			{
				ptr++;
			}
			if (isdigit(*ptr) == 0)
			{
				if (*ptr != '-')
				{
					printf("Error not a number or a comma , in line: %d \n", count_line);
					error = 1;
					return 0;
				}
			}
			if (comma)
			{
				code_num = atoi(ptr);
				if (code_num < 0)
				{
					code_num = complete_to_two(code_num);
					init_code_unsigned(dataImage, DC, 1);
					dataImage[*DC].chosen_word.data.opcode = code_num;
					dataImage[*DC].chosen_word.data.A = 1;
					print_stat(dataImage, (*DC));
					(*DC)++;
					ptr++;
				}
				else
				{
					init_code_unsigned(dataImage, DC, 1);
					dataImage[*DC].chosen_word.data.opcode = code_num;
					dataImage[*DC].chosen_word.data.A = 1;
					print_stat(dataImage, (*DC));
					(*DC)++;
				}
				comma = 0;
				while (isdigit(*ptr) != 0)
				{
					ptr++;
				}
			}
			else
			{
				printf("Error no comma , in line:%d\n", count_line);
				error = 1;
				return 0;
			}
			skip_spaces(ptr);
			if (*ptr == '\0' || *ptr == '\n')
			{
				break;
			}
		}
	}
	return 1;
}
/* This function tries to find the addressing method of a given operand and returns -1 if it was not found */
int address_meth(char *word, int line_num, symbol_item *head)
{
	char *start_word;
	char *end_word;
	char *brackets_exist;
	char *close_brackets_exist;

	if (word[0] == '#') /*  immediate address */
	{
		if (atoi(word + 1) != 0 || word[1] == '0')
			return 0; /*  immediate address */
		else		  /* not a number after */
		{
			printf("ERROR : invalid number after # in line %d \n", line_num);
			error = 1;
			return -1;
		}
	}

	if (is_register(word) != -1) /*  direct register address */
		return 3;				 /*  direct register address */
	else
	{
		brackets_exist = strchr(word, '['); /*  true if found '[' */
		close_brackets_exist = strchr(word, ']');
		start_word = strtok(word, "[");
		if (!brackets_exist)
		{
			if (is_symbol_def(start_word) == 2)
			{
				return 1; /*  direct address */
			}
			else
			{
				printf("ERROR : invalid operator %d \n", line_num);
				error = 1;
				return -1;
			}
		}
		else
		{
			brackets_exist++;
			if (is_symbol_def(start_word) == 2) /*  index address  */
			{
				start_word = strtok(NULL, "");
				end_word = strtok(brackets_exist, "]"); /*   word inside [word]   */
				if (close_brackets_exist)
				{
					if (is_register(end_word) != -1)
						return 2; /*  index address */
				}
				else
				{
					printf("ERROR : Missing closing brackets %d \n", line_num);
					error = 1;
					return -1;
				}
			}
			else
			{
				printf("ERROR : Not a symbol defenition %d \n", line_num);
				error = 1;
				return -1;
			}
		}
	}
	return -1;
}

char *point_next(char *str, char *delim) /*This function returns a pointer to the first occurrence of the character delim in the string str*/
{
	char *token;
	/* get the first token */
	token = strtok(str, delim);
	if (!token)
	{
		printf(" missing %s delimiter \n", delim);
		error = 1;
		return token;
	}
	token = strtok(NULL, delim);
	return token;
}

int end_of_line(char *pointer_line, int count_line) /*this function returns 1 if the line ended , 0 otherwise*/
{
	skip_spaces(pointer_line);
	if (*pointer_line == '\0' || *pointer_line == '\n')
	{
		return 1;
	}
	printf("ERROR - there are too many words | line: %d \n", count_line);
	error = 1;
	return 0;
}

/*this function skip spaces */
char *skip_spaces(char *line)
{
	while (*line != '\0' && isspace(*line) != 0)
	{
		line++;
	}
	return line;
}

int complete_to_two(int x) /*returns the complement to two of x*/
{
	return x & 0xffff;
}
int is_symbol_def(char *word)
{
	/* 0: symbol not legal*/
	/* 1: symbol */
	/* 2: not symbol def == possible symbol */
	char last;
	int len;
	int i = 0;
	len = strlen(word);
	last = *(word + len - 1);
	if (len >= SYMBOL_MAXLEN)
	{
		return 0;
	}
	while (i == (len - 1))
	{
		if (!(isalpha(word[i]) != 0 || isdigit(word[i]) != 0))
		{
			return 0;
		}
		i++;
	}
	if (last == ':')
	{
		word[len] = '\0';
		return 1;
	}
	return 2;
}

/*the function receives a line without the function's name and after we check the propriety of the operands */
operands *build_operands(line *source_line, int num)
{
	operands *new_op = (operands *)malloc(sizeof(operands));
	char rest[LINE_LEN];
	char *inside;
	char *word = (char *)malloc(sizeof(char) * strlen(source_line->content) + 1);
	line *temp_line = copy_line(source_line);
	rest[0] = '\0';
	if (num == 1)
	{
		sscanf(temp_line->content, "%s %s", word, rest);
		word = skip_spaces(word);
		inside = strchr(temp_line->content, '[');
		if (inside)
		{
			inside++;
			inside++;
			new_op->inside_brackets = atoi(inside);
		}
		if (*word != '\0' && !rest[0])
		{
			strcpy(new_op->first_operand, word);
			new_op->num_of_operands = 1;
			free(word);
			return new_op;
		}
		else
		{
			if (!word)
				printf("\n Error: missing operand \n");
			else
				printf(" Error: too many opperands \n");
			error = 1;
			new_op->num_of_operands = -1;
			free(word);
			return new_op;
		}
	}

	if (num == 2)
	{
		word = strtok(temp_line->content, ","); /*	collecting first opperand into word	*/
		if (!word)
		{
			printf("\n Error: missing operand \n");
			error = 1;
			new_op->num_of_operands = -1;
			free(word);
			return new_op;
		}
		strcpy(new_op->first_operand, word); /*	saving first opperand	*/
		word = strtok(NULL, " ");			 /*	collecting second opperand into word	*/
		if (!word)
		{
			printf("\n Error: missing operand \n");
			error = 1;
			new_op->num_of_operands = -1;
			free(word);
			return new_op;
		}
		sscanf(word, "%s", rest); /*	trasforming rest <==> word	*/
		word = strtok(NULL, " ");
		if (!word)
		{
			strcpy(new_op->second_operand, rest); /*	saving first opperand	*/
			new_op->num_of_operands = 2;
			free(word);
			return new_op;
		}
		else
		{
			printf(" too many opperands \n");
			error = 1;
			new_op->num_of_operands = -1;
			free(word);
			return new_op;
		}
	}
	new_op->num_of_operands = -1;
	free(word);
	return new_op;
}

int is_comment_first(char *corrent_line)

{ /* return 1:is comment : return 0: not comment */
	char ch[LINE_LEN];
	sscanf(corrent_line, "%s", ch);
	if (ch[0] == ';')
		return 1;
	return 0;
}
/*check if the symbol is exist
if the symbol is exit we return it , & if the symbol is not in the SymbolList the function returns NULL*/
symbol_item *symbol_exist(char *word, symbol_item *head)
{
	symbol_item *temp;
	temp = head;
	while (temp->next != NULL)
	{
		if (!strcmp(temp->symbol_name, word))
		{
			return temp;
		}
		temp = temp->next;
		if (temp->next == NULL)
		{
			if (!strcmp(temp->symbol_name, word))
			{
				return temp;
			}
		}
	}
	return NULL;
}

/*adding symbol to the symbol list*/
symbol_item *add_symbol(symbol_item *head, char *name, int in_value)
{
	symbol_item *temp = head;
	if (head == NULL)
	{
		head = (symbol_item *)malloc(sizeof(symbol_item));
		strcpy(head->symbol_name, name);
		head->value = in_value;
		head->next = NULL;
		return head;
	}
	while (temp->next)
	{
		temp = temp->next;
	}
	temp->next = (symbol_item *)malloc(sizeof(symbol_item));
	strcpy(temp->next->symbol_name, name);
	temp->next->value = in_value;
	temp->next->next = NULL;
	return temp->next;
}

line *copy_line(line *old_l) /*return a copied line that contain the same string as line:old_l*/
{
	line *new_l = (line *)malloc(sizeof(line));
	if (!new_l)
	{
		printf("malloc failed");
		return NULL;
	}
	new_l->first_word = (char *)malloc(sizeof(char) * SYMBOL_MAXLEN);
	new_l->content = (char *)malloc(sizeof(char) * LINE_LEN);
	if (strlen(old_l->content))
		strcpy(new_l->content, old_l->content);
	if (strlen(old_l->first_word))
		strcpy(new_l->first_word, old_l->first_word);
	new_l->line_number = old_l->line_number;
	return new_l;
}
/*if word is a name of action then return the action else it returns NULL */
action is_action(char *word)
{
	int i;
	if (word[strlen(word) - 1] == '\n')
	{
		word[strlen(word) - 1] = '\0';
	}
	for (i = 0; i < NUMBER_ACTIONS - 1; i++)
	{
		if (!strcmp(actions[i].name, word))
			return actions[i];
	}
	return actions[NUMBER_ACTIONS - 1];
}

line *split_line(line *new_line, char *skip, int count_line)
{
	char *corrent;
	corrent = strtok(new_line->content, skip);
	if (!corrent)
	{
		printf(" Error: missing \"%s\" \n", skip);
		return NULL;
	}
	memset(new_line->first_word, '\0', strlen(new_line->first_word));
	strcpy(new_line->first_word, corrent);
	corrent = strtok(NULL, "");
	if (!corrent)
	{
		if (!strcmp(new_line->first_word, "stop\n") || !strcmp(new_line->first_word, "rts\n"))
		{
			return new_line;
		}
		printf(" Error: missing content after \"%s\" \n", skip);
		return NULL;
	}
	memset(new_line->content, '\0', strlen(new_line->content));
	strcpy(new_line->content, corrent);
	return new_line;
}

/* separate the first word from the rest of the line and save each part*/
line *create_line(char *source_line, char *skip, int count_line)
{
	line *new_line;
	char *corrent;

	if (source_line)
	{
		new_line = (line *)malloc(sizeof(line));
		new_line->first_word = (char *)malloc(sizeof(char) * (SYMBOL_MAXLEN + 1));
		new_line->content = (char *)malloc(sizeof(char) * (strlen(source_line) + 1));
		corrent = strtok(source_line, skip);
		if (!corrent)
		{
			printf(" Error: missing %s ", skip);
			return NULL;
		}
		strcpy(new_line->first_word, corrent);
		corrent = strtok(NULL, "");
		if (corrent)
		{
			strcpy(new_line->content, corrent);
		}
	}
	else
	{
		return NULL;
	}
	new_line->line_number = count_line;
	return new_line; /* return the expected line  */
}

void init_code_unsigned(machineCode *code, int *IC, int word_num) /*initalize all the fields of the memory word(in line IC) into zero*/
{
	code[*IC].line_num = 0;
	code[*IC].undefinedSymbol = 0;
	if (word_num == 1)
	{

		code[*IC].chosen_word.data.A = 0;
		code[*IC].chosen_word.data.R = 0;
		code[*IC].chosen_word.data.E = 0;
		code[*IC].chosen_word.data.S = 0;
		code[*IC].chosen_word.data.opcode = 0;
		code[*IC].chosen_word.code.A = 0;
		code[*IC].chosen_word.code.R = 0;
		code[*IC].chosen_word.code.E = 0;
		code[*IC].chosen_word.code.S = 0;
		code[*IC].chosen_word.code.destinationAddress = 0;
		code[*IC].chosen_word.code.sourceAddress = 0;
		code[*IC].chosen_word.code.sourceRegister = 0;
		code[*IC].chosen_word.code.destinationRegister = 0;
		code[*IC].chosen_word.code.funct = 0;
	}
	if (word_num == 2)
	{
		code[*IC].chosen_word.code.A = 0;
		code[*IC].chosen_word.code.R = 0;
		code[*IC].chosen_word.code.E = 0;
		code[*IC].chosen_word.code.S = 0;
		code[*IC].chosen_word.code.destinationAddress = 0;
		code[*IC].chosen_word.code.sourceAddress = 0;
		code[*IC].chosen_word.code.sourceRegister = 0;
		code[*IC].chosen_word.code.destinationRegister = 0;
		code[*IC].chosen_word.code.funct = 0;
	}
}

void print_stat(machineCode *code, int IC) /*this function prints the status for the memory word in line IC*/
{
	printf("----------------- ADDED new memory word: -----------------\n");
	printf("line_num: %d\n\n ", IC);
	printf("undefinedSymbol: %d\t opcode: %d\n\n", code[IC].undefinedSymbol, code[IC].chosen_word.data.opcode);
	printf("A: %d\n ", code[IC].chosen_word.code.A);
	printf("R: %d\n ", code[IC].chosen_word.code.R);
	printf("E: %d\n ", code[IC].chosen_word.code.E);
	printf("S: %d\n ", code[IC].chosen_word.code.S);
	printf("destinationAddress: %d\n ", code[IC].chosen_word.code.destinationAddress);
	printf("sourceAddress: %d\n ", code[IC].chosen_word.code.sourceAddress);
	printf("sourceRegister: %d\n ", code[IC].chosen_word.code.sourceRegister);
	printf("destinationRegister: %d\n ", code[IC].chosen_word.code.destinationRegister);
	printf("funct: %d\n\n ", code[IC].chosen_word.code.funct);
	printf("-------------------------------------------------\n");
}