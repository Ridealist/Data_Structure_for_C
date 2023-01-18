#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER_SIZE 20

int read_line( char str[], int n );

int main()
{
    char input[BUFFER_SIZE];
    int length;

    while (1)
    // while (true)
    {
        printf("$ "); 
        //scanf("%s", input);
        // 줄바꿈 문자까지(new line character) 읽어서 저장해버림.
        // fgets(input, BUFFER_SIZE, stdin);
        int len = read_line(input, BUFFER_SIZE); 
        // input[strlen(input) - 1] = '\0';
        // length = strlen(input);

        printf("%s:%d\n", input, len);
    }
    
    return 0;
}

int read_line( char str[], int limit ) {
    int ch, i = 0;

    // getchar() -> 정수 반환함
    while ((ch = getchar()) != '\n')
    {   
        if (i < limit - 1)
        {
            if (ch == ' ')
            {
                if (i == 0)
                    continue;
                else
                {
                    if (str[i - 1] == ' ')
                        continue;
                }
            }
            str[i++] = ch;
        }
    }
    if (i > 0 && str[i - 1] == ' ')
        str[i - 1] = '\0';
        return i - 1;

    str[i] = '\0';
    return i;
}

void readline( char array[], int limit ) {
    char input;
    int i = 0;
    
    while ((input = getchar()) != '\n')
    {
        if (i < limit - 1)
        {
            array[i] = input;
            i++;
        }
        // else
        //     break;
    }

    array[i] = '\0';
}