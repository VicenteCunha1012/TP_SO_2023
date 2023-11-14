#include <stdio.h>
#include <stdlib.h>

int main() {

    if (getenv("AMBIENTE") != NULL) {
        printf("%d",atoi(getenv("AMBIENTE")));
    }

    return 0;
}
