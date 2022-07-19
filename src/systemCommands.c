#include "headers.h"

void parse(char *line, char **argv)
{
    while (*line != '\0')
    {
        while (*line == ' ' || *line == '\t' || *line == '\n')
            *line++ = '\0'; /* replace white spaces with 0    */
        *argv++ = line;     /* save the argument position     */
        while (*line != '\0' && *line != ' ' &&
               *line != '\t' && *line != '\n')
            line++; /* skip the argument until ...    */
    }
    *argv = '\0'; /* mark the end of argument list  */
}

void execute(char **argv)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    { /* fork a child process           */
        printRed("ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    { /* for the child process:         */
   
        if (execvp(*argv, argv) < 0)
        { /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else
    {
        currentFG = pid;
        // wait until child completed
        while (waitpid(pid, &status, WUNTRACED) != pid)
        {
            ;
        }
        currentFG = -2;
    }
}

int sortBGprocess(const void *a, const void *b)
{
    return (strcmp(((struct bgProcesses_ *)a)->pName, ((struct bgProcesses_ *)b)->pName));
}

void executeBG(char **argv)
{
    pid_t pid;
    int status;

    if ((pid = fork()) < 0)
    { /* fork a child process           */
        printRed("ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0)
    { /* for the child process:         */
        if (execvp(*argv, argv) < 0)
        { /* execute the command  */
            printf("ERROR: exec failed\n");
            exit(1);
        }
    }
    else
    {
        pid_t backgroundID = 0;
        printf("%d", pid);
        setpgid(pid, backgroundID);
        bgProcesses[bgProcessCount].pName = (char *)malloc(100 * sizeof(char));
        strcpy(bgProcesses[bgProcessCount].pName, argv[0]);
        bgProcesses[bgProcessCount].pID = pid;
        bgProcesses[bgProcessCount].jobNo = bgProcessCount;
        bgProcessCount++;
        qsort(bgProcesses, bgProcessCount, sizeof(struct bgProcesses_), sortBGprocess);
    }
}
