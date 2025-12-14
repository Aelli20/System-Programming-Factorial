#include <stdio.h>
#include "factorial.h"

int main() {
    int num = 5;
    long long result = calculate_factorial(num);

    printf("--- [Dynamic Linking Result] ---\n"); // 메시지만 수정
    printf("%d! (Factorial) = %lld\n", num, result);

    return 0;
}