#include <stdio.h>
#include <string.h>

#define CAPACITY 100
#define BUFFER_SIZE 20

char* names[CAPACITY];
char* numbers[CAPACITY];
int n = 0;

void add();
void find();
void status();
void delete();

void read();
void save();
void order();


int main()
{
    char command[BUFFER_SIZE];
    while (1)
    {
        order();
        printf("$ ");
        scanf("%s", command);
        if (strcmp(command, "add") == 0)
            add();
        else if (strcmp(command, "find") == 0)
            find();
        else if (strcmp(command, "status") == 0)
            status();
        else if (strcmp(command, "delete") == 0)
            delete();
        else if (strcmp(command, "read") == 0)
            read();
        else if (strcmp(command, "save") == 0)
            save();
        else if (strcmp(command, "exit") == 0)
            break;
    }
    return 0;
}

void read() {
    // 함수에서도 "필요한 데이터(자료 구조)"가 무엇이 있을지 먼저 생각하고 정의하기
    char filename[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    char number[BUFFER_SIZE];

    scanf("%s", filename);

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
        names[n] = strdup(name);
        numbers[n] = strdup(number);
        n++;
    }
    fclose(in_fp);
}

void save() {
    char buf[BUFFER_SIZE];
    char filename[BUFFER_SIZE];
    /*
    다음처럼 처리 가증
    scanf("%s", buf);
    scanf("%s", filename); 
    */
    scanf("%s %s", buf, filename);

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

void order() {
    for (int i = 0; i <= n - 2; i++) {
        for (int j = i + 1; j <= n - 1; j++) {
            if (strcmp(names[i], names[j]) > 0) {
                char* temp_name = names[i];
                char* temp_number = numbers[i];
                names[i] = names[j];
                numbers[i] = numbers[j];
                names[j] = temp_name;
                numbers[j] = temp_number;
            }
        }
    }
}

void add() {
    char name[BUFFER_SIZE];
    char number[BUFFER_SIZE];
    scanf("%s %s", name, number);

    // 데이터 추가 알고리즘 변경
    /*
    for (int i = n - 1; i >= 0; i--) {
        if (strcmp(names[i], name) > 0) {
            names[i + 1] = names[i];
            numbers[i + 1] = numbers[i];
            if (i == 0) {
                names[0] = strdup(name);
                numbers[0] = strdup(number);
                break;
            }
        } 
        else if (strcmp(names[i], name) <= 0) {
            names[i + 1] = strdup(name);
            numbers[i + 1] = strdup(number);
            break;
        }
    }
    */

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


void find() {
    char name[BUFFER_SIZE];
    scanf("%s", name);

    for (int i = 0; i < n; i++)
    {
        if (strcmp(names[i], name) == 0)
        {
            printf("%s\n", numbers[i]);
            return;
        }
    }

    /*
    int index = search(name);
    if (index == -1) {
        printf("No person named '%s' exists.\n", name);
        return;
    }
    printf("%s\n", numbers[index]);
    */

    printf("No person named '%s' exists.\n", name);
}

void status() {
    for (int i = 0; i < n; i++) {
        printf("%s %s\n", names[i], numbers[i]);
    }
    printf("Total %d persons.\n", n);
}

void delete() {
    char name[BUFFER_SIZE];
    scanf("%s", name);

    // 데이터 삭제 알고리즘 수정
    for (int i = 0; i < n; i++) {
        if (strcmp(names[i], name) == 0) {
            for (int j = i; j < n - 1; j++) {
                names[j] = names[j + 1];
                numbers[j] = numbers[j + 1];
            }
            n--;
            
            printf("%s was deleted successfully.\n", name);
            return;
        }
    }
    printf("No person named '%s' exists.\n", name); 

    /*
    search함수를 만들어서 활용
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
    */
}