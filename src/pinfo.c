#include "headers.h"

void pinfoCommand(int type, char *pidStr)
{
    pid_t pid;
    if (type == 1)
    {
        pid = getpid();
    }
    else
    {
        pid = atoi(pidStr);
    }
    char filePath[1000];
    // for getting the info about the process with pid
    sprintf(filePath, "/proc/%d/stat", pid);

    if (open(filePath, O_RDONLY) < 0)
    {
        printRed("There is no process with given process ID.");
    }
    else
    {
        FILE *f;
        char status;
        long long mem;
        f = fopen(filePath, "r");
        fscanf(f, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lld %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status, &mem);
        fclose(f);
        printf("pid -- %d\n", pid);
        printf("Process Status -- %c", status);
        if (getpgid(pid) == foregroundID)
        {
            printf("+");
        }
        printf("\n");
        printf("memory -- %lld\n", mem);

        char execPath[1000];
        sprintf(filePath, "/proc/%d/exe", pid); // For executable path
        int len = readlink(filePath, execPath, sizeof(execPath));
        execPath[len] = 0;
        // printf("%s", execPath);

        printf("Executable Path -- ");
        int lenCurr = strlen(currDir);
        int lenExec = strlen(execPath);
        if (lenExec < lenCurr)
        {
            printf("%s", execPath);
        }
        else
        {
            bool beyondCurr = true;
            for (int i = 0; i < lenCurr; i++)
            {
                if (currDir[i] != execPath[i])
                {
                    beyondCurr = false;
                    break;
                }
            }
            if (!beyondCurr)
            {
                printf("%s", execPath);
            }
            else
            {
                char *afterHome;
                afterHome = (char *)malloc(1000 * sizeof(char));
                int i;
                // printf("%d%d",lenCurr,lenExec);
                for (i = lenCurr; i <= lenExec; i++)
                {
                    afterHome[i - lenCurr] = execPath[i];
                }
                afterHome[i] = 0;
                printf("~");
                printf("%s", afterHome);
                free(afterHome);
            }
        }
    }
}