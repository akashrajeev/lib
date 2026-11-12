
#include <stdio.h>
#include <stdlib.h>

int a[100], used[100], n, pos, total;

void move(int x) {
    total += abs(pos - x);
    printf(" -> %d", x);
    pos = x;
}

int main() {
    int head, i, j, k, t, nearest;

    printf("Number of requests: ");
    scanf("%d", &n);
    if(n < 1 || n > 100) return 1;

    printf("Enter requests: ");
    for(i=0; i<n; i++)
        scanf("%d", &a[i]);

    printf("Initial head: ");
    scanf("%d", &head);

    // SSTF
    pos = head;
    total = 0;
    printf("SSTF: %d", head);

    for(i=0; i<n; i++) {
        nearest = -1;

        for(j=0; j<n; j++)
            if(!used[j] && (nearest==-1 ||
               abs(pos-a[j]) < abs(pos-a[nearest])))
                nearest = j;

        move(a[nearest]);
        used[nearest] = 1;
    }
    printf("\nSSTF movement = %d\n", total);

    // Sort requests for SCAN and C-LOOK
    for(i=0; i<n; i++)
        for(j=i+1; j<n; j++)
            if(a[i] > a[j]) {
                t = a[i];
                a[i] = a[j];
                a[j] = t;
            }

    k = 0;
    while(k<n && a[k]<=head)
        k++;

    // SCAN: towards cylinder 0
    pos = head;
    total = 0;
    printf("SCAN: %d", head);

    for(i=k-1; i>=0; i--)
        move(a[i]);

    if(pos != 0)
        move(0);

    for(i=k; i<n; i++)
        move(a[i]);

    printf("\nSCAN movement = %d\n", total);

    // C-LOOK: towards cylinder 0
    pos = head;
    total = 0;
    printf("C-LOOK: %d", head);

    for(i=k-1; i>=0; i--)
        move(a[i]);

    for(i=n-1; i>=k; i--)
        move(a[i]);

    printf("\nC-LOOK movement = %d\n", total);

    return 0;
}
