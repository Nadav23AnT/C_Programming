#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100
#define ZERO 0
#define VAILD 1
#define CLOSE 2

int main();
int isBalanced(char str[]);
int special_line(char str[], int special);
void print_status(char line_str[], int status);
char pop(char stack[], char str, int i);
void push(char stack[], char str, int i);

int main()
{
    char str[MAX];
    char symbol;
    int status = VAILD;
    int result = VAILD;
    int special = ZERO;
    int i = 0;

    printf("Hello! Please enter Your C Program Text below:\n");
    while ((symbol = getchar()) != EOF)
    {
        if (symbol == '\n')
        {
            status = isBalanced(str);
            print_status(str, status);
            if (special == -1 || (status == 0 && special_line(str, special) == 0))
            {
                result = ZERO;
            }
            special = special_line(str, special);
            memset(str, '\0', MAX);
            i = -1;
        }
        if(i == -1)
            str[i+1] = symbol;
        else
            str[i] = symbol;
        i++;
    }
    status = isBalanced(str);
    print_status(str, status);
    if (special == -1 || (status == 0 && special_line(str, special) == 0))
    {
        result = ZERO;
    }
    special = special_line(str, special);
    memset(str, '\0', MAX);
    i = -1;

    if (result == VAILD)
    {
        printf("In Conclusion:\tThe Text is Balanced");
    }
    else
    {
        printf("In Conclusion:\tThe Text is NOT Balanced");
    }
    return 0;
}

int isBalanced(char str[])
{
    char stack[MAX];
    int i = 0;
    int j = 0;
    size_t len = strlen(str);
    char parenthesis_one = '(';
    char parenthesis_two = '[';
    char parenthesis_three = '{';
    char parenthesis_one_c = ')';
    char parenthesis_two_c = ']';
    char parenthesis_three_c = '}';

    for (i = ZERO; i <= len; i++)
    {
        if (str[i] == '/' && str[i + VAILD] == '*')
        {
            for (i; i <= len; i++)
            {
                if (str[i] == '*' && str[i + VAILD] == '/')
                    break;
            }
        }
        if (str[i] == parenthesis_one || str[i] == parenthesis_two || str[i] == parenthesis_three)
        {
            push(stack, str[i], j);
            j++;
        }
        if (str[i] == parenthesis_one_c)
        {
            if (pop(stack, str[i], j) == parenthesis_one)
            {
                j--;
            }
            else
            {
                return 0;
            }
        }
        if (str[i] == parenthesis_two_c)
        {
            if (pop(stack, str[i], j) == parenthesis_two)
            {
                j--;
            }
            else
            {
                return 0;
            }
        }
        if (str[i] == parenthesis_three_c)
        {
            if (pop(stack, str[i], j) == parenthesis_three)
            {
                j--;
            }
            else
            {
                return 0;
            }
        }
    }
    return (j == 0) ? 1 : 0;
}

int special_line(char str[], int special)
{
    size_t s_len = strlen(str);
    int i = 0;
    while (i <= s_len)
    {
        if (isspace(str[i]) == 0 && str[i] != '{' && str[i] != '}')
        {
            return ZERO;
        }
        while (isspace(str[i]) != 0)
        {
            i++;
        }
        if (str[i] == '{')
        {
            if (special == VAILD)
            {
                return -1;
            }
            else
            {
                return VAILD;
            }
        }
        else
        {
            if (str[i] == '}')
            {
                if (special == CLOSE)
                {
                    return -1;
                }
                else
                {
                    return CLOSE;
                }
            }
            else
            {
                return ZERO;
            }
        }
        i++;
    }
    return ZERO;
}

void print_status(char line_str[], int status)
{
    printf("%s", line_str);
    if (status == VAILD)
    {
        printf(" -\tLine is balanced\n");
    }
    else
    {
        printf(" -\tLine is NOT balanced\n");
    }
}

char pop(char stack[], char str, int i)
{
    int temp;
    if (i > 0)
    {
        temp = stack[i - VAILD];
        stack[i - VAILD] = '\0';
        return temp;
    }
    else
    {
        return 'E';
    }
}

void push(char stack[], char str, int i)
{
    stack[i] = str;
}
