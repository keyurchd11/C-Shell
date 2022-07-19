#ifndef __HEADERS__
#define __HEADERS__

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <math.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

// global variables
char hostName[1000];
char userName[1000];
char currDir[1000];
char prevDir[1000];
char homeDir[1000];
bool terminate;
char **fileNames;
int fileCount;

// display prompt
void prompt();

// formatting
void printBlue(char *toPrint);
void printCyan(char *toPrint);
void printRed(char *toPrint);
void printPaddedNumber(int num, int maxNumLen);
void resetBG();
void startShell();

// input

char **getCommands(int *totCommands);


void breakCommand(char *command);
void executeCommand(char **command, int totParams);
void runningTheCommands(char **commandInfo, int totParams);
void getInput();

// system commands

void echoCommand(char **words, int len);
void cdCommand(char *newPath);
void pwdCommand();

// Directories

bool checkDirectory(char *newPath);

// ls
int compFileNames(const void *a, const void *b);
void getAllFiles();
int prepareDetailLS(bool all);
bool printLs(bool all, bool details, bool file_, char *fileName);
void lsCommand();

// system commands

void execute(char **argv);
void parse(char *line, char **argv);
void executeBG(char **argv);

// keeping track of background processes

struct bgProcesses_
{
    char *pName;
    pid_t pID;
    int jobNo;
};
struct bgProcesses_ bgProcesses[1000];
int bgProcessCount;

// pinfo

pid_t foregroundID;
void pinfoCommand(int type, char *pidStr);

// history
int totalHistory;
struct history_
{
    char *commands;
};
struct history_ history[25];
void loadHistory();
void saveHistory();

void printJobs();
void printJobsWithFlag(char **commands, int totParams);
void printJob(int num, pid_t pid, char flag);
void changeToForeground(char *numStr);
void bg(char *jobNoStr);
int sortBGprocess(const void *a, const void *b);

// handling signals
pid_t currentFG;

#endif