#include "headers.h"
// #include "formatting.c"
// #include "prompt.c"

void setUp()
{
    startShell();
    int hostCode = gethostname(hostName, 1000);
    int userCode = getlogin_r(userName, 1000);
    getcwd(homeDir, 1000);
    getcwd(currDir, 1000);
    strcpy(prevDir, currDir);
    foregroundID = getpgid(getpid());
    loadHistory();
    currentFG = -2;
}
void signalHandler()
{
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    int num = bgProcessCount;
    for (int i = 0; i < bgProcessCount; i++)
    {
        if (bgProcesses[i].pID == pid)
        {
            fprintf(stderr, "%s with pid %d exited ", bgProcesses[i].pName, bgProcesses[i].pID);
            for (int j = i; j < bgProcessCount - 1; j++)
            {
                bgProcesses[j] = bgProcesses[j + 1];
            }
            num = bgProcesses[i].jobNo;
            bgProcessCount--;
            if (WIFEXITED(status))
                fprintf(stderr, "normally.\n");
            else if (WIFSIGNALED(status))
                fprintf(stderr, "abnormally.\n");
        }
    }
    for (int i = 0; i < bgProcessCount; i++)
    {
        if (bgProcesses[i].jobNo > num)
        {
            bgProcesses[i].jobNo--;
        }
    }
}

void handleCtrlC()
{
    if (currentFG != -2)
    {
        kill(currentFG, SIGKILL);
        currentFG = -2;
    }
}
void handleCtrlZ()
{
    if (currentFG == -2)
    {
        printRed("No foreground Process!");
        return;
    }
    kill(currentFG, SIGTSTP);
    char nameOfTask[100];
    sprintf(nameOfTask, "/proc/%d/cmdline", currentFG);

    FILE *fp = fopen(nameOfTask, "r");
    fgets(nameOfTask, 100, fp);
    fclose(fp);
    bgProcesses[bgProcessCount].pName = (char *)malloc(100 * sizeof(char));
    strcpy(bgProcesses[bgProcessCount].pName, nameOfTask);
    bgProcesses[bgProcessCount].pID = currentFG;
    bgProcesses[bgProcessCount].jobNo = bgProcessCount;
    bgProcessCount++;
    qsort(bgProcesses, bgProcessCount, sizeof(struct bgProcesses_), sortBGprocess);
    currentFG = -2;
}
int main(void)
{
    bgProcessCount = 0;
    signal(SIGCHLD, signalHandler);
    signal(SIGINT, handleCtrlC);
    signal(SIGTSTP, handleCtrlZ);
    terminate = false;
    setUp();
    while (!terminate)
    {
        prompt();
        getInput();
    }
    saveHistory();
    return 0;
}