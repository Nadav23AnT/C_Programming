#include "pre_processor.h"
#include "extern.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    int i = 1;
    FILE *input_source_file;
    char preFile[NAME_SIZE];
    machineCode code[8192];
    machineCode dataImage[8092];
    char source_file_name[LINE_LEN];
    int temp;

    while (i < argc)
    {
        strcpy(source_file_name, argv[i]);
        pre_processor(source_file_name);

	    sprintf(preFile, "%s.am",source_file_name);
        input_source_file = fopen(preFile, "r+");

        printf("Starting first_pass on: %s \n", source_file_name);
        temp = first_pass(input_source_file, code, dataImage,argv[i]);
        printf("**********************\n");
        if (!temp)
        {
            printf("DONE FIRST SCAN [] ERROR FOUND STOPING PROGRAM.\n");
            return 0;
        }
        printf("Starting Second_pass on: %s \n", source_file_name);
        i++;
    }
    printf("\n \t --------- Completed the Assembler program! ---------\n");
    printf("\n \t ----------------- Nadav and Malak -----------------\n");
    printf("\n \t --------------------- Thank you! ---------------------\n");
    return 0;
}