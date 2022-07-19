#include "headers.h"

int compFileNames(const void *a, const void *b)
{
    const char **ia = (const char **)a;
    const char **ib = (const char **)b;
    return strcmp(*ia, *ib);
    /* strcmp functions works exactly as expected from
	comparison function */
}

void getAllFiles()
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    fileNames = (char **)malloc(5000 * sizeof(char *));
    fileCount = 0;
    if (d)
    {
        dir = readdir(d);
        while ((dir = readdir(d)) != NULL)
        {
            fileNames[fileCount] = (char *)malloc(100 * sizeof(char));
            strcpy(fileNames[fileCount], dir->d_name);
            fileCount++;
        }
        closedir(d);
    }
    qsort(fileNames, fileCount, sizeof(char *), compFileNames);
}

int maxLink = 0, maxUser = 0, maxGroup = 0, maxSz = 0, sizeSum = 0;
int prepareDetailLS(bool all)
{
    char tempPath[1000];
    getcwd(tempPath, 1000);
    for (int it = 0; it < fileCount; it++)
    {
        if (!all && fileNames[it][0] == '.')
            continue;
        int totLen = strlen(tempPath + 2 + strlen(fileNames[it]));
        char filePath[totLen];
        int i = 0;
        for (i = 0; i < strlen(tempPath); i++)
            filePath[i] = tempPath[i];
        filePath[i] = '/';
        i++;
        for (int j = 0; j < strlen(fileNames[it]); j++)
        {
            filePath[i] = fileNames[it][j];
            i++;
        }
        filePath[i] = 0;
        struct stat s;
        if (lstat(filePath, &s) == 0)
        {
            // total
            sizeSum += s.st_blocks / 2;
            // link
            int linkSz = log10(s.st_nlink) + 1;
            if (linkSz > maxLink)
                maxLink = linkSz;
            char *usr = getpwuid(getuid())->pw_name,
                 *grp = getgrgid(getgid())->gr_name;
            // user
            if (strlen(usr) > maxUser)
                maxUser = strlen(usr);
            // group
            if (strlen(grp) > maxGroup)
                maxGroup = strlen(grp);
            // size
            if (maxSz < log10(s.st_size) + 1)
                maxSz = log10(s.st_size) + 1;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

bool printLs(bool all, bool details, bool file_, char *fileName)
{
    char tempPath[1000];
    getcwd(tempPath, 1000);
    if (!file_)
    {
        if (details)
        {
            if (prepareDetailLS(all) == -1)
            {
                printRed("Invalid arguments passed.");
            }
            printf("total: %d\n", sizeSum);
        }
        for (int it = 0; it < fileCount; it++)
        {
            if (!all && fileNames[it][0] == '.')
                continue;
            if (details)
            {
                int totLen = strlen(tempPath + 2 + strlen(fileNames[it]));
                char filePath[totLen];
                int i = 0;
                for (i = 0; i < strlen(tempPath); i++)
                    filePath[i] = tempPath[i];
                filePath[i] = '/';
                i++;
                for (int j = 0; j < strlen(fileNames[it]); j++)
                {
                    filePath[i] = fileNames[it][j];
                    i++;
                }
                filePath[i] = 0;
                struct stat s;
                if (lstat(filePath, &s) < 0)
                {
                    perror(filePath);
                    return false;
                }
                // print if it is directory
                if (S_ISDIR(s.st_mode))
                {
                    printf("d");
                }
                else
                {
                    printf("-");
                }
                // print the permissions
                int access[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                                 S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
                for (int i = 0; i < 9; i++)
                {
                    if (s.st_mode & access[i])
                    {
                        if (i % 3 == 0)
                        {
                            printf("r");
                        }
                        else if (i % 3 == 1)
                        {
                            printf("w");
                        }
                        else
                        {
                            printf("x");
                        }
                    }
                    else
                    {
                        printf("-");
                    }
                }
                printf(" ");
                // Link
                printPaddedNumber(s.st_nlink, maxLink);
                // printing the user and group of each file
                // https://stackoverflow.com/questions/7328327
                char *usrName = getpwuid(getuid())->pw_name,
                     *groupName = getgrgid(getgid())->gr_name;
                printf(" %.*s %.*s  ", maxUser, usrName, maxGroup, groupName);

                // print the size of each file
                printPaddedNumber(s.st_size, maxSz);

                // printing date
                struct tm *tm;
                char last_modified[200];
                time_t t = s.st_mtime;
                // get current time
                tm = localtime(&t);
                if (tm == NULL)
                {
                    perror("localtime");
                    return false;
                }
                time_t current_time = time(0);
                // if difference between current time and last modified is 6 months
                if (current_time - t < 180 * 24 * 60 * 60)
                    strftime(last_modified, sizeof(last_modified), "%b %d %R", tm);
                else
                    strftime(last_modified, sizeof(last_modified), "%b %d %Y", tm);
                printf("  %s ", last_modified);

                printf("%s", fileNames[it]);
                printf("\n");
            }
            else
            {
                // just the file name
                printf("%s", fileNames[it]);
                printf("\n");
            }
        }
    }
    else
    {
        bool Found = false;
        for (int it = 0; it < fileCount; it++)
        {
            if (strcmp(fileName, fileNames[it]) == 0)
            {
                Found = true;
                if (details)
                {
                    int totLen = strlen(tempPath + 2 + strlen(fileNames[it]));
                    char filePath[totLen];
                    int i = 0;
                    for (i = 0; i < strlen(tempPath); i++)
                        filePath[i] = tempPath[i];
                    filePath[i] = '/';
                    i++;
                    for (int j = 0; j < strlen(fileNames[it]); j++)
                    {
                        filePath[i] = fileNames[it][j];
                        i++;
                    }
                    filePath[i] = 0;
                    struct stat s;
                    if (lstat(filePath, &s) < 0)
                    {
                        perror(filePath);
                        return false;
                    }
                    if (S_ISDIR(s.st_mode))
                    {
                        printf("d");
                    }
                    else
                    {
                        printf("-");
                    }
                    // print the permissions
                    int access[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                                     S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
                    for (int i = 0; i < 9; i++)
                    {
                        if (s.st_mode & access[i])
                        {
                            if (i % 3 == 0)
                            {
                                printf("r");
                            }
                            else if (i % 3 == 1)
                            {
                                printf("w");
                            }
                            else
                            {
                                printf("x");
                            }
                        }
                        else
                        {
                            printf("-");
                        }
                    }
                    // Link
                    printf(" %ld", s.st_nlink);

                    // printing the user, group, and size of file
                    // https://stackoverflow.com/questions/7328327
                    char *usrName = getpwuid(getuid())->pw_name,
                         *groupName = getgrgid(getgid())->gr_name;
                    printf(" %s %s %ld", usrName, groupName, s.st_size);

                    // printing date
                    struct tm *tm;
                    char last_modified[200];
                    time_t t = s.st_mtime;
                    // get current time
                    tm = localtime(&t);
                    if (tm == NULL)
                    {
                        perror("localtime");
                        return false;
                    }
                    time_t current_time = time(0);
                    // if difference between current time and last modified is 6 months
                    if (current_time - t < 180 * 24 * 60 * 60)
                        strftime(last_modified, sizeof(last_modified), "%b %d %R", tm);
                    else
                        strftime(last_modified, sizeof(last_modified), "%b %d %Y", tm);
                    printf("  %s ", last_modified);
                }
            }
        }
        return Found;
    }
    return true;
}

void lsCommand(char **arguments, int totalArgs)
{
    bool all = false;
    bool details = false;
    bool pathFlag = false;
    bool pathFound = false;
    maxLink = 0;
    maxUser = 0;
    maxGroup = 0;
    maxSz = 0;
    sizeSum = 0;
    char lsPath[1000];
    lsPath[0] = 0;
    bool invalid = false;
    for (int i = 1; i < totalArgs; i++)
    {
        if (arguments[i][0] == '-')
        {
            for (int j = 1; j < strlen(arguments[i]); j++)
            {
                if (arguments[i][j] == 'l')
                    details = true;
                else if (arguments[i][j] == 'a')
                    all = true;
                else
                {
                    invalid = true;
                }
            }
        }
    }
    if (invalid)
    {
        printRed("Invalid Flag.");
        return;
    }
    for (int i = 1; i < totalArgs; i++)
    {
        if (arguments[i][0] == '-')
        {
            continue;
        }
        else if (lsPath[0] == 0)
        {
            pathFlag = true;
            strcpy(lsPath, arguments[i]);
            pathFound = true;
        }
        if (pathFlag)
        {
            printf("%s:\n", lsPath);
            if (checkDirectory(lsPath))
            {
                chdir(lsPath);
                getAllFiles();
                printLs(all, details, false, "");
                chdir(currDir);
            }
            else
            {
                int len = strlen(lsPath);
                int i;
                for (i = len - 1; i >= 0; i--)
                {
                    if (lsPath[i] == '/')
                    {
                        break;
                    }
                }
                char fileName[len + 2 - i];
                if (i >= 0)
                {
                    lsPath[i] = 0;
                }
                i++;
                int j = 0;
                for (i; i < len; i++)
                {
                    fileName[j] = lsPath[i];
                    j++;
                }
                fileName[j] = 0;

                if (checkDirectory(lsPath))
                {
                    chdir(lsPath);
                    getAllFiles();
                    if (printLs(all, details, true, fileName))
                    {
                        chdir(currDir);
                        printf("%s/%s\n", lsPath, fileName);
                    }
                    else
                    {
                        printRed("invalid arguments.\n");
                    }
                }
                else
                {
                    printRed(lsPath);
                    printRed("/");
                    printRed(fileName);
                    printRed("is an invalid arguments\n");
                    return;
                }
            }
            lsPath[0] = 0;
            pathFlag = false;
        }
    }
    if (pathFound)
    {
        ;
    }
    else
    {
        getAllFiles();
        printLs(all, details, false, "");
    }

    free(fileNames);
}
