#include <ctype.h>  // whitespace 문자인지 검사하는 함수 제공
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define BUFFER_SIZE 80

int read_line_with_compression( char str[], int n );

int main()
{
    char input[BUFFER_SIZE];

    while (1)
    {
        printf("$ "); 
        int len = read_line_with_compression(input, BUFFER_SIZE);
        printf("%s:%d\n", input, len);
    }
    return 0;
}

int read_line_with_compression( char compressed[], int limit )
{
    int ch, i = 0;
    while ((ch = getchar()) != '\n')
    {
        if (i < limit - 1)
        {
            if (!isspace(ch) || i > 0 && !isspace(compressed[i - 1]))
                compressed[i++] = ch;
        }
    
    if (i > 0 && isspace(compressed[i - 1]))
        i--;
    
    compressed[i] = '\0';
    
    return i;
}