#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define die(msg) do {perror(msg); exit(EXIT_FAILURE);} while (0);




int main(int argc, char * * argv) {
    char *buffer;
    size_t bufsize = 160;
    size_t line;
    pid_t cpid;
    int wstatus;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    printf("wish> ");
    

    while (getline(&buffer,&bufsize, stdin) != NULL) {
        
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
                printf("wish> ");
        }
    }
    return 0;
}
