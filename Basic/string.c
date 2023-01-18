#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 100

int main(void)
{
    // 배열의 type은 배열의 각 칸에 저장되는 데이터의 타입!
    // 포인터 주소를 저장해야 하니 *
    // 포인터에서 char 데이터 타입을 저장해야 하니 char
    char* words[100];
    int n = 0;
    char buffer[BUFFER_SIZE];

    for (int i = 0; i < 4; i++)
    {
        scanf("%s", buffer);
        char* temp = malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(temp, buffer);
        words[n] = temp;
        n++;
    }
 
    printf("\n");
    for (int i = 0; i < 4; i++)
    {
        printf("%s\n", words[i]);
    }

    return 0;
}