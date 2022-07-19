#include "headers.h"

char **getCommands(int *totCommands)
{
    char *command;
    char *commandList;
    char *commandText;
    size_t maxCharacters = 10000;
    commandList = (char *)malloc(maxCharacters * sizeof(char));
    if (getline(&commandList, &maxCharacters, stdin) == -1)
    {
        terminate = true;
        char **invalid;
        return invalid;
    }
    // printf("%s", commandList);
    const char delim1[2] = ";";
    command = strtok(commandList, delim1);
    char **commands;
    commands = (char **)malloc(100 * sizeof(char *));

    *totCommands = 0;
    while (command != NULL)
    {
        if (totalHistory == 20)
        {
            free(history[0].commands);
            for (int i = 0; i < 19; i++)
            {
                history[i] = history[i + 1];
            }
            totalHistory--;
        }
        commands[*totCommands] = (char *)malloc(1000 * sizeof(char));
        strcpy(commands[*totCommands], command);
        history[totalHistory].commands = (char *)malloc((strlen(command) + 5) * sizeof(char));
        if (totalHistory == 0 || strcmp(history[totalHistory - 1].commands, command) != 0)
        {
            strcpy(history[totalHistory].commands, command);
            totalHistory++;
        }
        command = strtok(NULL, delim1);
        (*totCommands)++;
        if (*totCommands >= 100)
            break;
    }
    free(commandList);
    return commands;
}

void executeCommand(char **command, int totParams)
{
    if (totParams == 0)
        return;
    bool valid = false;
    for (int ii = 0; ii < strlen(command[totParams - 1]); ii++)
    {
        if (command[totParams - 1][ii] != ' ' && command[totParams - 1][ii] != '\n')
            valid = true;
    }
    if (!valid)
    {
        totParams--;
    }
    if (strcmp(command[0], "ls") == 0)
    {
        lsCommand(command, totParams);
    }
    else if (strcmp(command[0], "cd") == 0)
    {
        if (totParams == 1)
        {
            cdCommand(homeDir);
        }
        else if (totParams > 2)
        {
            printRed("Too many Arguments\n");
        }
        else
            cdCommand(command[1]);
    }
    else if (strcmp(command[0], "echo") == 0)
    {
        echoCommand(command, totParams);
    }
    else if (strcmp(command[0], "pinfo") == 0)
    {
        if (totParams == 1)
            pinfoCommand(1, "");
        else if (totParams == 2)
            pinfoCommand(2, command[1]);
        else
            printRed("Too many arguments.");
    }
    else if (strcmp(command[0], "quit") == 0)
    {
        terminate = true;
    }
    else if (strcmp(command[0], "exit") == 0)
    {
        terminate = true;
    }
    else if (strcmp(command[0], "pwd") == 0)
    {
        pwdCommand();
    }
    else if (strcmp(command[0], "clear") == 0)
    {
        startShell();
    }
    else if (strcmp(command[0], "repeat") == 0)
    {
        int times = atoi(command[1]);
        while (times--)
        {
            executeCommand(&command[2], totParams - 2);
        }
    }
    else if (strcmp(command[0], "history") == 0)
    {
        int till = totalHistory;
        if (totParams > 1)
        {
            if (strcmp(command[1], " ") != 0)
            {
                till = atoi(command[1]);
                if (totalHistory < till)
                {
                    till = totalHistory;
                }
            }
        }
        for (int i = 0; i < till; i++)
        {
            printf("%s", history[i].commands);
        }
    }
    else if (strcmp("jobs", command[0]) == 0)
    {
        if (totParams == 1)
        {
            printJobs();
        }
        else
        {
            printJobsWithFlag(command, totParams);
        }
    }
    else if (strcmp("sig", command[0]) == 0)
    {
        int pNo = atoi(command[1]);
        pNo--;
        int sigNum = atoi(command[2]);
        if (pNo < 0 || pNo >= bgProcessCount)
        {
            printRed("No job with the given Job Number exists");
        }
        else
        {

            for (int i = 0; i < bgProcessCount; i++)
            {
                if (bgProcesses[i].jobNo == pNo)
                {
                    kill(bgProcesses[i].pID, SIGKILL);
                }
            }
        }
    }
    else if (strcmp("fg", command[0]) == 0)
    {
        changeToForeground(command[1]);
    }
    else if (strcmp("bg", command[0]) == 0)
    {
        bg(command[1]);
    }
    else
    {
        bool background = false;
        int totLen = 0;
        for (int it = 0; it < totParams; it++)
        {
            if (strcmp("&", command[it]) == 0)
            {
                background = true;
                continue;
            }
            totLen += (strlen(command[it]) + 1);
        }
        // printf("here");
        char cmd[totLen + 10];
        int i = 0;
        for (int it = 0; it < totParams; it++)
        {
            if (strcmp("&", command[it]) == 0)
            {
                continue;
            }
            for (int j = 0; j < strlen(command[it]); j++)
            {
                cmd[i] = command[it][j];
                i++;
            }
            if (it < totParams - 1)
            {
                cmd[i] = ' ';
                i++;
            }
        }
        if (cmd[i - 1] == ' ')
        {
            cmd[i - 1] = 0;
        }
        cmd[i] = 0;
        // printRed(cmd);
        if (!background)
        {
            char *argv[64];
            parse(cmd, argv);
            execute(argv);
        }
        else
        {
            char *argv[64];
            parse(cmd, argv);
            executeBG(argv);
        }
    }
    printf("\n");
}

void breakCommand(char *command)
{
    char *parts;
    char delim[2] = " ";
    char **commandInfo;
    commandInfo = (char **)malloc(100 * sizeof(char *));
    int totParams = 0;
    parts = strtok(command, delim);
    while (parts != NULL)
    {
        commandInfo[totParams] = (char *)malloc(500 * sizeof(char));
        commandInfo[totParams] = parts;
        totParams++;
        parts = strtok(NULL, delim);
    }
    int lastWordLen = strlen(commandInfo[totParams - 1]);
    if (commandInfo[totParams - 1][lastWordLen - 1] == '\n')
        commandInfo[totParams - 1][lastWordLen - 1] = 0;
    for (int i = 0; i < totParams; i++)
    {
        if (strcmp(commandInfo[i], "\t") == 0)
        {
            for (int j = i; j < totParams - 1; j++)
            {
                strcpy(commandInfo[j + 1], commandInfo[j]);
            }
            i--;
            totParams--;
        }
    }

    // runningTheCommands(commandInfo, totParams);

    int totPipes = 0;
    for (int i = 0; i < totParams; i++)
    {
        if (strcmp(commandInfo[i], "|") == 0)
        {
            totPipes++;
        }
    }
    if (totPipes == 0)
    {
        runningTheCommands(commandInfo, totParams);
    }
    else
    {
        int pipePosn[totPipes + 2];
        pipePosn[0] = -1;
        int initials = 1;
        for (int i = 0; i < totParams; i++)
        {
            if (strcmp(commandInfo[i], "|") == 0)
            {
                pipePosn[initials] = i;
                initials++;
            }
        }
        pipePosn[initials] = totParams;
        int fileDes[2 * totPipes];
        for (int i = 0; i < 2 * totPipes; i += 2)
        {
            if (pipe(fileDes + i))
            {
                printRed("Could not create pipe");
                return;
            }
        }
        for (int i = 0; i < totPipes + 1; i++)
        {
            int pid = fork();
            if (pid == 0)
            {
                if (i < totPipes)
                {
                    dup2(fileDes[2 * i + 1], 1);
                }
                if (i > 0)
                {
                    dup2(fileDes[2 * i - 2], 0);
                }
                char *piped_command[pipePosn[i + 1] - pipePosn[i]];
                initials = 0;
                for (int j = pipePosn[i] + 1; j < pipePosn[i + 1]; j++)
                {
                    piped_command[initials] = (char *)malloc(sizeof(char) * strlen(commandInfo[j]));
                    strcpy(piped_command[initials], commandInfo[j]);
                    initials++;
                }
                piped_command[pipePosn[i + 1] - pipePosn[i] - 1] = (char *)malloc(2);
                strcpy(piped_command[pipePosn[i + 1] - pipePosn[i] - 1], "\0");
                for (int j = 0; j < 2 * totPipes; j++)
                {
                    close(fileDes[j]);
                }
                runningTheCommands(piped_command, pipePosn[i + 1] - pipePosn[i] - 1);
                int pip1 = pipePosn[i + 1];
                int pip2 = pipePosn[i];
                for (int j = 0; j < pip1 - pip2 - 1; j++)
                {
                    free(piped_command[j]);
                }
                exit(1);
            }
        }
        for (int i = 0; i < 2 * totPipes; i++)
        {
            close(fileDes[i]);
        }
        for (int pipNum = 0; pipNum <= totPipes; pipNum++)
        {
            wait(NULL);
        }
    }
}
void runningTheCommands(char **commandInfo, int totParams)
{
    int inFile = -1;
    int outFile = -1;
    bool append = false;
    int tempParams = totParams;

    for (int i = 0; i < totParams; i++)
    {
        if (strcmp(commandInfo[i], "<") == 0)
        {
            inFile = i + 1;
            if (tempParams > i)
                tempParams = i;
        }
        if (strcmp(commandInfo[i], ">") == 0)
        {
            outFile = i + 1;
            if (tempParams > i)
                tempParams = i;
        }
        if (strcmp(commandInfo[i], ">>") == 0)
        {
            outFile = i + 1;
            if (tempParams > i)
                tempParams = i;
            append = true;
        }
    }
    totParams = tempParams;
    int prevSTDIN = dup(STDIN_FILENO);
    int prevSTDOUT = dup(STDOUT_FILENO);
    int inFD;
    int outFD;

    if (inFile != -1)
    {
        inFD = open(commandInfo[inFile], O_RDONLY);
        // printf("%s\n", commandInfo[inFile]);
        if (inFD < 0)
        {
            close(prevSTDIN);
            printRed("Unable to redirect input\n");
            return;
        }
        if (dup2(inFD, STDIN_FILENO) == -1)
        {
            printRed("Unable to redirect input\n");
            return;
        }
    }
    if (outFile != -1)
    {
        int opMethod = O_TRUNC;
        if (append)
            opMethod = O_APPEND;
        outFD = open(commandInfo[outFile], O_CREAT | O_WRONLY | opMethod, 0644);
        // printf("%s\n", commandInfo[outFile]);

        if (outFD < 0)
        {
            close(prevSTDOUT);
            printRed("Unable to redirect output.");
            return;
        }
        if (dup2(outFD, STDOUT_FILENO) == -1)
        {
            printRed("Unable to redirect output.");
            return;
        }
    }
    executeCommand(commandInfo, totParams);

    if (outFile != -1)
    {
        close(outFD);
        dup2(prevSTDOUT, STDOUT_FILENO);
    }
    if (inFile != -1)
    {
        close(inFD);
        dup2(prevSTDIN, STDIN_FILENO);
    }
    // free(commandInfo);
}

void getInput()
{
    int totCommands = 0;
    char **commands = getCommands(&totCommands);
    // printf("reached");
    if (terminate)
    {
        return;
    }
    for (int i = 0; i < totCommands; i++)
    {
        breakCommand(commands[i]);
        free(commands[i]);
    }
    free(commands);
}