#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 80 /* The maximum length command */

int process_input(char *input, char *args[], int *flag);
void print_history(char *history[], int count);
void save_history(char *input, char *history[], int count);
char *rtrim(char *str);
char *ltrim(char *str);
char *trim(char *str);

int main(void)
{
    char *history[MAX_LINE];
    char *args[MAX_LINE / 2 + 1]; /* command line arguments */
    int should_run = 1, count = 0;           /* flag to determine when to exit program */
    pid_t pid, pid1;
    while (should_run)
    {
        printf("wybsh>");
        fflush(stdout);
        int flag = 0;

        char *input = (char *)malloc(sizeof(char) * MAX_LINE);
        
        fgets(input, MAX_LINE, stdin);
        input = trim(input);
        if (strlen(input) == 0)
            continue;
        if (strcmp(input, "history") == 0)
        {
            print_history(history, count);
            continue;
        }

        char *tmp = (char *)malloc(sizeof(char) * MAX_LINE);
        strcpy(tmp, input);
        int cnt = process_input(input, args, &flag);

        if (cnt == -1)  {
            should_run = 0;
            continue;
        }

        if (cnt == 1 && strcmp(args[0], "!!") == 0)
        {
            if (count == 0) {
                printf("No commands in history.\n");
                continue;
            }
                
            strcpy(input, history[count-1]);
            process_input(input, args, &flag);
            strcpy(tmp, input);
        }

        if (cnt == 1 && args[0][0] == '!') {
            int idx;
            sscanf(args[0], "!%d", &idx);
            if (count - idx < 0) {
                printf("No such command in history.\n");
                continue;
            }
                
            strcpy(input, history[count-idx]);
            process_input(input, args, &flag);
            strcpy(tmp, input);
        }
 

        save_history(tmp, history, count++);

        pid = fork();

        if (pid < 0)
        {
            fprintf(stderr, "Fork failed!");
            return 1;
        }
        else if (pid == 0 && cnt != 0)
        {
            execvp(args[0], args);
        }
        else
        {
            pid1 = getpid();
            if (!flag)
            {
                wait(NULL);
            }
        }
    }
    return 0;
}

int process_input(char *input, char *args[], int *flag)
{
    int cnt = 0;
    char *result;
    result = strtok(input, " ");
    if (strcmp(result, "exit") == 0) {
        return -1;
    }
    while (result != NULL)
    {
        args[cnt++] = result;
        result = strtok(NULL, " ");
        if (result == NULL)
        {
            if (strcmp(args[cnt - 1], "&") == 0)
            {
                cnt--;
                *flag = 1;
            }
        }
    }
    args[cnt] = NULL;
    return cnt;
}

void save_history(char *input, char *history[], int count)
{
    history[count] = (char*)malloc(sizeof(input));
    strcpy(history[count], input);
}

void print_history(char *history[],int count)
{
    if (count == 0)
        return;
    count--;
    int i;
    for (int i = 0; i <= count; i++) {
        printf("%d: %s\n", count - i + 1, history[i]);
    }
}

char *rtrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}
 
	int len = strlen(str);
	char *p = str + len - 1;
	while (p >= str  && isspace(*p))
	{
		*p = '\0';
		--p;
	}
 
	return str;
}
 
//去除首部空格
char *ltrim(char *str)
{
	if (str == NULL || *str == '\0')
	{
		return str;
	}
 
	int len = 0;
	char *p = str;
	while (*p != '\0' && isspace(*p))
	{
		++p;
		++len;
	}
 
	memmove(str, p, strlen(str) - len + 1);
 
	return str;
}
 
//去除首尾空格
char *trim(char *str)
{
	str = rtrim(str);
	str = ltrim(str);
	
	return str;
}
