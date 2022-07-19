#include "headers.h"

void printBlue(char *toPrint)
{
    printf("\e[34m");
    printf("%s", toPrint);
    printf("\e[97m");
}
void printCyan(char *toPrint)
{
    printf("\e[36m");
    printf("%s", toPrint);
    printf("\e[97m");
}
void resetBG()
{
    system("clear");
    printf("\e[0m");
    printf("\e[1;1H\e[2J");
}
void startShell()
{
    printf("\e[48;2;0;0;0m \n");
    system("clear");

    printCyan("Welcome to k-Shell !\n");
}
void printRed(char *toPrint)
{
    printf("\e[31m");
    printf("%s", toPrint);
    printf("\e[97m");
}
void printPaddedNumber(int num, int maxNumLen)
{
    int numLen = 1;
    if (num > 0)
        numLen = log10(num) + 1;
    for (int it = 0; it < maxNumLen - numLen; it++)
    {
        printf(" ");
    }
    printf("%d", num);
}