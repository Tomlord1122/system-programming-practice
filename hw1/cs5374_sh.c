#include <stdio.h>             // printf, fprintf, stderr
#include <string.h>            // strlen, strcpy, strcmp
#include <sys/wait.h>          // wait
#include <stdlib.h>            // atoi
#include <unistd.h>            // fork, execvp, pipe, dup2, close, STDIN_FILENO, STDOUT_FILENO
#include <readline/readline.h> // readline
#include <readline/history.h>  // add_history, history_get, history_length, history_base, clear_history
#include <limits.h>            // _POSIX_ARG_MAX
#include <signal.h>

// _POSIX_ARG_MAX  => max number of commands to be supported
#define clear() printf("\033[H\033[J") // \033 is escape character, \033[H moves cursor to top left, \033[J clears screen

// Function to take input
int Input(char *str)
{
    char *buffer;
    buffer = readline("\n$ "); // read a line from the user
    if (strlen(buffer) != 0)
    {
        add_history(buffer);
        strcpy(str, buffer); // 把 buffer 複製到 str
        free(buffer);
        return 0;
    }
    else
    {
        free(buffer);
        return 1; // empty string
    }
}

void signalHandler(int sigNum)
{
    signal(SIGINT, signalHandler);
    fflush(stdout);
    printf("\nTerminating the shell...\n"); // 提醒用戶 shell 正在終止
    exit(0);
}

void displayHistory(int last_n)
{
    HIST_ENTRY *historyList;
    int total = history_length; // Total number of commands in history
    int start = (total <= last_n) ? 0 : total - last_n;

    for (int i = start; i < total; ++i)
    {
        historyList = history_get(i + history_base);
        if (historyList)
        {
            printf("%5d  %s\n", i + history_base, historyList->line);
        }
    }
}

void clearHistory()
{
    clear_history();
}

// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\n%s", cwd);
}

// return the number of pipe
int parsePipe(char *str, char **parsed)
{
    int i = 0;
    while (i < _POSIX_ARG_MAX)
    {
        parsed[i] = strsep(&str, "|");
        if (parsed[i] == NULL)
        {
            break;
        }
        i++;
    }
    return i - 1;
}

// Function to parse command words
void parseSpace(char *str, char **parsed)
{
    int i;
    for (i = 0; i < _POSIX_ARG_MAX; i++)
    {
        parsed[i] = strsep(&str, " "); // strsep: 用空格分割字串
        if (parsed[i] == NULL)         // 沒有更多空格了
        {
            break;
        }
        if (strlen(parsed[i]) == 0) // 讓parsed[i]不是空字串
        {
            i--;
        }
    }
}

// Function to execute system command
void executeArgs(char **parsed) // 接受字符串數組
{
    // Fork a child
    pid_t pid = fork();

    if (pid == -1) // fork fail
    {
        fprintf(stderr, "error: %s\n", "fork fail");
    }
    else if (pid == 0) // success
    {
        if (execvp(parsed[0], parsed) < 0) // 在當前process執行parsed[0]指定的程序
        {
            fprintf(stderr, "error: %s\n", "execvp fail");
        }
        exit(0);
    }
    else // parent
    {
        wait(NULL);
        return;
    }
}

// Function where the piped system commands is executed
void execArgsPiped(char **parsed, int pipeNum)
{
    int pipefds[2 * pipeNum];
    // initPipe(pipefds, pipeNum);
    for (int i = 0; i < pipeNum; i++)
    {
        if (pipe(pipefds + i * 2) < 0)
        {
            fprintf(stderr, "error: %s\n", "pipe fail");
        }
    }

    int pid;
    int j = 0;
    while (parsed[j])
    {
        pid = fork();
        if (pid == 0)
        {
            // If not first command, redirect stdin
            if (j != 0)
            {
                //
                if (dup2(pipefds[(j - 1) * 2], STDIN_FILENO) < 0)
                {
                    fprintf(stderr, "error: %s\n", "dup2 fail");
                    exit(EXIT_FAILURE);
                }
            }
            // If not last command
            if (parsed[j + 1] != NULL)
            {
                if (dup2(pipefds[j * 2 + 1], STDOUT_FILENO) < 0)
                {
                    fprintf(stderr, "error: %s\n", "dup2 fail");
                    exit(EXIT_FAILURE);
                }
            }
            // Close all pipes
            for (int i = 0; i < 2 * pipeNum; i++)
            {
                close(pipefds[i]);
            }
            char *parsedCommand[_POSIX_ARG_MAX];
            parseSpace(parsed[j], parsedCommand); // Assuming parseSpace is a function that splits the command into program and arguments
            if (execvp(parsedCommand[0], parsedCommand) < 0)
            {
                fprintf(stderr, "error: %s\n", "execvp fail");
                exit(EXIT_FAILURE);
            }
        }
        else if (pid < 0)
        {
            fprintf(stderr, "error: %s\n", "fork fail");
            exit(EXIT_FAILURE);
        }
        j++;
    }

    for (int i = 0; i < 2 * pipeNum; i++)
    {
        close(pipefds[i]);
    }

    for (int i = 0; i < pipeNum + 1; i++)
    {
        wait(NULL);
    }
}

// Function to execute builtin commands
int cmdHandler(char **parsed)
{
    int NoOfCmds = 3;

    int switchArg = 0;
    char *ListofCmds[NoOfCmds];

    ListofCmds[0] = "cd";
    ListofCmds[1] = "exit";
    ListofCmds[2] = "history";

    // 找是哪一個command
    for (int i = 0; i < NoOfCmds; i++)
    {
        if (strcmp(parsed[0], ListofCmds[i]) == 0) // check if parsed[0] is equal to ListofCmds[i]
        {
            switchArg = i + 1;
            break;
        }
    }

    switch (switchArg)
    {
    case 1: // cd [dir]
        if (parsed[1] == NULL)
        {
            // 如果沒有提供目錄名，則改為用戶的家目錄
            char *homeDir = getenv("HOME");
            if (homeDir == NULL)
            {
                fprintf(stderr, "error: %s\n", "cd: HOME not set");
            }
            else if (chdir(homeDir) != 0)
            {
                fprintf(stderr, "error: %s\n", "cd: fail to change to home directory");
            }
        }
        else if (chdir(parsed[1]) != 0)
        {
            fprintf(stderr, "error: %s\n", "cd: fail to change directory");
        }
        return 1;
    case 2: // exit
        exit(0);
        return 1;
    case 3: // history [-c][n]
        if (parsed[1] == NULL)
        {
            displayHistory(10); // Display last 10 commands
        }
        else if (strcmp(parsed[1], "-c") == 0)
        {
            clearHistory(); // Clear history
        }
        else
        {
            int n = atoi(parsed[1]);
            displayHistory(n); // Display last n commands
        }
        return 1;
    default:
        break;
    }
    return 0;
}

// Function to process the command
int processString(char *str, char **parsed, char **parsedpipe)
{

    char *strpiped[_POSIX_ARG_MAX];
    int piped = 0;
    piped = parsePipe(str, strpiped); // return the number of pipe

    if (piped > 0)
    {
        execArgsPiped(strpiped, piped);
    }
    else // no pipe
    {

        parseSpace(str, parsed);
        if (cmdHandler(parsed))
            return 0;
        else
            return 1;
    }
    return 1 + piped;
}

int main()
{
    char inputString[_POSIX_ARG_MAX];
    char *parsedArgs[_POSIX_ARG_MAX];
    char *parsedArgsPiped[_POSIX_ARG_MAX];
    int execFlag = 0;

    while (1)
    {
        signal(SIGINT, signalHandler);
        // take input
        // printDir();
        printf("\n");
        if (Input(inputString))
        {
            continue;
        }

        // process
        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);

        // execute
        if (execFlag == 1)
        {
            executeArgs(parsedArgs);
        }
        if (execFlag > 1)
        {
            execArgsPiped(parsedArgs, execFlag - 1);
        }
    }

    for (int i = 0; parsedArgs[i] != NULL; i++)
    {
        free(parsedArgs[i]);
    }
    for (int i = 0; parsedArgsPiped[i] != NULL; i++)
    {
        free(parsedArgsPiped[i]);
    }

    return 0;
}
