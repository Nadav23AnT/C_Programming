#ifndef HEADEREXTERN_H
#define HEADEREXTERN_H
#include "structs.h"
#include "utils.h"

/*  second_scan: main function of second scan in assembler */
int second_scan(FILE *filename, int ICF, int DCF, machineCode *code_import, machineCode *data_import, symbol_item *import_head,char *);
/*  is_guide: checks if the next word is start of a guidance sentence   */
int is_guide(char *word);
/*  symbol_att: add entry attribute to the symbol   */
int symbol_att(symbol_item *import_head, char *ptr_corrent);
/*  complete_binarycode: complete all undefined Symbols in the machine code table */
int complete_binarycode(machineCode *code_import, symbol_item *import_head, int ICF);
/*  Objects file: output file - include all the machine code */
int build_obfile(machineCode *code_import, int ICF, machineCode *data_import, int DCF, char *input_file_name);
/*  External file: output file - include all symbols with external attribute    */
int build_exfile(symbol_item *import_head, char *input_file_name);
/*  Entry file: include all symbols with "entry" attribute */
int build_entfile(symbol_item *import_head, char *input_file_name);

/*------------------ first scan functions ------------------*/
int first_pass(FILE* filename, machineCode* code_import, machineCode* data_import,char *);
/*addind the first "word" in to the array code & initalize funct in the second word*/
void type_start(action act, machineCode* code, int* IC);
/*this function initalize (1)sourceAddress,(2)sourceRegister and (3)destinationAddress,(4)destinationRegister according to the number of operands.
if num_of_operands=1 we initalize (1),(2) ,else if num_of_operands=2 we initalize (1),(2),(3),(4) */
void type_to_SD(int address_source, int address_destination, operands* ready, int *IC, machineCode* code, symbol_item* head);
/**/
void type_second_term(int is_imm_value, int is_direct, int is_index,symbol_item* symbol, char* word, int* IC, machineCode* code);
/* return 0 if word="MOV",and 1 if word="CMP" , etc...*/
int detect_action(char* word);
/*checkes if the address method is valid according to the param value and the function "corrent"*/
int address_is_valid(int address_me, action corrent, int count_line, char* value);
/*add function code to the array code*/
int add_actionCode(line* source_line, machineCode* code, int* IC, int count_line, action corrent, symbol_item* head);
/* Reinit symbol values with the new ICF */
int re_init_all_symbols(symbol_item* head, int ICF);
int init_symbol_step2(symbol_item* head, int IC);
int init_symbol(symbol_item* head, int att1, int att2, int IC);
/*initalize the base of the symbol */
int init_base(int x);
/*initalize the offset of the symbol*/
int init_offset(int x, int IC);
/* checks if a1 and a2 are equal*/
int isEqual(int a1[4], int a2[4]);
int add_stringCode(line* checked_line, machineCode* dataImage, int* DC, int count_line);
/* checks if it is registerd ,if str="ri" and (i>=0 , i<=16) the function returns i else it returns -1  */
int is_register(char* str);
int add_dataCode(char* checked_line, machineCode* dataImage, int* DC, int count_line);
/* This function tries to find the addressing method of a given operand and returns -1 if it was not found */
int address_meth(char* word, int line_num, symbol_item* head);
char* point_next(char* str, char* delim);
/*this function skip spaces */
char* skip_spaces(char* line);
int complete_to_two(int x);
void init_code_unsigned(machineCode* code, int* IC, int word_num);
void print_stat(machineCode* code, int IC);
/*this function check if word is a symbol defintion
return 0 if the symbol is not legal
return 1 if it is symbol defintion
return 2 if it is NOT a symbol
*/
int is_symbol_def(char* word);
int end_of_line(char* pointer_line, int count_line);
int is_comment_first(char* corrent_line);

/*---- Prototypes functions in first scan: -----*/
operands* build_operands(line* source_line, int num); /*the function receives a line without the function's name and after we check the propriety of the operands */
/*check if the symbol is exist
if the symbol is exit we return it , & if the symbol is not in the SymbolList the function returns NULL*/
symbol_item* symbol_exist(char* word, symbol_item* head);
symbol_item *add_symbol(symbol_item *head, char *name, int value); /*adding symbol to the symbol list*/
line* copy_line(line* old_l);
/*if word is a name of action then return the action else it returns NULL */
action is_action(char* word);
line* split_line(line* new_line, char* skip, int count_line);
/* separate the first word from the rest of the line and save each part*/
line* create_line(char* source_line, char* skip, int count_line);

#endif