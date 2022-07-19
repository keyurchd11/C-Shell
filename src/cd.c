#include "headers.h"

void cdCommand(char *newPath)
{
    if (strcmp(newPath, "-") == 0)
    {
        strcpy(newPath, prevDir);
    }
    else if (strcmp(newPath, "~") == 0)
    {
        strcpy(newPath, homeDir);
    }
    if (checkDirectory(newPath))
    {
        chdir(newPath);
        strcpy(prevDir, currDir);
        getcwd(currDir, 1000);
    }
    else
    {
        printRed("The Directory doesn't exist.");
    }
}

void pwdCommand()
{
    printf("%s", currDir);
}
