#include <stdio.h>
#include <string.h>

int main(void) {
    // char str[] = "    study hard,    or  sleep.     ";  // char array
    char *str = "    study hard,    or  sleep.     ";   // string literal
    char delim[] = " ";
    char *token;

    token = strtok(str, delim);
    // 포인터를 이용해 string literal로 선언할 경우, 수정이 불가능 하여 strtok을 활용할 수 없음.

    while (token != NULL) {
        printf("next token is: %s:%zd\n", token, strlen(token));
        token = strtok(NULL, delim);
    }
}