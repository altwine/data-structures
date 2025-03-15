#include <stdio.h>

int main() {
    int N;
    scanf("%d", &N);

    int rem = 0;
    for (int i = 1; i <= N; i++) {
        rem = (rem * 10 + 1) % N;
        if (rem == 0) {
            for (int _ = 0; _ < i; _++)
                printf("1");
            return 0;
        }
    }

    printf("NO\n");
    return 0;
}
