#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "list.h"

// int main(int argc, char *argv[])
// {
//     char *filename = NULL;
//     char c;

//     ListNode *variables = NULL;

//     if (strncmp(argv[1], "-file", 5) == 0)
//     {
//         filename = strchr(argv[1], '=') + 1;
//     }

//     FILE *fp = fopen(filename, "r");

//     if (fp == NULL)
//     {
//         printf("Error occured while opening %s", filename);
//         return 1;
//     }
//     else
//     {
//         while ((c = getc(fp)) != EOF)
//         {
//             if ((strchr("+-^* ", c)) == NULL)
//             {
//                 char buf[2] = {c, '\0'};
//                 variables = appendStr(variables, buf);
//             }
//         }
//     }

//     fclose(fp);

//     printList(variables);

//     printf("%d\n", len(variables));


//     // int countOfOperation = pow(2, len(variables));
//     int countOfOperation = 1 << len(variables);
//     // for (int i = 0; i < 1; i++)
//     // {
//     //     const char binNumber = convertToBin(i);
//     //     printf("%08d\n", binNumber[0]);
//     // }

//     printf("\n");
//     return 0;
// }
