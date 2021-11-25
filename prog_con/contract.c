/*Write software that shortens the string to have sequences of consecutive characters according to the Ascii table */
/* Nadav Chen : nadav2282@gmail.com */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLENGTH 81

/* function declaration */
void contract(char s1[], char s2[]);

int main()
{
    char str[MAXLENGTH], str2[MAXLENGTH];

    printf("Hey User! \n Please write a string up to 80 characters long\n");
    while (fgets(str, MAXLENGTH, stdin))
    {
        contract(str, str2); /* calling contract function */
        printf("\nstr1: %s\n", str);
        printf("str2: %s\n", str2);
        break;
    }
    return 0;
}

void contract(char s1[], char s2[])
{
    /* local variable definition */
    /* count; count squence value , s2_spot; s2 corrent index  */
    char symbol = s1[0]; /* corrent charactor */
    int count = 0, s2_spot = 0;
    char start, end;
    int i, j;
    size_t s1_len = strlen(s1);

    for (i = 1; i < s1_len; i++) /* start checking s1 arrray */
    {
        if ((s1[i] - 1) == symbol && isalnum(s1[i]) == isalnum(symbol))
        /* returns 1(true) if next charactor is bigger and match symbol */
        {
            symbol = s1[i];
            count++;
        }
        else /* Keep sequences that are less then 2 */
        {
            if (count == 0) /* squence is one char */
            {
                symbol = s1[i];
                s2[s2_spot] = s1[i];
                s2_spot++;
            }
            else if (count == 1) /* squence is 2 chars */
            {
                s2[s2_spot] = s1[i - count];
                s2_spot++;
                s2[s2_spot] = s1[i];
                s2_spot++;
                symbol = s1[i];
                count = 0;
            }
        }
        if (count > 1) /* start creating squence */
        {
            if (s2_spot == 0) /* if it is the first char in str1 true. */
                start = s1[i - count];
            else
            {
                if (s2[s2_spot - 1] != s1[i - count]) /* check that no duplicate first char in squence */
                { 
                    start = s1[i - count]; /* first char in squence */
                }
                else /* remove duplicate first char in squence */
                {
                    s2_spot--; 
                    start = s1[i - count]; /* first char in squence */
                }
            }
            end = s1[i]; /* last char in squence */
            for (j = i + 1; j < s1_len; j++)
            {
                if ((s1[j] - 1) == symbol && isalnum(s1[j]) == isalnum(symbol)) /* check until the end of the squence */
                {
                    symbol = s1[j];
                    end = symbol;
                    count++;
                }
                else
                    break; /* end of squence */
            }
            s2[s2_spot] = start; /* add first char to str2 */
            s2_spot++;
            s2[s2_spot] = '-'; /* add '-' char to str2 */
            s2_spot++;
            s2[s2_spot] = end; /* add last char of squence to str2 */
            s2_spot++;
            count = 0; /* rebase count */
            i = j - 1; /* redefine i as the end of the last squence */
            symbol = s1[i];
        }
    }
}
