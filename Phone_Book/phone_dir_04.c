#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CAPACITY 100
#define BUFFER_LENGTH 100

typedef struct {
    char *name;
    char *number;
    char *email;
    char *group;
} Person;

Person directory[CAPACITY];

int n = 0;

void read(char filename[]);
void save(char filename[]);
void handle_add(char name[]);
void add(char name[], char number[], char email[], char group[]);
void find(char name[]);
void status();
void delete(char name[]);


int read_line(FILE *fp, char str[], int n) {
    int ch, i = 0;

    while ((ch = fgetc(fp)) != '\n' && ch != EOF ) {
        if (i < n)
            str[i++] = ch;
    }
    str[i] = '\0';

    // printf("%s\n", str);
    return i;
}

int compose_name(char str[], int limit) {
    char *ptr;
    int length = 0;
    // TODO 한 프로젝트 내에서 strtok 함수는 연동됨!
    ptr = strtok(NULL, " ");
    if (ptr == NULL)
        return 0;

    strcpy(str, ptr);
    length += strlen(ptr);

    while ((ptr = strtok(NULL, " ")) != NULL) {
        // TODO buffer overflow 방지에 대해 항상 고민!ㅜ 
        if (length + strlen(ptr) + 1 < limit) {
            // str[length++] = ' ';
            // str[length] = '\0';
            // strcat은 두 단어가 '\0' null character로 끝난다는 가정에서만 정상 동작함! 
            strcat(str, " ");
            strcat(str, ptr);
            length += strlen(ptr) + 1;
        }
    }
    return length;
}

int main() {
    char command_line[BUFFER_LENGTH];
    char *command, *arg;
    char name_str[BUFFER_LENGTH];

    while (1) {
        printf("$ ");
        if (read_line(stdin, command_line, BUFFER_LENGTH) <= 0)
            continue;
        
        // TODO 파일 형식 validate 추가
        command = strtok(command_line, " ");

        if (strcmp(command, "read") == 0) {
            arg = strtok(NULL, " ");
            if (arg == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            if (strstr(arg, "txt") == NULL) {
                printf("Invalid File Format.\n");
                continue;
            }
            read(arg);
        }
        else if (strcmp(command, "add") == 0) {
            if (compose_name(name_str, BUFFER_LENGTH) <= 0) {
                printf("Name required.\n");
                continue;
            }
            handle_add(name_str); 
        }

        else if (strcmp(command, "find") == 0) {
            if (compose_name(name_str, BUFFER_LENGTH) <= 0) {
                printf("Name required.\n");
                continue;
            }
            find(name_str);
        }

        else if (strcmp(command, "status") == 0) {
            status();
        }

        else if (strcmp(command, "delete") == 0) {
            if (compose_name(name_str, BUFFER_LENGTH) <= 0) {
                printf("Invalid arguments.\n");
                continue;
            }
            delete(name_str);
        }
        else if (strcmp(command, "save") == 0)
        {
            arg = strtok(NULL, " ");
            if (arg == NULL || strcmp(arg, "as") != 0) {
                printf("Invalid command format.\n");
                continue;
            }
            arg = strtok(NULL, " ");
            if (arg == NULL) {
                printf("File name required.\n");
                continue;
            }
            if (strstr(arg, "txt") == NULL) {
                printf("Invalid File Format.\n");
                continue;
            }
            save(arg);
        }
        else if (strcmp(command, "exit") == 0)
            break;
        else
            printf("Invalid command format.\n");
            continue;
    }
} 


void read(char filename[]) {
    char buffer[BUFFER_LENGTH];
    char *name, *number, *email, *group;
    char delim[] = "#";

    FILE * fp = fopen(filename, "r");
    // 파일을 열지 못했을 때 예외 처리!
    if (fp == NULL) {
        printf("Open failed.\n");
        return;
    }

    while (1) {
        if (read_line(fp, buffer, BUFFER_LENGTH) <= 0)
            break;
        // TODO strdup()를 사용할 필요가 없다!
        // strtok은 원본 문자열을 메모리에서 제거하지 않음.
        // '#' -> '\0'으로 바꾼 각각의 포인터를 제공하므로 각가의 포인터가 별개의 단어로 취급됨.
        name = strtok(buffer, delim);
        number = strtok(NULL, delim);
        email = strtok(NULL, delim);
        group = strtok(NULL, delim);
        add(name, number, email, group);
    }
    fclose(fp);
}

void save(char filename[]) {
    int i;
    FILE * fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Open failed.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s#", directory[i].name);
        fprintf(fp, "%s#", directory[i].number);
        fprintf(fp, "%s#", directory[i].email);
        // 마지막에는 줄바꿈 문자까지 추가
        fprintf(fp, "%s#\n", directory[i].group);
    }
    fclose(fp);
}

int search(char name[]) {
    int index;
    for (int index = 0; index < n; index++) {
        if (strcmp(name, directory[index].name) == 0) {
            return index;
        }
    }
    return -1;
}

void print_person(Person p) {
    printf("%s:\n", p.name);
    printf("    Phone: %s\n", p.number);
    printf("    Email: %s\n", p.email);
    printf("    Group: %s\n", p.group);
}

void find(char name[]) {
    int index = search(name);
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return;
    }
    print_person(directory[index]);
}

void status() {
    for (int i = 0; i < n; i++) {
        print_person(directory[i]);
    }
    printf("Total %d persons.\n", n);
}


void add(char name[], char number[], char email[], char group[]) {

    int i = n - 1;
    while (i >= 0 && strcmp(directory[i].name, name) > 0) {
        // TODO
        // names[i] = names[i + 1] -> 오류! 오른쪽 값을 왼쪽 값으로 할당하는 것. (없는 값을 할당하는 꼴)
        // 기존 i번 째 원소를 i+1번 째 원소로 할당하는 것. 
        directory[i + 1] = directory[i];
        i--; 
    }

    directory[i + 1].name = strdup(name);
    // 입력값이 없는 경우 " " 하나의 공백 문자로 입력됨에 유의
    directory[i + 1].number = strdup(number);
    directory[i + 1].email = strdup(email);
    directory[i + 1].group = strdup(group);
    n++;
    
    printf("%s was added successfully.\n", name);
}

void handle_add(char name[]) {
    char number[BUFFER_LENGTH], email[BUFFER_LENGTH], group[BUFFER_LENGTH];
    char empty[] = " ";

    printf("    Phone: ");
    read_line(stdin, number, BUFFER_LENGTH);
    // if (strlen(number) <= 0)
    //     strcpy(number, empty);

    printf("    Email: ");
    read_line(stdin, email, BUFFER_LENGTH);
    // if (strlen(email) <= 0)
    //     *email = *empty;

    printf("    Group: ");
    read_line(stdin, group, BUFFER_LENGTH);
    // if (strlen(group) <= 0)
    //     *group = *empty;

    // add(name, number, email, group);
    // TODO 삼항연산자 사용 고려
    add(name, (char *)(strlen(number) > 0 ? number : empty),
              (char *)(strlen(email) > 0 ? email : empty),
              (char *)(strlen(group) > 0 ? group : empty));
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
        directory[j] = directory[j + 1];
    }
    n--;

    printf("%s was deleted successfully.\n", name);  
}