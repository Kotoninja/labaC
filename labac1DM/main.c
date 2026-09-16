#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include <string.h>
#include "bin.h"

#define MAX_SIZE 1024

int Priority(char symbol)
{
    switch (symbol)
    {
    case '!':
        return 4;
    case '&':
        return 3;
    case '|':
        return 2;
    case '^':
        return 1;
    case '(':
        return 0;
    default:
        return -1;
    }
}

void convertToPostfix(char infix[], char postfix[])
{
    int lenInfix = strlen(infix);
    StackNodePtr stack = NULL;

    int indexPostfix = 0;

    for (int i = 0; i < lenInfix; i++)
    {
        char symbol = infix[i];
        switch (symbol)
        {
        case '!':
        case '^':
        case '&':
        case '|':
        {
            while (!isEmpty(stack))
            {
                char topValue = stackTop(stack);

                if (symbol == '!')
                {
                    if (Priority(symbol) < Priority(topValue))
                    {
                        postfix[indexPostfix++] = pop(&stack);
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    if (Priority(symbol) <= Priority(topValue))
                    {
                        postfix[indexPostfix++] = pop(&stack);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            push(&stack, symbol);
            break;
        }
        case ')':
            while (!isEmpty(stack))
            {
                char topValue = stackTop(stack);
                if (topValue == '(')
                {
                    pop(&stack);
                    break;
                }
                postfix[indexPostfix++] = pop(&stack);
            }
            break;
        case '(':
            push(&stack, symbol);
            break;
        default:
            postfix[indexPostfix++] = symbol;
        }
    }

    while (!isEmpty(stack))
    {
        char topValue = stackTop(stack);
        postfix[indexPostfix++] = pop(&stack);
    }
}

int main(int argc, char *argv[])
{
    // Парсинг флага
    char *filename = NULL;

    if (strncmp(argv[1], "-file", 5) == 0)
    {
        filename = strchr(argv[1], '=') + 1;
    }

    // Открытие файла для чтения
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error occured while opening %s", filename);
        return 1;
    }

    // Создаем массив куда записываются данные из файла (в массиве только операции и операнды)
    char infix[MAX_SIZE]; //    a | ( b & c ^ d )
    int len = 0;
    int symbol;
    int countOfVariables;

    while ((symbol = getc(fp)) != EOF && len < MAX_SIZE - 1)
    {
        if (symbol != ' ')
        {
            infix[len++] = (char)symbol;
            // Cчитаем количество переменных
            switch (symbol)
            {
            case '!':
            case '&':
            case '(':
            case ')':
            case '^':
            case '|':
            {
                continue;
            }
            default:
            {
                countOfVariables++;
            }
            }
        }
    }
    infix[len] = '\0';

    fclose(fp);

    // Создаем массив для постфиксного выражения
    char postfix[len];//    a b c & d ^ |

    // for (int i = 0; i < len; i++)
    // {
    //     printf("%c", infix[i]);
    // }

    convertToPostfix(infix, postfix);
    
    for (int i = 0; i < len; i++)
    {
        printf("%c ", postfix[i]);
    }

    // printList(variables);

    // printf("%d\n", len(variables));

    // int countOfOperation = pow(2, len(variables));
    int countOfOperation = 1 << countOfVariables; // 16
    printf("countOfOperation - %d\n", countOfOperation);
    for (int i = 0; i < countOfOperation; i++)
    {

        int binNumber = convertToBin(i);
        printf("bin %d\n", binNumber);
        // for (int bit = countOfVariables - 1; bit >= 0; bit--)
        //     putchar('0' + ((i >> bit) & 1));
        // putchar('\n');
    }

    printf("\n");
    return 0;
}
