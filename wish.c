#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define die(msg) do {perror(msg); exit(EXIT_FAILURE);} while (0);

void execute(FILE *input) {
    char *buffer;
    size_t bufsize = 160;

    pid_t cpid;
    int wstatus;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    while (getline(&buffer,&bufsize, input) != -1) {
        char s2[160];
        strncpy(s2, buffer, bufsize * sizeof(char));
        char *s1;
        s1 = (char *)malloc(bufsize * sizeof(char));
        if (s1 == NULL)
        {
            perror("Unable to allocate buffer");
            exit(1);
        }
        s1 = strtok(s2, " ");
        
        if (strcmp(s1, "exit\n") == 0) {
            exit(0);
        } else if (strcmp(s1, "cd") == 0){
            s1 = strtok(NULL, "\n");
            //printf("%s\n", s1);
            if (chdir(s1) == -1) {
                perror("chdir failed");
                printf("%s\n", s1);
                exit(1);
            }
            //system("ls");
            //printf("wish> ");
        } else if (strcmp(s1,"path\n") == 0){
            char cwd[1024];
            getcwd(cwd,sizeof(cwd));
            printf("%s\n",cwd);
            //printf("wish> ");

        } else {
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
                    //printf("wish> ");
            }
        }
    }
}


int main(int argc, char * * argv) {
    char *buffer;
    size_t bufsize = 160;

    pid_t cpid;
    int wstatus;

    FILE *input;

    buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }

    if (argc > 1) {
        // Open file, run commands, next file
        for (int i = 1; i < argc; i++) {
            if ((input = fopen(argv[i], "r")) == NULL) {
                perror("Failed to open file for reading");
                exit(1);
            }
            //fprintf(stderr, "Executing from %s\n", argv[i]);
            execute(input);
        }

        //exit after all files are looped through
    } else {
        printf("wish> ");

        while (getline(&buffer,&bufsize, stdin) != -1) {
            char s2[160];
            strncpy(s2, buffer, bufsize * sizeof(char));
            char *s1;
            s1 = (char *)malloc(bufsize * sizeof(char));
            if (s1 == NULL)
            {
                perror("Unable to allocate buffer");
                exit(1);
            }
            s1 = strtok(s2, " ");
            
            if (strcmp(s1, "exit\n") == 0) {
                exit(0);
                return 0;
            } else if (strcmp(s1, "cd") == 0){
                s1 = strtok(NULL, "\n");
                //printf("%s\n", s1);
                if (chdir(s1) == -1) {
                    perror("chdir failed");
                    printf("%s\n", s1);
                    exit(1);
                }
                //system("ls");
                printf("wish> ");
            } else if (strcmp(s1,"path\n") == 0){
                char cwd[1024];
                getcwd(cwd,sizeof(cwd));
                printf("%s\nwish> ",cwd);

            } else {
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
        }
    }
    
    return 0;
}
