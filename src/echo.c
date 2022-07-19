#include "headers.h"

void echoCommand(char **words, int len)
{
    for (int i = 1; i < len; i++)
    {
        printf("%s ", words[i]);
    }
}