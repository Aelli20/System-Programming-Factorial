#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // 런타임 링킹 함수를 위한 헤더

int main() {
    void *handle;
    long long (*factorial_func)(int); 
    char *error;

    // dlopen, dlsym, dlclose를 사용한 런타임 로딩 로직
    handle = dlopen("./libfact.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        exit(1);
    }

    factorial_func = dlsym(handle, "calculate_factorial");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "dlsym error: %s\n", error);
        exit(1);
    }

    int num = 5;
    long long result = factorial_func(num);

    printf("--- [Runtime Linking Result] ---\n");
    printf("%d! (Factorial) = %lld (Runtime Linked)\n", num, result);

    if (dlclose(handle) < 0) {
        fprintf(stderr, "dlclose error: %s\n", dlerror());
        exit(1);
    }

    return 0;
}