#include "extern.h"
#include "utils.h"

int error2 = 0;
/*  second_scan: main function of second scan in assembler */
int second_scan(FILE *file, int ICF, int DCF, machineCode *code_import, machineCode *data_import, symbol_item *import_head,char *filename)
{
    char corrent_line[LINE_LEN];
    char source_line[LINE_LEN];
    int symbolDef = 0;
    char *ptr_corrent;
    rewind(file);
    while (fgets(source_line, LINE_LEN, file))
    {
        strcpy(corrent_line, source_line);
        ptr_corrent = strtok(corrent_line," ");
        symbolDef = is_symbol_def(ptr_corrent);
        if (symbolDef == 1)
        {
            ptr_corrent = strtok(NULL, ""); /*  continue to next word  */ 
        }
        if (is_guide(ptr_corrent) == 1)
        {
            continue;
        }
        if (is_guide(ptr_corrent) == 2) /*.entry*/
        {
            /*   add entry to symbol  attribute */
            ptr_corrent = strtok(NULL, " ");
            symbol_att(import_head, ptr_corrent);
            continue;
        }
    }
    complete_binarycode(code_import, import_head, ICF);
    if (error2 == 1)
    {
        return 0;
    }
    /*  Build the OUTPUT files    */
    build_obfile(code_import, ICF, data_import, DCF, filename); /*  Objects file    */
    build_exfile(import_head, filename);                        /*  External file   */
    build_entfile(import_head, filename);                       /*  Entry file  */
    return 1;
}
/*  is_guide: checks if the next word is start of a guidance sentence   */
int is_guide(char *word)
{
    if (!strcmp(".string", word) || !strcmp(".data", word) || !strcmp(".extern", word))
    {
        return 1;
    }
    if (!strcmp(".entry", word))
    {
        return 2;
    }
    return 0;
}
/*  symbol_att: add entry attribute to the symbol   */
int symbol_att(symbol_item *import_head, char *ptr_corrent)
{
    symbol_item *temp = symbol_exist(ptr_corrent, import_head);
    if (!temp)
    {
        printf("ERROR - symbol does not exist %s :( \n ", temp->symbol_name);
        error2 = 1;
        return 0;
    }
    temp->attribute2 = ENTRY_SYMBOL;
    return 1;
}
/*  complete_binarycode: complete all undefined Symbols in the machine code table */
int complete_binarycode(machineCode *code_import, symbol_item *import_head, int ICF)
{
    int i;
    symbol_item *temp;
    for (i = 100; i < ICF; i++)
    {
       /* print_stat(code_import, i);*/
        if (code_import[i].undefinedSymbol != 0) /* undefinedSymbol */
        {
            temp = symbol_exist(code_import[i].missing_symbol_name, import_head);
            if (!temp) /* symbol is NOT exist */
            {
                printf("ERROR - symbol is not exist \n ");
                error2 = 1;
                return 0;
            }
            else
            { /* complete the binary code */
                code_import[i].chosen_word.data.opcode = temp->baseAddress;
                if (temp->attribute == EXTERNAL_SYMBOL || temp->attribute2 == EXTERNAL_SYMBOL)
                {
                    code_import[i].chosen_word.data.E = 1;
                    temp->extline = i-1;
                }
                else
                {
                    code_import[i].chosen_word.data.R = 1;
                }
                /*print_stat(code_import, i);*/
                i++;
                code_import[i].chosen_word.data.opcode = temp->offset;
                if (temp->attribute == EXTERNAL_SYMBOL || temp->attribute2 == EXTERNAL_SYMBOL)
                {
                    code_import[i].chosen_word.data.E = 1;
                    temp->extline = i-1;
                }
                else
                {
                    code_import[i].chosen_word.data.R = 1;
                }
                /*print_stat(code_import, i);*/
                i++;
            }
            printf("%04d OB FILE : ICF Write\t A%d-B%d-C%d-D%d-E%d \n",
                i, (code_import[i]).chosen_word.ob.a,
                (code_import[i]).chosen_word.ob.b,
                (code_import[i]).chosen_word.ob.c,
                (code_import[i]).chosen_word.ob.d,
                (code_import[i]).chosen_word.ob.e);
            
        }
        /*print_stat(code_import, i);*/
    }
    return 0;
}
/*  Objects file: output file - include all the machine code */
int build_obfile(machineCode *code_import, int ICF, machineCode *data_import, int DCF, char *input_file_name)
{
    FILE *fp_new;
    char filename[SYMBOL_MAXLEN];
    int i = 0;
    int line_num = 100;

    sprintf(filename, "%s.ob", input_file_name);
    fp_new = fopen(filename, "w"); /* output.ob file */
    fprintf(fp_new ,"\t \t %d %d\n",ICF-100 ,DCF);
    while (line_num < ICF)
    {
        fprintf(fp_new, "%04d  ", line_num);
        fprintf(fp_new, "A%x-B%x-C%x-D%x-E%x \n", 
                (code_import[line_num]).chosen_word.ob.a,
                (code_import[line_num]).chosen_word.ob.b, 
                (code_import[line_num]).chosen_word.ob.c,
                (code_import[line_num]).chosen_word.ob.d, 
                (code_import[line_num]).chosen_word.ob.e);
        printf("%04d OB FILE : ICF Write\t A%x-B%x-C%x-D%x-E%x \n", 
            line_num,(code_import[line_num]).chosen_word.ob.a,
            (code_import[line_num]).chosen_word.ob.b,
            (code_import[line_num]).chosen_word.ob.c,
            (code_import[line_num]).chosen_word.ob.d, 
            (code_import[line_num]).chosen_word.ob.e);
        /* print_stat(code_import, line_num); */
        line_num++;
    } /*line_num=ICF*/
    while (i < DCF)
    {
        fprintf(fp_new, "%04d  ", line_num);
        fprintf(fp_new, "A%x-B%x-C%x-D%x-E%x \n",(data_import[i]).chosen_word.ob.a,
                (data_import[i]).chosen_word.ob.b, (data_import[i]).chosen_word.ob.c,
                (data_import[i]).chosen_word.ob.d, (data_import[i]).chosen_word.ob.e);
        printf("%04d DCF Write\t A%x-B%x-C%x-D%x-E%x \n", line_num, (data_import[i]).chosen_word.ob.a,
            (data_import[i]).chosen_word.ob.b, (data_import[i]).chosen_word.ob.c,
            (data_import[i]).chosen_word.ob.d, (data_import[i]).chosen_word.ob.e);
        /* print_stat(data_import, i); */
        i++;
        line_num++;
    }
    fclose(fp_new);
    return 0;
}
/*  External file: output file - include all symbols with external attribute    */
int build_exfile(symbol_item *import_head, char *input_file_name)
{
    FILE *fp_new;
    char filename[SYMBOL_MAXLEN];
    symbol_item *temp = import_head;

    sprintf(filename, "%s.ext", input_file_name);
    fp_new = fopen(filename, "w"); /* output.ext file */
    temp = temp->next;
    while (temp)
    {
        if (temp->attribute == EXTERNAL_SYMBOL || temp->attribute2 == EXTERNAL_SYMBOL)
        {
            fprintf(fp_new, "%s  BASE  %d  \n", temp->symbol_name, temp->extline);
            fprintf(fp_new, "%s  OFFSET  %d  \n\n\n", temp->symbol_name, temp->extline + 1);
        }
        temp = temp->next;
    }
    fclose(fp_new);
    return 0;
}
/*  Entry file: include all symbols with "entry" attribute */
int build_entfile(symbol_item *import_head, char *input_file_name)
{

    FILE *fp_new;
    char filename[SYMBOL_MAXLEN];
    symbol_item *temp = import_head;

    sprintf(filename, "%s.ent", input_file_name);
    fp_new = fopen(filename, "w"); /* output.ent file */
    while (temp)
    {
        if (temp->attribute == ENTRY_SYMBOL || temp->attribute2 == ENTRY_SYMBOL)
        {
            fprintf(fp_new, "%s , %d , %d  \n", temp->symbol_name, temp->baseAddress, temp->offset);
        }
        temp = temp->next;
    }
    fclose(fp_new);
    return 0;
}
