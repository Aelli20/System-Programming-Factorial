#include "factorial.h"

long long calculate_factorial(int n) {
    if (n < 0) {
        return 0;
    }
    if (n == 0 || n == 1) {
        return 1;
    }
    return (long long)n * calculate_factorial(n - 1);
}