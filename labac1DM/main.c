#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include <string.h>
#include "bin.h"

/*
Как делаем:
1. Парсим флаг с указанием файла с stdin
2. Открываем файл и читаем данны в массив char (инфиксная запись)
    1. Отдельно считаем количество переменных в выражении (n)
3. Конвертируем инфиксную запись в постфиксную
4. Создаем цикл с 2^n итерациями (для построения таблицы истинности). Переводим i-ую итерацию в двоичное число, каждый элемент которого будет в стеке
5. В i-ой итерации передаем stack с элементами двочиного числа и выражение в функцию convertToPostfix и выводим результат
*/

#define MAX_SIZE 1024

/// @brief Возвращает приоритет логической операции
/// @param symbol
/// @return
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

int isOperator(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return 0;
    }
    return 1;
}

int calculate(int op1, int op2, char operator)
{
    switch (operator)
    {
    case '!':
        return !op1;
    case '&':
        return op1 && op2;
    case '|':
        return op1 || op2;
    case '^':
        return (!op1 || op2) && (op1 && !op2);
    default:
        return -1;
    }
}

void calculateExperssion(char postfix[], StackNodePtr table)
{

    int n = strlen(postfix);
    StackNodePtr stack = NULL;
    char answer[MAX_SIZE];
    int answerIndex = 0;

    for (int i = 0; i < n; i++)
    {
        char symbol = postfix[i];

        if (!isOperator(symbol))
        {
            int digit = pop(&table);
            answerIndex += sprintf(answer + answerIndex, "%d ", digit);
            printf("symbol - %c; digit - %d \n", symbol, digit);
            push(&stack, digit);
            continue;
        }
        else if (symbol == '!' && lenStack(stack) >= 1)
        {
            int value = pop(&stack);
            int calculation = calculate(value, 0, '!');
            push(&stack, calculation);
        }
        // else if (isOperator(symbol) && lenStack(stack) < 2)
        // {
        //     printf("STACK LT 2");
        //     break;
        // }
        else
        {
            int right = pop(&stack);
            int left = pop(&stack);
            int calculation = calculate(left, right, symbol);
            push(&stack, calculation);
        }
    }

    // if (lenStack(stack) != 1)
    // {
    //     printf("len stack not equal 1");
    // }
    printf("%s %d", answer, pop(&stack));
}

/// @brief Конвертирет инфиксное выражение из infix в посфиксное и записывает его в postfix
/// @param infix
/// @param postfix
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

/// @brief Сборка всей логики
/// @param argc
/// @param argv
/// @return
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
    char infix[MAX_SIZE];
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
    char postfix[len];

    // for (int i = 0; i < len; i++)
    // {
    //     printf("%c", infix[i]);
    // }

    convertToPostfix(infix, postfix);
    // for (int i = 0; i < len; i++)
    // {
    //     printf("%c ", postfix[i]);
    // }

    // printList(variables);

    // printf("%d\n", len(variables));

    int countOfOperation = 1 << countOfVariables; // 16
    // printf("infix - %s\n", infix);
    printf("infix - %s\n", postfix);
    printf("a b c d Answer\n");

    for (int i = 0; i < countOfOperation; i++)
    {
        StackNodePtr stackBinNumber = convertToBin(i, countOfVariables);
        // printStack(stackBinNumber);
        calculateExperssion(postfix, stackBinNumber);
        printf("\n");
        // break;
        // printf("bin %d\n", binNumber);
    }

    printf("\n");
    return 0;
}
