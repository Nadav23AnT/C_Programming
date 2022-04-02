#include "pre_processor.h"

int pre_processor(char *input_file_name)
{
	macro *head = (macro *)malloc(sizeof(macro));
	macro *temp;
	char filename[NAME_SIZE];
    char newFile[NAME_SIZE];
	char corrent_line[LINE_SIZE];
	char macroName[LINE_SIZE];
	char macro_content[NAME_SIZE];
	char checked_word[LINE_SIZE];
	char copy_content[NAME_SIZE];
	char copy_name[NAME_SIZE];
	int end_of_macro = 0;
	FILE *fp;
	FILE *fp_new;

	head->next = NULL;
	head->last_one = 1;
	macro_content[0] = '\0';
	
	sprintf(filename, "%s.as", input_file_name);
    fp = fopen(filename, "r");			 /* input file */

    sprintf(newFile, "%s.am", input_file_name);
	fp_new = fopen(newFile, "w+");				 /* output.as file */

	while (fgets(corrent_line, LINE_SIZE, fp) != NULL)

	{
		if (is_macro(corrent_line, head)) /* PASTE */
		{
			sscanf(corrent_line, "%s", macroName);
			temp = head;
			while (strcmp(macroName, temp->macro_name) != 0)
			{
				temp = temp->next;
			}
			fputs(temp->text, fp_new);
		}
		else /*  starting defintion of macro	*/
		{
			if (!(is_comment(corrent_line)) && is_macro_def(corrent_line)) /* if it is start of macro defenition  */
			{

				sscanf(corrent_line, "%s %s", checked_word, macroName); /* macro_name = Macro name*/
				printf("MACRO: Created needs to be macroname: %s MACRO: %s\n", macroName, checked_word);
				while (!end_of_macro)

				{
					memset(corrent_line, '\0', LINE_SIZE);
					memset(checked_word, '\0', LINE_SIZE);
					fgets(corrent_line, LINE_SIZE, fp);
					printf("%s \n %c", corrent_line, corrent_line[0]);
					sscanf(corrent_line, "%s", checked_word);
					if (!strcmp("endm", checked_word)) /* if end of macro def*/
					{
						printf("Copied Macro Content %s\n", macroName);
						break;
					}
					else
					{
						if (macro_content[0] == '\0')
						{
							strcpy(macro_content, corrent_line);
						}
						else
						{
							strcat(macro_content, corrent_line);
						}
						printf("DEBUG: %s \n after content \n after line: %s \n", macro_content, corrent_line);
					}
				}
				strcpy(copy_content, macro_content);	/**/
				strcpy(copy_name, macroName);			/**/
				push(head, copy_name, copy_content);	/* creating new MACRO */
				memset(macroName, '\0', LINE_SIZE);		/* init */
				memset(macro_content, '\0', NAME_SIZE); /* init */
			}

			else /* if it is not macro or a comment : COPY and PASTE */
			{
				fputs(corrent_line, fp_new);
				printf("copied line: %s \n", corrent_line);
			}
		}
		memset(corrent_line, '\0', LINE_SIZE);
	}
	fclose(fp);
	fclose(fp_new);
	free(head);
	printf("PRE_PROCEESSOR: Completed \n");
	return 0;
}

int is_macro_def(char *corrent_line)

{ /* return 1:is macro def: return 0: not macro def*/
	char name[NAME_SIZE];
	sscanf(corrent_line, "%s", name);
	if (!strcmp("macro", name))
	{
		return 1;
	}
	return 0;
}

int is_macro(char *corrent_line, macro *head)

{ /* return 1:is macro : return 0: not macro */
	macro *temp = head;
	char name[NAME_SIZE];
	sscanf(corrent_line, "%s", name);
	while (!is_empty(temp))
	{
		if (!strcmp(temp->macro_name, name)) /* if equal 0 -> 1*/
		{
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

int is_comment(char *corrent_line)

{ /* return 1:is comment : return 0: not comment */
	char ch[LINE_SIZE];
	sscanf(corrent_line, "%s", ch);
	if (ch[0] == ';')
		return 1;
	return 0;
}

int is_empty(macro *corrent) /* if is empty return 1*/
{
	if (!corrent)
	{
		return 1;
	}
	if (corrent->last_one == 1)
	{ /*	just for the first symbol in the list	*/
		return 1;
	}
	else
	{
		return 0;
	}
}
void push(macro *head, char *name, char *content)
{
	macro *current = head;
	if (head->last_one == 1)
	{
		head->last_one = 0;
		strcpy(head->text, content);
		strcpy(head->macro_name, name);
		head->next = NULL;
		return;
	}
	while (current->next)
	{
		current = current->next;
	}
	current->last_one = 0;
	current->next = (macro *)malloc(sizeof(macro));
	strcpy(current->next->text, content);
	strcpy(current->next->macro_name, name);
	current->next->last_one = 1;
}
