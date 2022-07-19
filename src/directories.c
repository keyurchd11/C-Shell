#include "headers.h"

bool checkDirectory(char *newPath)
{
    if (chdir(newPath) != 0)
    {
        return false;
    }
    else
    {
        chdir(currDir);
    }
    return true;
}

