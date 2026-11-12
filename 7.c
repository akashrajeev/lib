
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int p1[2], p2[2];
    char file[100]={0}, ch;

    pipe(p1);
    pipe(p2);

    int pid = fork();

    if(pid < 0)
        return 1;

    if(pid == 0) {
        // Child process
        close(p1[1]);
        close(p2[0]);

        read(p1[0], file, sizeof(file));
        close(p1[0]);

        FILE *fp = fopen(file, "r");

        if(fp == NULL) {
            char msg[] = "File not found\n";
            write(p2[1], msg, strlen(msg));
        }
        else {
            int c;
            while((c = fgetc(fp)) != EOF) {
                ch = c;
                write(p2[1], &ch, 1);
            }
            fclose(fp);
        }

        close(p2[1]);
    }
    else {
        // Parent process
        close(p1[0]);
        close(p2[1]);

        printf("Enter filename: ");
        scanf("%99s", file);

        write(p1[1], file, strlen(file)+1);
        close(p1[1]);

        printf("File contents:\n");

        while(read(p2[0], &ch, 1) > 0)
            putchar(ch);

        close(p2[0]);
        wait(NULL);
    }

    return 0;
}
