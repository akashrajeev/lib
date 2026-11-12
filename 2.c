
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int a[20], n, i, j, temp;

    printf("Enter number of elements: ");                           
    scanf("%d", &n);

    if(n < 1 || n > 20)
        return 1;

    printf("Enter numbers: ");
    for(i=0; i<n; i++)
        scanf("%d", &a[i]);

    int pid = fork();

    if(pid < 0) {
        printf("Fork failed\n");
        return 1;
    }

    if(pid == 0) {
        // Child: Descending order
        for(i=0; i<n; i++) {
            for(j=i+1; j<n; j++) {
                if(a[i] < a[j]) {
                    temp = a[i];
                    a[i] = a[j];
                    a[j] = temp;
                }
            }
        }

        printf("Child descending: ");
        for(i=0; i<n; i++)
            printf("%d ", a[i]);
        printf("\n");
    }
    else {
        // Parent: Ascending order
        for(i=0; i<n; i++) {
            for(j=i+1; j<n; j++) {
                if(a[i] > a[j]) {
                    temp = a[i];
                    a[i] = a[j];
                    a[j] = temp;
                }
            }
        }

        wait(NULL);

        printf("Parent ascending: ");
        for(i=0; i<n; i++)
            printf("%d ", a[i]);
        printf("\n");
    }

    return 0;
}




#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid = fork();

    if(pid == 0) {
        printf("Child finished\n");
    }
    else {
        printf("Parent sleeping\n");
        sleep(15);
        wait(NULL);
        printf("Parent finished\n");
    }

    return 0;
}




#include <stdio.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if(pid == 0) {
        sleep(3);
        printf("Child running\n");
        printf("New Parent ID: %d\n", getppid());
    }
    else {
        printf("Parent finished\n");
    }

    return 0;
}
