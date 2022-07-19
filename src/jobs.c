#include "headers.h"
void printJobs()
{
    for (int i = 0; i < bgProcessCount; i++)
    {
        printJob(i, bgProcesses[i].pID, 'A');
    }
}
void printJobsWithFlag(char **commands, int totParams)
{
    bool run = false, stopped = false;
    for (int i = 0; i < totParams; i++)
    {
        if (commands[i][0] == '-')
        {
            for (int j = 0; j < strlen(commands[i]); j++)
            {
                if (commands[i][j] == 'r')
                    run = true;
                if (commands[i][j] == 's')
                    stopped = true;
            }
        }
    }
    if (run && stopped)
        printJobs();
    else if (run)
    {
        for (int i = 0; i < bgProcessCount; i++)
        {
            printJob(i, bgProcesses[i].pID, 'R');
        }
    }
    else if (stopped)
    {
        for (int i = 0; i < bgProcessCount; i++)
        {
            printJob(i, bgProcesses[i].pID, 'T');
        }
    }
}

void printJob(int num, pid_t pid, char flag)
{
    char filePath[1000];
    sprintf(filePath, "/proc/%d/stat", pid);
    flag = toupper(flag);
    FILE *f;
    char status;
    long long mem;
    f = fopen(filePath, "r");
    fscanf(f, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lld %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d", &status, &mem);
    fclose(f);
    if (flag == 'A')
    {
        if (status == 'R' || status == 'S')
        {
            printf("[%d] Running %s [%d]\n", bgProcesses[num].jobNo + 1, bgProcesses[num].pName, bgProcesses[num].pID);
        }
        else
        {
            printf("[%d] Stopped %s [%d]\n", bgProcesses[num].jobNo + 1, bgProcesses[num].pName, bgProcesses[num].pID);
        }
    }
    if (flag == 'R')
    {
        if (status == 'R' || status == 'S')
        {
            printf("[%d] Running %s [%d]\n", bgProcesses[num].jobNo + 1, bgProcesses[num].pName, bgProcesses[num].pID);
        }
    }
    if (flag == 'T')
    {
        if (status == 'T')
        {
            printf("[%d] Stopped %s [%d]\n", bgProcesses[num].jobNo + 1, bgProcesses[num].pName, bgProcesses[num].pID);
        }
    }
}

void changeToForeground(char *numStr)
{
    int num = atoi(numStr);
    if (num < 1 || num > bgProcessCount)
    {
        printRed("No job with the given Job Number exists");
        return;
    }
    int STATUS;
    pid_t pid = -1;
    num--;
    char *nameOfTask;
    for (int i = 0; i < bgProcessCount; i++)
    {
        if (bgProcesses[i].jobNo == num)
        {
            int ts;
            // scanf("%d", &ts);
            pid = bgProcesses[i].pID;
            nameOfTask = (char *)malloc((strlen(bgProcesses[i].pName) * sizeof(char)));
            strcpy(nameOfTask, bgProcesses[i].pName);
            for (int j = i; j < bgProcessCount - 1; j++)
            {
                bgProcesses[j] = bgProcesses[j + 1];
            }
            bgProcessCount--;
        }
        if (bgProcesses[i].jobNo > num)
        {
            bgProcesses[i].jobNo--;
        }
    }
    printf("%s", nameOfTask);
    signal(SIGTTOU, SIG_IGN); //ignore sigttou

    tcsetpgrp(STDIN_FILENO, getpgid(pid)); // change foregroud group to make that process a part of the foreground process

    currentFG = pid;
    kill(pid, SIGCONT);
    waitpid(pid, &STATUS, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, foregroundID);

    signal(SIGTTOU, SIG_DFL);
    if (WIFSTOPPED(STATUS))
    {
        bgProcesses[bgProcessCount].pName = (char *)malloc(100 * sizeof(char));
        strcpy(bgProcesses[bgProcessCount].pName, nameOfTask);
        bgProcesses[bgProcessCount].pID = currentFG;
        bgProcesses[bgProcessCount].jobNo = bgProcessCount;
        bgProcessCount++;
        qsort(bgProcesses, bgProcessCount, sizeof(struct bgProcesses_), sortBGprocess);
    }
    currentFG = -2;
}

void bg(char *jobNoStr)
{
    int jobNo = atoi(jobNoStr);
    if (jobNo < 1 || jobNo > bgProcessCount)
    {
        printRed("No job with the given Job Number exists");
        return;
    }
    jobNo--;
    for (int i = 0; i < bgProcessCount; i++)
    {
        if (bgProcesses[i].jobNo == jobNo)
        {
            kill(bgProcesses[i].pID, SIGTTIN);
            kill(bgProcesses[i].pID, SIGCONT);
        }
        if (bgProcesses[i].jobNo > jobNo)
        {
            bgProcesses[i].jobNo--;
        }
    }
}