#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int erase_blanks(char *expression) {
    char *buffer = (char *)malloc(sizeof(char) * strlen(expression));
    strcpy(buffer, expression);

    int idx = 0;
    for (int i = 0; i < strlen(buffer); i++) {
        char ch = buffer[i];
        if (ch == ' ')
            continue;
        expression[idx] = ch;
        idx++;
    }
    expression[idx + 1] = '\0';
    free(buffer);
    return idx;
}

int erase_blanks_two(char *expression) {
    int i = 0;
    while (expression[i] != '\0') {
        if (expression[i] == ' ') {
            int l = strlen(expression);
            for (int j = i; j < l - 1; j++) {
                expression[j] = expression[j + 1];
            }
            expression[l - 1] = '\0';
            continue;
        }
        else
            i++;
    }
    return i;
}



int main() {

    char str[100] = "  dfdml +  = ddfd d  x ^ 2 + dd    ;";

    printf("%d\n", erase_blanks_two(str));

    

    // printf("%d\n", erase_blanks_two(str));

    // char *p = strtok(str, " ");
    // printf("%s\n", p);
    // printf("%d\n", p == NULL);
    // p = strtok(NULL, " ");
    // printf("%s\n", p);
    // printf("%d\n", p == NULL);
    // p = strtok(NULL, " ");
    // printf("%s\n", p);
    // printf("%d\n", p == NULL);
    // p = strtok(NULL, " ");
    // printf("%s\n", p);
    // printf("%d\n", p == NULL);
    // p = strtok(NULL, " ");
    printf("%s\n", str);
    printf("문자열 길이: %zd\n", strlen(str));

    return 0;
}