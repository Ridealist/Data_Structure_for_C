#include "string_tools.h"   // 자신의 header 파일을 include 한다


int read_line(FILE *fp, char str[], int size) {
    int ch, i = 0;
    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (i < size - 1)
            str[i++] = ch;
    }
    str[i] = '\0';
    return i;
}