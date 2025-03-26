#include <stdio.h>             // printf, fprintf, stderr
#include <string.h>            // strlen, strcpy, strcmp
#include <sys/wait.h>          // wait
#include <stdlib.h>            // atoi 
#include <unistd.h>            // fork, execvp, pipe, dup2, close, STDIN_FILENO, STDOUT_FILENO
#include <readline/readline.h> // readline
#include <readline/history.h>  // add_history, history_get, history_length, history_base, clear_history
#include <limits.h>            // _POSIX_ARG_MAX -> 4096
#include <signal.h>
#include <errno.h>



// Add these global declarations at the top of the file, after the includes
char *g_parsedArgs[_POSIX_ARG_MAX] = {NULL};
char *g_parsedArgsPiped[_POSIX_ARG_MAX] = {NULL};

// Function to take input
int Input(char *str)
{
    char *buffer;
    buffer = readline("$"); // Exactly $ with no extra spaces
    if (strlen(buffer) >= _POSIX_ARG_MAX)
    {
        fprintf(stderr, "error: %s\n", "Command too long");
        free(buffer);
        return 1;
    }
    if (strlen(buffer) != 0)
    {
        HIST_ENTRY *entry = history_get(history_length);
        if (entry == NULL || strcmp(entry->line, buffer) != 0)
        {
            add_history(buffer);
        }
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

void cleanup(char **parsed, char **parsedpipe)
{
    if (parsed != NULL) {
        for (int i = 0; parsed[i] != NULL; i++)
        {
            if (parsed[i] != NULL) {
                free(parsed[i]);
                parsed[i] = NULL;
            }
        }
    }
    
    if (parsedpipe != NULL) {
        for (int i = 0; parsedpipe[i] != NULL; i++)
        {
            if (parsedpipe[i] != NULL) {
                free(parsedpipe[i]);
                parsedpipe[i] = NULL;
            }
        }
    }
}

void signalHandler(int sigNum)
{
    signal(SIGINT, signalHandler);
    printf("\n");  // Add newline for clean display
    clear_history();
    cleanup(g_parsedArgs, g_parsedArgsPiped);
    exit(0);
}

void displayHistory(int last_n)
{
    HIST_ENTRY *historyList;
    int total = history_length;
    int start = (total <= last_n) ? 0 : total - last_n;

    for (int i = start; i < total; ++i)
    {
        historyList = history_get(i + history_base);
        if (historyList)
        {
            printf("%5d  %s\n", i + history_base, historyList->line); // Right-align to 5 characters
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
    char *token;
    while (i < _POSIX_ARG_MAX)
    {
        token = strsep(&str, "|");
        if (token == NULL)
        {
            parsed[i] = NULL;
            break;
        }
        parsed[i] = strdup(token);  // Use strdup to properly allocate memory
        if (parsed[i] == NULL) {
            fprintf(stderr, "error: %s\n", strerror(errno));
            // Clean up previously allocated memory
            for (int j = 0; j < i; j++) {
                free(parsed[j]);
            }
            exit(EXIT_FAILURE);
        }
        i++;
    }
    return i - 1;
}

// Function to parse command words
void parseSpace(char *str, char **parsed)
{
    int i;
    char *token;
    for (i = 0; i < _POSIX_ARG_MAX; i++)
    {
        token = strsep(&str, " ");
        if (token == NULL)
        {
            parsed[i] = NULL;
            break;
        }
        if (strlen(token) == 0)
        {
            i--;
            continue;
        }
        parsed[i] = strdup(token);  // Use strdup to properly allocate memory
        if (parsed[i] == NULL) {
            fprintf(stderr, "error: %s\n", strerror(errno));
            // Clean up previously allocated memory
            for (int j = 0; j < i; j++) {
                free(parsed[j]);
            }
            exit(EXIT_FAILURE);
        }
    }
    parsed[i] = NULL;  // Ensure array is NULL-terminated
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
            fprintf(stderr, "error: %s\n", strerror(errno));
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
                if (dup2(pipefds[(j - 1) * 2], STDIN_FILENO) < 0)
                {
                    fprintf(stderr, "error: %s\n", strerror(errno));
                    exit(EXIT_FAILURE);
                }
            }
            // If not last command
            if (parsed[j + 1] != NULL)
            {
                if (dup2(pipefds[j * 2 + 1], STDOUT_FILENO) < 0)
                {
                    fprintf(stderr, "error: %s\n", strerror(errno));
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
                fprintf(stderr, "error: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        else if (pid < 0)
        {
            fprintf(stderr, "error: %s\n", strerror(errno));
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
            fprintf(stderr, "error: %s\n", "cd: missing argument");
        }
        else if (chdir(parsed[1]) != 0)
        {
            fprintf(stderr, "error: %s\n", strerror(errno));
        }
        return 1;
    case 2: // exit
        exit(0);
        return 1;
    case 3: // history [-c][n]
        if (parsed[1] == NULL)
        {
            displayHistory(10); // display last 10 commands
        }
        else if (strcmp(parsed[1], "-c") == 0)
        {
            clearHistory();
        }
        else
        {
            char *endPtr;
            long n = strtol(parsed[1], &endPtr, 10);
            if (*endPtr != '\0' || n <= 0)
            {
                fprintf(stderr, "error: %s\n", "history: argument must be a positive number");
            }
            else if (n > 10)
            {
                displayHistory(10);
            }
            else
            {
                displayHistory((int)n);
            }
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
    int piped = parsePipe(str, strpiped);

    if (piped > 0)
    {
        execArgsPiped(strpiped, piped);
        return 1 + piped;
    }
    else
    {
        parseSpace(str, parsed);
        if (cmdHandler(parsed))
            return 0;
        return 1;
    }
}

int main()
{
    char inputString[_POSIX_ARG_MAX];
    int execFlag = 0;
    signal(SIGINT, signalHandler); // SIGINT means interrupt
    while (1)
    {
        if (Input(inputString))
        {
            continue;
        }

        // process
        execFlag = processString(inputString, g_parsedArgs, g_parsedArgsPiped);

        // execute
        if (execFlag == 1)
        {
            executeArgs(g_parsedArgs);
        }
        if (execFlag > 1)
        {
            execArgsPiped(g_parsedArgs, execFlag - 1);
        }
    }

    cleanup(g_parsedArgs, g_parsedArgsPiped);
    return 0;
}
