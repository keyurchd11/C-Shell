#include "headers.h"

void loadHistory()
{
    char historyPath[1100];
    sprintf(historyPath, "%s/.history.txt", homeDir);
    totalHistory = 0;
    if (open(historyPath, O_RDONLY) < 0)
    {
        FILE *fp;
        fp = fopen(historyPath, "w");
        fprintf(fp, "0\n");
        fclose(fp);
    }
    else
    {
        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        fp = fopen(historyPath, "r");
        if (fp == NULL)
            exit(EXIT_FAILURE);
        read = getline(&line, &len, fp) != -1;
        totalHistory = atoi(line);
        int it = 0;
        while ((read = getline(&line, &len, fp)) != -1)
        {
            history[it].commands = (char *)malloc(sizeof(char) * 100);
            strcpy(history[it].commands, line);
            it++;
        }
        fclose(fp);
    }
    
}

void saveHistory()
{
    FILE *fp;
    char historyPath[1100];
    sprintf(historyPath, "%s/.history.txt", homeDir);
    fp = fopen(historyPath, "w+");
    fprintf(fp, "%d\n", totalHistory);
    for (int i = 0; i < totalHistory; i++)
    {
        fprintf(fp, "%s", history[i].commands);
    }
    fclose(fp);
}
