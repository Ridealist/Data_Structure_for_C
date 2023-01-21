#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define INIT_CAPACITY 3
#define BUFFER_SIZE 50

char** names;
char** numbers;

int capacity = INIT_CAPACITY; // size of array (names & numbers)
int n = 0;

char delim[] = " ";

void init_directory();
void process_command();
void reallocate();

void add(char name[], char number[]);
void find(char name[]);
void status();
void delete(char name[]);

void read(char filename[]);
void save(char filename[]);

// char** tokenize(char command[]);
// bool validate(char command[]);

int main()
{
    init_directory();   // 배열 names와 numbers 생성
    process_command();

    return 0;
}

void init_directory() {
    // malloc은 void* 를 반환하므로 타입 캐스팅을 해주는게 좋음
    names = (char **)malloc(sizeof(char *) * INIT_CAPACITY);
    numbers = (char **)malloc(sizeof(char *) * INIT_CAPACITY);
}

void reallocate() {
    int i;
    
    capacity *= 2;
    char** tmp1 = (char **)malloc(sizeof(char *) * capacity);
    char** tmp2 = (char **)malloc(sizeof(char *) * capacity);

    for (i = 0; i < n; i++) {
        tmp1[i] = names[i];
        tmp2[i] = numbers[i];
    }

    free(names);
    free(numbers);

    names = tmp1;
    numbers = tmp2;

    printf("Memory was reallocated successfully.\nCurrent Capcity : %d\n", capacity);
    // TODO realloc 함수 사용할 수 없는지 고민해보기!
/*     char* backup_buffer_names = malloc(sizeof(char *) * capacity);
    char* backup_buffer_numbers = malloc(sizeof(char *) * capacity);
    names = (char **) realloc(names, capacity);
    numbers = (char **) realloc(numbers, capacity);
    if (names == NULL || numbers == NULL) {
        printf("Memory allocation is failed");
        names = backup_buffer_names;
        numbers = backup_buffer_numbers;
        return;
    }
    free(backup_buffer_names);
    free(backup_buffer_numbers); */
}

int read_line(char str[], int limit) {
    int ch, i = 0;

    while ((ch = getchar()) != '\n') {
        if (i < limit - 1)
            str[i++] = ch;
    }
    str[i] = '\0';
     
    return i;
}

void process_command() {
    char command_line[BUFFER_SIZE];  // 한 라인을 통채로 읽어오기 위한 버퍼
    char *command, *arg1, *arg2;

    while (1) {
        printf("$ ");
    
        if (read_line(command_line, BUFFER_SIZE) <= 0)
            continue;
        
        command = strtok(command_line, delim);
        if (command == NULL)
            continue;
        else if (strcmp(command, "read") == 0) {
            arg1 = strtok(NULL, delim);
            if (arg1 == NULL) {
                printf("File name required.\n");
                continue;
            }
            read(arg1);
        }
        else if (strcmp(command, "add") == 0) {
            arg1 = strtok(NULL, delim);
            arg2 = strtok(NULL, delim);
            if (arg1 == NULL || arg2 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            add(arg1, arg2);
        }
        else if (strcmp(command, "find") == 0) {
            arg1 = strtok(command, delim);
            if (arg1 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            find(arg1);
        }
        else if (strcmp(command, "delete") == 0) {
            arg1 = strtok(command, delim);
            if (arg1 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            delete(arg1);
        }
        else if (strcmp(command, "status") == 0)
            status();
        else if (strcmp(command, "save") == 0)
        {
            arg1 = strtok(NULL, delim);
            if (arg1 == NULL || strcmp(arg1, "as") != 0) {
                printf("Invalid command format.\n");
                continue;
            }
            arg2 = strtok(NULL, delim);
            if (arg2 == NULL) {
                printf("File name required.\n");
                continue;
            }
            save(arg2);
        }
        else if (strcmp(command, "exit") == 0)
            break;
        else
            printf("Invalid command format.\n");
            continue;
    }
}


// bool validate(char command[]) {
//     char** tokens = tokenize(command);
//     if (strcmp(tokens[0], "read") == 0) {
//         char* ptr = strtok(tokens[1], ".");
//         if (ptr == NULL) {
//             printf("File name required.\n");
//             return false;
//         }
//         ptr = strtok(NULL, ".");
//         if (strcmp(ptr, "txt") != 0) {
//             printf("Invalid File Format.\n");
//             return false;
//         }
//     }
//     return true;
// }

// char** tokenize(char command[]) {
//     char** tokens = malloc(sizeof(char*) * BUFFER_SIZE);
//     for (int i = 0; i < BUFFER_SIZE; i++) {
//         tokens[i] = malloc(sizeof(char) * BUFFER_SIZE);
//     }
//     char* ptr = strtok(command, " ");
    
//     int i = 0;
//     while (ptr != NULL) {
//         tokens[i] = strdup(ptr);
//         ptr = strtok(NULL, " ");
//         i++;
//     }
//     return tokens;
// }


void read(char filename[]) {
    // TODO
    // 함수에서도 "필요한 데이터(자료 구조)"가 무엇이 있을지 먼저 생각하고 정의하기
    // 포인터 변수 자체에는 데이터를 저장할 수 없다!
    // 데이터를 저장하려면 동적 메모리를 할당해야 한다
    // or 애초에 char name[BUFFER_SIZE] 처럼 "크기를 가진 배열"을 선언해야 한다.
    char *name = malloc(sizeof(char) * BUFFER_SIZE);
    char *number = malloc(sizeof(char) * BUFFER_SIZE);

    FILE * in_fp = fopen(filename, "r");
    // 파일을 열지 못했을 때 예외 처리!
    if (in_fp == NULL) {
        printf("Open failed.\n");
        return;
    }
    /* 
    아래 처럼 fscanf를 2번 호출하여 각 단어 별로 처리하도록도 가능!
    while (fscanf(in_fp, "%s", name) != EOF) {
        fscanf(in_fp, "%s", number);
    }
    */
    while (fscanf(in_fp, "%s %s", name, number) != EOF)
    {
        // 기존에 정의한 add() 함수를 반복 실행하면 됨!
        add(name, number);
    }
    fclose(in_fp);

    free(name);
    free(number);
}

void save(char filename[]) {

    FILE * out_fp = fopen(filename, "w");
    // 혹시 모를 예외 처리 
    if (out_fp == NULL) {
        printf("Open failed.\n");
        return;
    }
    for (int i = 0; i < n; i++) {
        fprintf(out_fp, "%s %s\n", names[i], numbers[i]);
    }
    fclose(out_fp);
}

void add(char name[], char number[]) {
    if (n >= capacity) {
        reallocate();
    }

    int i = n - 1;
    while (i >= 0 && strcmp(names[i], name) > 0) {
        // names[i] = names[i + 1] -> 오류! 오른쪽 값을 왼쪽 값으로 할당하는 것. (없는 값을 할당하는 꼴)
        // 기존 i번 째 원소를 i+1번 째 원소로 할당하는 것. 
        names[i + 1] = names[i];
        numbers[i + 1] = numbers[i];
        i--;
    }
    names[i + 1] = strdup(name);
    numbers[i + 1] = strdup(number);

    n++;
    printf("%s was added successfully.\n", name);
}

int search(char name[]) {
    int index;
    for (int index = 0; index < n; index++) {
        if (strcmp(name, names[index]) == 0) {
            return index;
        }
    }
    return -1;
}

void find(char name[]) {
    int index = search(name);
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return;
    }
    printf("%s\n", numbers[index]);
}

void status() {
    for (int i = 0; i < n; i++) {
        printf("%s %s\n", names[i], numbers[i]);
    }
    printf("Total %d persons.\n", n);
}

void delete(char name[]) {
    // 데이터 삭제 알고리즘 수정
    int index = search(name);  // returns -1 if not exists
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return; 
    }

    int j = index;
    for (; j < n - 1; j++) {
        names[j] = names[j + 1];  // 오른쪽의 값을 왼쪽에 할당한다. = j+1번째 사람을 j번째로 이동시킨다.
        numbers[j] = numbers[j + 1];
    }    
}