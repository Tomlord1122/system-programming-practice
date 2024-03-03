#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <limits.h> // _POSIX_ARG_MAX
#include <signal.h>
#include <errno.h>

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
        return 0;
    }
    else
    {
        return 1; // empty string
    }
}

void signalHandler(int sigNum)
{
    signal(SIGINT, signalHandler);
    fflush(stdout);
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
    printf("\nDir: %s", cwd);
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
    else // paren
    {
        wait(NULL);
        return;
    }
}

// Function where the piped system commands is executed
void execArgsPiped(char **parsed, char **parsedpipe)
{

    // pipefd[0] refers to the read end of the pipe
    // pipefd[1] refers to the write end of the pipe
    // 第一個輸出連到第二個輸入
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0)
    {
        fprintf(stderr, "error: %s\n", "Pipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0)
    {

        fprintf(stderr, "error: %s\n", "Could not fork");
        return;
    }

    if (p1 == 0)
    {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);               // close read end
        dup2(pipefd[1], STDOUT_FILENO); // 將標準輸出重定向到pipefd[1]
        close(pipefd[1]);               // close write end

        if (execvp(parsed[0], parsed) < 0)
        {
            fprintf(stderr, "error: %s\n", "Could not execute command 1..");

            exit(0);
        }
    }
    else
    {
        // Parent executing
        p2 = fork();

        if (p2 < 0)
        {
            fprintf(stderr, "error: %s\n", "Could not fork");
            return;
        }

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0)
            {
                fprintf(stderr, "error: %s\n", "Could not execute command 2..");
                exit(0);
            }
        }
        else
        {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }
    }
}

// Function to execute builtin commands
int cmdHandler(char **parsed)
{
    int NoOfCmds = 3;

    int switchArg = 0;
    char *ListofCmds[NoOfCmds];
    char *username;

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
        chdir(parsed[1]);
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

// Function to parse command words
int parsePipe(char *str, char **parsed)
{
    for (int i = 0; i < 2; i++)
    {
        parsed[i] = strsep(&str, "|");
        if (parsed[i] == NULL)
        {
            break;
        }
    }

    if (parsed[1] == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
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

// Function to process the command
int processString(char *str, char **parsed, char **parsedpipe)
{

    char *strpiped[2];
    int piped = 0;
    piped = parsePipe(str, strpiped); // 有pipe就會回傳1

    if (piped)
    {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    }
    else
    {

        parseSpace(str, parsed);
    }

    if (cmdHandler(parsed))
        return 0;
    else
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
        printDir();
        if (Input(inputString))
        {
            continue;
        }

        // process
        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);

        // 0 -> no command or builtin command
        // 1 -> simple command
        // 2 -> piped command

        // execute
        if (execFlag == 1)
        {
            executeArgs(parsedArgs);
        }
        if (execFlag == 2)
        {
            execArgsPiped(parsedArgs, parsedArgsPiped);
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
