#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include <string.h>
#include "bin.h"
#include "hashmap.h"
#include <stddef.h>
#include <assert.h>
#include <ctype.h>

/* Операции
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
/// @return symbol
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
/// @return 0 or 1
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
/// @return 0 or 1, if unknow method: -1
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

/// @brief Создает словарь с переменными и их значениями (0,1)
/// @param stackBinNumber
/// @param variables
/// @return variables = "abc" : {"a" : 0, "b" : 0, "c" : 0}
hash_map_t *generateCase(StackNodePtr stackBinNumber, char *variables)
{
    int countOfVariables = strlen(variables);
    hash_map_t *hashmapVariables = hash_map_create(countOfVariables);
    for (int i = 0; i < countOfVariables; i++)
    {
        hashmapVariables = hash_map_insert(hashmapVariables, (char[]){variables[i], '\0'}, pop(&stackBinNumber));
    }

    return hashmapVariables;
}

/// @brief Высчитывает логическое выражение
/// @param postfix
/// @param hashMap
/// @return 0 or 1
int calculateExperssion(char postfix[], hash_map_t *hashMap)
{
    assert(hashMap != NULL);

    int n = strlen(postfix);
    StackNodePtr stack = NULL;

    for (int i = 0; i < n; i++)
    {
        char symbol = postfix[i];

        if (!isOperator(symbol))
        {
            int digit = hash_map_get(hashMap, (char[]){symbol, '\0'});
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

/// @brief Создает СДНФ
/// @param variables
/// @param postfix
void generateSDNF(char *variables, char *results)
{
    int n = strlen(variables);
    printf("СДНФ: ");
    int countOfVariables = n;
    int countOfOperation = 1 << countOfVariables;

    char answer[MAX_SIZE];
    int answerIndex = 0;

    for (int i = 0; i < countOfOperation; i++)
    {
        StackNodePtr stackBinNumber = convertToBin(i, countOfVariables);
        hash_map_t *hashmapVariables = generateCase(stackBinNumber, variables);

        int result = results[i];

        if (result != '1')
            continue;

        char buffer[MAX_SIZE];
        int bufferIndex = 0;

        for (int i = 0; i < n; i++)
        {
            char variabel = variables[i];
            if (hash_map_get(hashmapVariables, (char[]){variabel, '\0'}) == 1)
            {
                buffer[bufferIndex++] = variabel;
            }
            else
            {
                buffer[bufferIndex++] = '!';
                buffer[bufferIndex++] = variabel;
            }
            buffer[bufferIndex++] = '&';
        }
        buffer[bufferIndex - 1] = '\0';

        answer[answerIndex++] = '(';
        for (int i = 0; i < strlen(buffer); i++)
        {
            answer[answerIndex++] = buffer[i];
        }
        answer[answerIndex++] = ')';

        answer[answerIndex++] = '|';
    }

    if (answerIndex == 0)
    {
        printf("Не существует\n");
        return;
    }

    answer[answerIndex - 1] = '\0';
    printf("%s\n", answer);
}

/// @brief Создает СКНФ
/// @param variables
/// @param postfix
void generateSKNF(char *variables, char *results)
{
    int n = strlen(variables);
    printf("СКНФ: ");
    int countOfVariables = n;
    int countOfOperation = 1 << countOfVariables;

    char answer[MAX_SIZE];
    int answerIndex = 0;

    for (int i = 0; i < countOfOperation; i++)
    {
        hash_map_t *hashmapVariables = generateCase(convertToBin(i, n), variables);
        int result = results[i];

        if (result != '0')
            continue;

        char buffer[MAX_SIZE];
        int bufferIndex = 0;

        for (int i = 0; i < n; i++)
        {
            char variabel = variables[i];
            if (hash_map_get(hashmapVariables, (char[]){variabel, '\0'}) == 0)
            {
                buffer[bufferIndex++] = variabel;
            }
            else
            {
                buffer[bufferIndex++] = '!';
                buffer[bufferIndex++] = variabel;
            }
            buffer[bufferIndex++] = '|';
        }
        buffer[bufferIndex - 1] = '\0';

        answer[answerIndex++] = '(';
        for (int i = 0; i < strlen(buffer); i++)
        {
            answer[answerIndex++] = buffer[i];
        }
        answer[answerIndex++] = ')';

        answer[answerIndex++] = '&';
    }

    if (answerIndex == 0)
    {
        printf("Не существует\n");
        return;
    }

    answer[answerIndex - 1] = '\0';
    printf("%s\n", answer);
}

/// @brief Строит таблицу истинности
/// @param variables
/// @param postfix
void generateTable(char *variables, char *results)
{
    int n = strlen(variables);
    for (int i = 0; i <= n - 1 + 7 - 18; i++)
    {
        printf(" ");
    }
    printf("Таблица истинности\n");
    int countOfOperation = 1 << n; // Считаем количество операци в таблице истинности
    // Формируем таблицу
    for (int i = 0; i < n; i++)
    {
        printf("%c ", variables[i]);
    }
    printf("Answer\n");

    for (int i = 0; i < countOfOperation; i++)
    {
        StackNodePtr stackBinNumber = convertToBin(i, n); // Создаем ряд чисел для переменных (типо 1 0 0 0 или 0 0 1 1)
        printStack(stackBinNumber);
        // Результат выражения
        printf("   %c\n", results[i]);
    }
}

/// @brief Генерирует все ответы для кадого случая бинарного выражения
/// @param variables abcd
/// @param postfix abc||
/// @return 10101010
char *generateResults(char *variables, char *postfix)
{
    int n = strlen(variables);

    static char answer[MAX_SIZE];
    int answerIndex = 0;

    for (int i = 0; i < 1 << n; i++)
    {
        hash_map_t *hashmapVariables = generateCase(convertToBin(i, n), variables);
        int result = calculateExperssion(postfix, hashmapVariables);
        answer[answerIndex++] = result ? '1' : '0';
    }

    answer[answerIndex] = '\0';
    return answer;
}

/// @brief Выводит, какие переменные являются фиктивными
/// @param variables
/// @param results
/// @param postfix
void generateFictitious(char *variables, char *results, char *postfix)
{

    int n = strlen(variables);

    char answer[MAX_SIZE];
    int answerIndex = 0;

    for (int i = 0; i < n; i++)
    {
        char variable = variables[i];
        char key[2] = {variable, '\0'};

        int isFictitious = 1;
        for (int j = 0; j < 1 << n; j++)
        {
            char firstResult = results[j];

            hash_map_t *hashmapNumbers = generateCase(convertToBin(j, n), variables);

            int switchNumber = hash_map_get(hashmapNumbers, key);

            hashmapNumbers = hash_map_insert(hashmapNumbers, key, !switchNumber); // Меняем значение переменной на обратное

            int secondResult = calculateExperssion(postfix, hashmapNumbers);

            if (firstResult != ('0' + secondResult))
            {
                isFictitious = 0;
                break;
            };
        }
        if (isFictitious)
        {
            answer[answerIndex++] = variable;
        }
    }
    answer[answerIndex] = '\0';

    if (answerIndex == 0)
    {
        printf("Фиктивных переменных нет");
    }
    else
    {
        printf("Фиктивные переменные - %s", answer);
    }
    printf("\n");
}

/// @brief Сборка всей логики
/// @param argc
/// @param argv
/// @return Возращает все ответы
int main(int argc, char *argv[])
{
    // Парсинг флага
    char *filename = NULL;

    filename = "data.txt";
    // if (strncmp(argv[1], "-file", 5) == 0)
    // {
    //     filename = strchr(argv[1], '=') + 1;
    // }
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
    int variablesIndex = 0;

    hash_map_t *frequency = hash_map_create(1); // Hashmap для подсчета частоты символов (чтобы не было повторок переменных)

    while ((symbol = tolower(getc(fp))) != EOF && len < MAX_SIZE - 1)
    {
        if (symbol != ' ')
        {
            infix[len++] = (char)symbol;
            // Cчитаем количество переменных
            if (!isOperator(symbol))
            {
                if (!hash_map_key(frequency, (char[]){symbol, '\0'})) // Если есть повторка - скипаем
                {
                    variables[variablesIndex++] = symbol;
                    frequency = hash_map_insert(frequency, (char[]){symbol, '\0'}, 0);
                }
            }
        }
    }

    infix[len] = '\0';
    variables[variablesIndex] = '\0';
    hash_map_free(frequency);
    fclose(fp);

    // Создаем массив для постфиксного выражения
    char postfix[len];

    convertToPostfix(infix, postfix);

    char *results = generateResults(variables, postfix); // Вычесляет все результаты булевого выражения

    generateTable(variables, results);               // Строим таблицу истинности
    generateSDNF(variables, results);                // Строим СДНФ
    generateSKNF(variables, results);                // Строим СКНФ
    generateFictitious(variables, results, postfix); // Узнаем, какие переменные фиктивны
    printf("\n");
    return 0;
}
