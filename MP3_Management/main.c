#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_tools.h"
#include "library.h"

#define BUFFER_LENGTH 200

void handle_add();
void process_command();


int main(void)
{
    initialize();
    process_command();

}


void process_command()
{
    char command_line[BUFFER_LENGTH];
    char *command, *arg1, *arg2;

    while (1) {
        printf("$ ");

        if (read_line(stdin, command_line, BUFFER_LENGTH) <= 0)     // get a line
            continue;
        
        command = strtok(command_line, " ");
        if (strcmp(command, "add") == 0)
            handle_add();
        
        // else if (strcmp(command, "search") == 0)
        //     handle_search();

        // else if (strcmp(command, "remove") == 0)
        //     handle_remove();

        else if (strcmp(command, "status") == 0)
            status();

        // else if (strcmp(command, "play") == 0)
        //     handle_play();

        // else if (strcmp(command, "save") == 0)
        //     handle_save();

        else if (strcmp(command, "exit") == 0)
            break;

    }
}

void handle_add() {
    char buffer[BUFFER_LENGTH];
    char *artist = NULL, *title = NULL, *filepath = NULL;  // 초기화까지 해두면 좋음

    printf("    Artist: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        artist = strdup(buffer);    // 입력이 없으면 초기화된 대로 NULL로. 아니면 입력된 값 할당됨.  

    printf("    Title: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        title = strdup(buffer);

    printf("    File: ");
    if (read_line(stdin, buffer, BUFFER_LENGTH) > 0)
        filepath = strdup(buffer);
    

    printf("%s %s %s\n", artist, title, filepath);      // TODO 코딩하면서 printf로 중간중간 컴파일 확인하는게 좋은 습관!

    /* add to the music library */
    add_song(artist, title, filepath);
}