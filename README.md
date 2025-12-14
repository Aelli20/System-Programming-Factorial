# Factorial Calculator: Static, Dynamic, and Runtime Linking

본 문서는 **Static Linking**, **Dynamic Linking**, **Runtime Linking**에 관한 구현 내용을 포함합니다.

C언어에서 재귀 함수를 이용해 팩토리얼(Factorial)을 계산하는 함수를 디자인하고, 세 가지 방식으로 링킹하는 과정을 설명합니다.

### 🖥️ 개발환경
* **OS** : Ubuntu 20.04 LTS
* **Compiler** : GCC 9.4.0

### ☘️ 리포지토리 구조
```bash
.
├── factorial_static
│   ├── main.c
│   ├── factorial.c
│   ├── factorial.h
│   ├── factorial.o
│   ├── libfact.a
│   └── main_static
├── factorial_dynamic
│   ├── main.c
│   ├── factorial.c
│   ├── factorial.h
│   ├── factorial.o
│   ├── libfact.so
│   └── main_dynamic
├── factorial_runtime
│   ├── main_runtime.c
│   ├── factorial.c
│   ├── factorial.h
│   ├── factorial.o
│   ├── libfact.so
│   └── main_runtime
└── README.md
```

🐿️ 코드 설명

factorial.h
다음의 헤더 파일은 calculate_factorial() 함수 프로토타입을 선언합니다.
```c
#ifndef FACTORIAL_H
#define FACTORIAL_H

long long calculate_factorial(int n);

#endif
```
factorial.c
다음의 C 파일은 헤더 파일을 포함하고, 함수 본문을 구현합니다. 재귀 호출을 사용하여 팩토리얼을 계산하는 함수를 디자인하였습니다.
```c
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
```

main.c
라이브러리를 사용하는 메인 프로그램입니다. calculate_factorial() 함수를 사용하여 계산 결과를 출력합니다.
```c
#include <stdio.h>
#include "factorial.h"

int main() {
    int num = 5;
    long long result = calculate_factorial(num);

    printf("--- [Linking Result] ---\n");
    printf("%d! (Factorial) = %lld\n", num, result);

    return 0;
}
```
main_runtime.c
Runtime Linking에서 사용되는 main 코드입니다. dlopen과 dlsym을 사용해 런타임에 동적으로 라이브러리를 로드하고 calculate_factorial 함수를 호출합니다.
```c
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // 런타임 링킹 함수 헤더

int main() {
    void *handle;
    long long (*factorial_func)(int); 
    char *error;

    // dlopen을 사용한 런타임 라이브러리 로드
    handle = dlopen("./libfact.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        exit(1);
    }

    // dlsym을 사용한 함수 심볼 로드
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
```

Static Linking
해당 과정의 실행은 factorial_static 폴더에서 이루어집니다.
```bash
1.객체 파일 생성 factorial.c 파일을 gcc를 통해 컴파일하여 객체 파일을 생성합니다.
$ gcc -c factorial.c -o factorial.o
2.아카이브 파일 생성 정적 라이브러리를 생성하는 ar -rcs 명령어를 입력합니다.
$ ar -rcs libfact.a factorial.o
3.컴파일 정적 라이브러리를 사용해서 실행 파일인 main_static을 생성합니다.
$ gcc -static main.c -L. -lfact -o main_static
4.실행 main_static 파일을 실행합니다.
$ ./main_static
```
<img width="907" height="282" alt="image" src="https://github.com/user-attachments/assets/9ec5f3f1-4a68-4b68-801e-4fca6f1a6720" />


Dynamic Linking
```bash
해당 과정의 실행은 factorial_dynamic 폴더에서 이루어집니다.
1.객체 파일 생성 factorial.c를 위치독립코드(PIC)로 컴파일하여 객체 파일을 생성합니다.
$ gcc -fPIC -c factorial.c -o factorial.o
2.동적 라이브러리 생성 동적 라이브러리를 생성하는 코드를 입력합니다.
$ gcc -shared -o libfact.so factorial.o
3.컴파일 및 링크 현재 디렉토리의 동적 라이브러리를 링크하고, 메인 파일을 컴파일합니다.
$ gcc main.c -L. -lfact -o main_dynamic
4.환경변수 설정 현재 디렉토리를 동적 라이브러리 검색 경로로 만들어 환경변수를 설정합니다.
$ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
5.실행 main_dynamic 파일을 실행합니다.
$ ./main_dynamic
```
<img width="913" height="341" alt="image" src="https://github.com/user-attachments/assets/00fd977c-93e3-4996-a542-e0ca326cf291" />

Runtime Linking
해당 과정은 factorial_runtime 폴더에서 이루어집니다.
```bash
1.객체 파일 생성 factorial.c를 위치독립코드(PIC)로 컴파일하여 객체 파일을 생성합니다.
$ gcc -fPIC -c factorial.c -o factorial.o
2.동적 라이브러리 생성 동적 라이브러리를 생성하는 코드를 입력합니다.
$ gcc -shared -o libfact.so factorial.o
3.컴파일 런타임 링크를 사용하도록 수정된 main_runtime.c를 컴파일합니다. (-ldl 옵션 필수)
$ gcc main_runtime.c -ldl -o main_runtime
4.환경변수 설정 현재 디렉토리를 동적 라이브러리 검색 경로로 만들어 환경변수를 설정합니다.
$ export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
5.실행 main_runtime 파일을 실행합니다.
$ ./main_runtime
```
<img width="915" height="339" alt="image" src="https://github.com/user-attachments/assets/1fde0d26-80b0-49b1-aae6-626f9fc49713" />





