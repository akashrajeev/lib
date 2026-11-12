
#include <stdio.h>

int main() {
    int n, m, i, j, k;
    int alloc[10][10], max[10][10], need[10][10];
    int avail[10], finish[10]={0}, safe[10];
    int count=0;

    printf("Enter processes and resources: ");
    scanf("%d%d", &n, &m);

    if(n<1 || n>10 || m<1 || m>10)
        return 1;

    printf("Enter Allocation matrix:\n");
    for(i=0; i<n; i++)
        for(j=0; j<m; j++)
            scanf("%d", &alloc[i][j]);

    printf("Enter Maximum matrix:\n");
    for(i=0; i<n; i++)
        for(j=0; j<m; j++) {
            scanf("%d", &max[i][j]);
            need[i][j] = max[i][j] - alloc[i][j];
        }

    printf("Enter Available resources:\n");
    for(j=0; j<m; j++)
        scanf("%d", &avail[j]);

    while(count < n) {
        int found = 0;

        for(i=0; i<n; i++) {
            if(finish[i] == 0) {

                for(j=0; j<m; j++)
                    if(need[i][j] > avail[j])
                        break;

                if(j == m) {
                    for(k=0; k<m; k++)
                        avail[k] += alloc[i][k];

                    safe[count] = i;
                    count++;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }

        if(found == 0)
            break;
    }

    if(count == n) {
        printf("System is SAFE\n");
        printf("Safe Sequence: ");

        for(i=0; i<n; i++)
            printf("P%d ", safe[i]);
    }
    else {
        printf("System is UNSAFE\n");
    }

    return 0;
}
