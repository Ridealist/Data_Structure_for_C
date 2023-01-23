#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define INIT_CAPACITY 100
#define BUFFER_LENGTH 100

typedef struct {
    char *name;
    char *number;
    char *email;
    char *group;
} Person;

// TODO 구조체 복사의 비효율성을 극복하기 위해 '구조체 포인터 배열'을 활용
Person ** directory;
int capacity;
int n;

void init() {
    directory = (Person **)malloc(sizeof(Person *) * INIT_CAPACITY);
    capacity = INIT_CAPACITY;
    n = 0;
}

void reallocate() {
    capacity *= 2;
    Person ** new_directory = (Person **)malloc(sizeof(Person *) * capacity);
    // TODO memcpy() 함수로 메모리 복제해도 괜찮은지?
    memcpy(new_directory, directory, sizeof(directory));
/*     for (int i = 0; i < n; i++) {
        new_directory[i] = directory[i];
    } */
    // 구조체를 가리키는 '주소'가 복제되는 것. 현재 한 구조체를 2 배열이 가리키는 상태.
    // free를 하지 않으면 기존 directory 주소를 모르게됨 -> garbage값 발생!
    free(directory);
    directory = new_directory;
}

void load(char filename[]);
void add(char name[], char number[], char email[], char group[]);
void delete();
void find();
void stat();
void save();

int read_line(FILE * fp, char str[], int limit);
int search(char name[]);

int main() {
    char command_line[BUFFER_LENGTH];
    char *command, *arg;

    init();

    while (1) {
        printf("$ ");
        if (read_line(stdin, command_line, BUFFER_LENGTH) <= 0)
            continue;
        
        command = strtok(command_line, " ");

        if (strcmp(command, "read") == 0) {
            arg = strtok(NULL, " ");
            if (arg == NULL) {
                printf("File not exitsts.\n");
                continue;
            }
            if (strstr(arg, "txt") == NULL) {
                printf("Invalid file format.\n");
                continue;
            }
            // printf("%s\n", arg);
            // FILE *fp = fopen(arg, "r");

            // char str[BUFFER_LENGTH];

            // read_line(fp, str, BUFFER_LENGTH);


            // printf("%s\n", str);
            load(arg);
        }

        else if (strcmp(command, "status") == 0) {
            stat();
        }

        else if (strcmp(command, "exit") == 0) {
            break;
        }

        else
            continue;
    }

    free(directory);
    return 0;
}


int read_line(FILE * fp, char str[], int limit) {
    int ch, i = 0;
    
    if (fp == NULL) {
        printf("File Not Exists.\n");
        return 0;
    }

    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (i < limit)
            str[i++] = ch;
    }
    str[i] = '\0';

    return i;
}

int search(char name[]) {
    int i;
    for (i = 0; i < n; i++) {
        if (strcmp(directory[i]->name, name) == 0)
            return i;
    }
    return -1;
}


void print_person(Person *ptr) {
    printf("%s:\n", ptr->name);
    printf("    Phone: %s\n", ptr->number);
    printf("    Email: %s\n", ptr->email);
    printf("    Group: %s\n", ptr->group);
}

void load(char *filename) {
    char buffer[BUFFER_LENGTH];
    char *name, *number, *email, *group;
    char *token;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("File open failed.\n");
        return;
    }

    while (1) {
        if (read_line(fp, buffer, BUFFER_LENGTH) <= 0)
            break;

        // TODO 빈값에 대해서 " " 공백이 아닌 NULL로 값을 채움.
        // TODO segmentation fault 지옥... 해결해보기...!
        name = strtok(buffer, "#");

        token = strtok(NULL, "#");
        if (strcmp(token, " ") == 0)
            number = NULL;
        else
            number = strdup(token);

        token = strtok(NULL, "#");
        if (strcmp(token, " ") == 0)
            email = NULL;
        else
            email = strdup(token);

        token = strtok(NULL, "#");
        if (strcmp(token, " ") == 0)
            group = NULL;
        else
            group = strdup(token);

        if (search(name) != -1) {
            printf("%s was already in the directory.\n", name);
            continue;
        }

        // TODO load() 함수에서 strdup로 복제된 배열을 넘겨주는 방식 변경
        // add()에서 strdup할 필요 없어짐
        add(strdup(name), number, email, group);
    }
    fclose(fp);
}

void stat() {
    for (int i = 0; i < n; i++) {
        print_person(directory[i]);
    }
    printf("Total %d persons.\n", n);
}

void add(char name[], char number[], char email[], char group[]) {
    // TODO 메모리 추가 할당 부분 변경
    if (n >= capacity)
        reallocate();

    int idx = n - 1;
    while (idx >=0 && strcmp(directory[idx]->name, name) > 0) {
        directory[idx + 1] = directory[idx];
        idx--;
    }
    // TODO 동적배열 추가시 "배열 원소" 에도 동적메모리 할당 필요!
    // TODO directory는 구조체 포인터 배열. 구조체를 저장할 배열 필요.
    directory[idx + 1] = (Person *)malloc(sizeof(Person));

    // TODO load()에서 strdup를 했으면, 여기서는 할당만해도 괜찮음.
    directory[idx + 1]->name = strdup(name);
    directory[idx + 1]->number = strdup(number);
    directory[idx + 1]->email = strdup(email);
    directory[idx + 1]->group = strdup(group);

    n++;

    printf("%s was added successfully.\n", name);
}

// TODO Person 객체뿐만 아니라, 각각 원소의 객체도 메모리 해제 필요...!
void release_person(Person *p) {
    free(p->name);
    // TODO strdup() 함수 자체도 malloc() 함수로 메모리를 할당 함. 
    // TODO 결국 strdup()로 생성된 "구조체 멤버 객체"들도 free 해줘야 메모리 누수를 막을 수 있음...!
    if (p->number != NULL) free(p->number);
    if (p->email != NULL) free(p->email);
    if (p->group != NULL) free(p->group);
    free(p);
} 

void delete(char name[]) {
    int idx = search(name);
    if (idx == -1) {
        printf("No person named '%s' exists.\n", name);
    }

    Person *p = directory[idx];
    for (; idx < n - 1; idx++) {
        directory[idx] = directory[idx + 1]; // TODO 포인터 주소가 덮여쓰여지는 것이므로, garbage 발생에 대해 항상 고려!
    }
    n--;
    // TODO 단순히 인덱스만 옮기는 것이 아니고, 할당된 메모리도 해제해주어야 함!!!
    release_person(p);
    printf("'%s' was deleted successfully.\n", name);
}

void find(char name[]) {
    int idx;
    if ((idx = search(name)) == -1) {
        printf("No person named '%s' existed.\n", name);
        return;
    }
    print_person(directory[idx]);
}