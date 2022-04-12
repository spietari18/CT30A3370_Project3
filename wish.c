#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define die(msg) do {perror(msg); exit(EXIT_FAILURE);} while (0);

#define BUFSIZE 80

void signal_handler(int signal) {
    if (signal == SIGALRM) {
        printf("Inactive for 15 seconds, exitting\n");
        exit(0);
    }
}

int main(int argc, char * * argv) {
	char buffer[BUFSIZE];
    pid_t cpid;
    int wstatus;

    signal(SIGINT, signal_handler);
    printf("wish $ ");
    alarm(15);

    while (fgets(buffer, BUFSIZE, stdin) != NULL) {
        alarm(15);
        //printf("Running: %s\n", buffer);
        if (strcmp(buffer, "exit\n") == 0) {
            exit(0);
            return 0;
        }

        switch(cpid = fork()) {
            case -1:
                die("fork");
                break;

            case 0:
                system(buffer);
                exit(0);
                break;

            default:
                waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
                printf("wish $ ");
        }
    }
    return 0;
}
