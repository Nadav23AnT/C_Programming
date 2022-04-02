#ifndef PREPROCESSOR_DEF
#define PREPROCESSOR_DEF

#include "utils.h"

#define LINE_SIZE 81
#define NAME_SIZE 255
#define NAME_LEN_MAC 20

typedef struct node
{
    char macro_name[NAME_LEN_MAC];
    char text[LINE_SIZE];
    int last_one;
    struct node *next;
} macro;

/*------------------ PreProcessor functions ------------------*/
int pre_processor(char *input_file_name);
int is_macro_def(char *corrent_line);
int is_macro(char *corrent_line, macro *head);
int is_comment(char *corrent_line);
void push(macro *head, char *name, char *content);
int is_empty(macro *corrent);

#endif