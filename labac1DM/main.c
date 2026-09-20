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
5. В i-ой итерации передаем hashmap(с переменными и элементами двочиного числа) и выражение в функцию convertToPostfix и выводим результат

! - инверсия
| - дизъюнкция
& - конъюнкция
^ - сложение по модулю 2
@ - эквивалентность
* - импликация
~ - коимпликация
> - стрелка Пирса
< - штрих Шеффера
*/

#define MAX_SIZE 1024

/// @brief Возвращает приоритет логической операции
/// @param symbol
/// @return
int Priority(char symbol)
{
    switch (symbol)
    {
    case '!': // инверсия
        return 7;
    case '&': // конъюнкция
        return 6;
    case '|': // дизъюнкция
    case '^': // сложение по модулю 2
        return 5;
    case '<': // штрих Шеффера
    case '>': // стрелка Пирса
        return 4;
    case '*': // импликация
    case '~': // коимпликация
        return 2;
    case '@': // эквивалентность
        return 1;
    default:
        return 0;
    }
}
/// @brief Возвращает является ли c оператором
/// @param c
/// @return
int isOperator(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return 0;
    }
    return 1;
}

/// @brief Считает логическое выражение между двумя(или одним) операндоми
/// @param op1
/// @param op2
/// @param operator
/// @return
int calculate(int op1, int op2, char operator)
{
    switch (operator)
    {
    case '!': // инверсия
        return !op1;
    case '&': // конъюнкция
        return op1 && op2;
    case '|': // дизъюнкция
        return op1 || op2;
    case '^': // сложение по модулю 2
        return (!op1 && op2) || (op1 && !op2);
    case '@': // эквивалентность
        return (op1 && op2) || (!op1 && !op2);
    case '*': // импликация
        return (!op1 || op2);
    case '~': // коимпликация
        return (op1 && !op2);
    case '>': // стрелка Пирса
        return !(op1 || op2);
    case '<': // штрих Шеффера
        return !(op1 && op2);
    default:
        return -1;
    }
}
/// @brief Высчитывает логическое выражение
/// @param postfix
/// @param hashMap
/// @return
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
        else
        {
            int right = pop(&stack);
            int left = pop(&stack);
            int calculation = calculate(left, right, symbol);
            push(&stack, calculation);
        }
    }

    assert(lenStack(stack) == 1);
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

        if (symbol == ')')
        {
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
        }
        else if (symbol == '(')
        {
            push(&stack, symbol);
        }
        else if (!isOperator(symbol))
        {
            postfix[indexPostfix++] = symbol;
        }
        else
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
        }
    }

    while (!isEmpty(stack))
    {
        char topValue = stackTop(stack);
        postfix[indexPostfix++] = pop(&stack);
    }
    postfix[indexPostfix] = '\0';
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
    char infix[MAX_SIZE]; // Строка для инфиксного(исходного) выражения
    int len = 0;
    int symbol; // Вспомогательная переменная для чтения файла

    char variables[MAX_SIZE]; // Строка, где хранятся все переменные
    int countOfVariables;

    hash_map_t *frequency = hash_map_create(1); // Hashmap для подсчета частоты символов (чтобы не было повторок переменных)

    while ((symbol = getc(fp)) != EOF && len < MAX_SIZE - 1)
    {
        if (symbol != ' ')
        {
            infix[len++] = (char)symbol;
            // Cчитаем количество переменных
            if (!isOperator(symbol))
            {
                if (!hash_map_key(frequency, (char[]){symbol, '\0'})) // Если есть повторка - скипаем
                {
                    variables[countOfVariables++] = symbol;
                    frequency = hash_map_insert(frequency, (char[]){symbol, '\0'}, 0);
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
    int countOfOperation = 1 << countOfVariables; // Считаем количество операци в таблице истинности

    // Формируем таблицу
    for (int i = 0; i < countOfVariables; i++)
    {
        printf("%c ", variables[i]);
    }
    printf("Answer\n");

    for (int i = 0; i < countOfOperation; i++)
    {
        StackNodePtr stackBinNumber = convertToBin(i, countOfVariables); // Создаем ряд чисел для переменных (типо 1 0 0 0 или 0 0 1 1)
        // Присвиваем каждой переменной значение
        hash_map_t *hashmapVariables = hash_map_create(countOfVariables);
        printStack(stackBinNumber);
        for (int i = 0; i < countOfVariables; i++)
        {
            hashmapVariables = hash_map_insert(hashmapVariables, (char[]){variables[i], '\0'}, pop(&stackBinNumber));
        }
        // Результат выражения
        printf("   %d\n", calculateExperssion(postfix, hashmapVariables));
    }

    printf("\n");
    return 0;
}
