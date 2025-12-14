#include <stdio.h>
#include "factorial.h"

int main() {
    int num = 5;
    long long result = calculate_factorial(num);

    printf("--- [Static Linking Result] ---\n");
    printf("%d! (Factorial) = %lld\n", num, result);

    return 0;
}