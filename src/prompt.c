#include "headers.h"

void prompt()
{
    printBlue(userName);
    printBlue("@");
    printBlue(hostName);
    printBlue(":");
    // print path. if same as calling dir, use "~". else absolute path
    if (strcmp(currDir,homeDir)==0)
    {
        printBlue("~");
    }
    else
    {
        int lenHome = strlen(homeDir);
        int lenCurr = strlen(currDir);
        if (lenCurr < lenHome)
        {
            printBlue(currDir);
        }
        else
        {
            bool beyondHome = true;
            for (int i = 0; i < lenHome; i++)
            {
                if (currDir[i] != homeDir[i])
                {
                    beyondHome = false;
                    break;
                }
            }
            if (!beyondHome)
            {
                printBlue(currDir);
            }
            else
            {
                char *afterHome;
                afterHome = (char *)malloc(1000 * sizeof(char));
                for (int i = lenHome; i <= lenCurr; i++)
                {
                    afterHome[i - lenHome] = currDir[i];
                }
                printBlue("~");
                printBlue(afterHome);
            }
        }
    }
    printf("\n$ ");
}