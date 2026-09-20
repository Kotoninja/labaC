#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include <string.h>
#include "bin.h"
#include "hashmap.h"
#include <stddef.h>
#include <assert.h>
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

int calculateExperssion(char postfix[], hash_map_t *hashMap)
{
    assert(hashMap != NULL);

    int n = strlen(postfix);
    StackNodePtr stack = NULL;

    // char answer[MAX_SIZE];
    // int answerIndex = 0;

    for (int i = 0; i < n; i++)
    {
        char symbol = postfix[i];

        if (!isOperator(symbol))
        {
            int digit = hash_map_get(hashMap, (char[]){symbol, '\0'});
            // printf("symbol - %c; digit - %d \n", symbol, digit);
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

    assert(lenStack(stack) != 1);
    return pop(&stack);
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
    char variables[MAX_SIZE];
    int countOfVariables;
    hash_map_t *frequency = hash_map_create(1);

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
                if (!hash_map_key(frequency, (char[]){symbol, '\0'}))
                {
                    variables[countOfVariables++] = symbol;
                    frequency = hash_map_insert(frequency, (char[]){symbol, '\0'}, 0);
                }
            }
            }
        }
    }
    infix[len] = '\0';
    variables[countOfVariables] = '\0';
    hash_map_free(frequency);
    fclose(fp);

    // Создаем массив для постфиксного выражения
    char postfix[len];

    convertToPostfix(infix, postfix);

    int countOfOperation = 1 << countOfVariables;

    for (int i = 0; i < countOfVariables; i++)
    {
        printf("%c ", variables[i]);
    }
    printf("Answer\n");

    for (int i = 0; i < countOfOperation; i++)
    {
        StackNodePtr stackBinNumber = convertToBin(i, countOfVariables);
        hash_map_t *hashmapVariables = hash_map_create(countOfVariables);
        printStack(stackBinNumber);
        for (int i = 0; i < countOfVariables; i++)
        {
            hashmapVariables = hash_map_insert(hashmapVariables, (char[]){variables[i], '\0'}, pop(&stackBinNumber));
        }
        printf("   %d\n", calculateExperssion(postfix, hashmapVariables));
    }

    printf("\n");
    return 0;
}
