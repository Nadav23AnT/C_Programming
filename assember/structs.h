#ifndef HEADERSTRUCTS_H
#define HEADERSTRUCTS_H
#define movebits(x) (1<<(x))

typedef struct action
{
    char* name;
    int func;
    int opcode;
    int source[4];
    int destination[4];
    int numOfOpera;
} action;

#define NUMBER_ACTIONS 17
#define IC_INIT 100
#define DC_INIT 0
#define SYMBOL_MAXLEN 31
#define LINE_LEN 81
/*action_blocki (i=4,3,2,0) are macros ,
 * action_blocki[j]=1 indicates that address method j is valid,
 * action_blocki[j]=0 if address method j is not valid */

typedef enum symbol_attribute
{
    CODE_SYMBOL,
    DATA_SYMBOL,
    EXTERNAL_SYMBOL,
    ENTRY_SYMBOL,
    NONE
} symbol_attribute;

typedef struct symbol_item
{
    char symbol_name[SYMBOL_MAXLEN];
    int value;
    unsigned int baseAddress;
    unsigned int offset;
    unsigned int extline;
    symbol_attribute attribute;
    symbol_attribute attribute2;
    struct symbol_item* next;
} symbol_item;

typedef struct line
{
    int line_number;
    char* first_word;
    char* content;
} line;

enum registry_numbers
{
    r0,
    r1,
    r2,
    r3,
    r4,
    r5,
    r6,
    r7,
    r8,
    r9,
    r10,
    r11,
    r12,
    r13,
    r14,
    r15,
    r16
} register_number;

typedef struct operands
{
    char first_operand[SYMBOL_MAXLEN];
    char second_operand[SYMBOL_MAXLEN];
    int inside_brackets;
    int num_of_operands;
} operands;

enum commands
{
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    JSR,
    RED,
    PRN,
    RTS,
    STOP,
    UNKNOWN
};
typedef struct div4 {
    unsigned int e : 4;
    unsigned int d : 4;
    unsigned int c : 4;
    unsigned int b : 4;
    unsigned int a : 4;
} div4;

typedef struct word1
{
    unsigned int opcode : 16;
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int S : 1;

} word1;

typedef struct word2
{
    unsigned int destinationAddress : 2;
    unsigned int destinationRegister : 4;
    unsigned int sourceAddress : 2;
    unsigned int sourceRegister : 4;
    unsigned int funct : 4;
    unsigned int E : 1;
    unsigned int R : 1;
    unsigned int A : 1;
    unsigned int S : 1;
} word2;

typedef struct machineCode
{
    union chosen_word
    {
        word1 data;
        word2 code;
        div4 ob;
    } chosen_word;
    int undefinedSymbol;
    char missing_symbol_name[SYMBOL_MAXLEN];
    int line_num;
} machineCode;


#endif