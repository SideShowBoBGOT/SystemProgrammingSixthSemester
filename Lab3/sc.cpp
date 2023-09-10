#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
long long get_nrows(char *file)
{
    int fd[2];
    int nrows = 0;
    char buf[255];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        char *command[4] = {"/bin/awk", "END{print NR}", file, 0};
        execvp(command[0], command);
        exit(EXIT_FAILURE);
    }
    else
    {
        read(fd[0], &buf, sizeof(buf));
        nrows = atoll(buf);
        close(fd[0]);
        close(fd[1]);
    }
    return nrows;
}
int main(int argc, char *argv[])
{
    const int NUM_PROCS = 9;
    const int NUM_PIPES = 8;
    int fds[NUM_PIPES][2], status;
    long long nrows = get_nrows(argv[1]);
    char *commands_args[NUM_PROCS][10] = {
        {"/bin/cat", argv[1], ""},
        {"/bin/awk", "BEGIN{FS=\" \"}($0 !~ /.*\"-\".*/){print $11}", ""},
        {"/bin/sed", "s/\"//g", ""},
        {"/bin/sort", "-s", ""},
        {"/bin/uniq", "-c", ""},
        {"/bin/awk", "BEGIN{FS=\" \";nrows=}{print $2\t$1\t$1/nrows*100\"%\"}", ""},
        {"/bin/sort", "-k", "3n", ""},
        {"/bin/tail", "-10", ""},
        {"/bin/tac", ""},
    };
    char res_str[255];
    sprintf(res_str, "BEGIN{FS=\" \";nrows=%lld}{print $2\"\t\"$1\"\t\"$1/nrows*100\"%\"}", nrows);
    commands_args[5][1] = res_str;
    for (int i = 0; i < NUM_PIPES; i++)
    {
        pipe(fds[i]);
    }
    pid_t pids[NUM_PROCS];
    for (int i = 0; i < NUM_PROCS; i++)
    {
        pids[i] = fork();
        if (pids[i] == 0)
        {
            if (i == 0)
            {
                dup2(fds[i][1], STDOUT_FILENO);
            }
            else if (i == NUM_PROCS - 1)
            {
                dup2(fds[i - 1][0], STDIN_FILENO);
            }
            else
            {
                dup2(fds[i - 1][0], STDIN_FILENO);
                dup2(fds[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < NUM_PIPES; j++)
            {
                close(fds[j][0]);
                close(fds[j][1]);
            }

            char *command[3];
            int j = 0;
            for (; commands_args[i][j] != ""; j++)
            {
                command[j] = commands_args[i][j];
            }
            command[j] = 0;
            execvp(command[0], command);
            exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < NUM_PIPES; i++)
    {
        close(fds[i][0]);
        close(fds[i][1]);
    }
    for (int i = 0; i < NUM_PROCS; i++)
    {
        if (i == NUM_PROCS - 1)
        {
            waitpid(pids[i], &status, 0);
        }
        else
        {
            waitpid(pids[i], 0, 0);
        }
    }
    exit(status);
    return 0;
}