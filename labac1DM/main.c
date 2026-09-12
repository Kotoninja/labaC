#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "list.h"

int main(int argc, char *argv[])
{
    char *filename = NULL;
    char c;

    ListNode *variables = NULL;

    if (strncmp(argv[1], "-file", 5) == 0)
    {
        filename = strchr(argv[1], '=') + 1;
    }

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error occured while opening %s", filename);
        return 1;
    }
    else
    {
        while ((c = getc(fp)) != EOF)
        {
            if ((strchr("+-^*", c)) == NULL)
            {
                variables = append(variables, c);
            }
        }
    }

    fclose(fp);

    printList(variables);

    printf("\n");
    return 0;
}
