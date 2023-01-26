#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX_POLYS 100
#define BUFFER_LENGTH 100

typedef struct term {
    int coef;  // 계수
    int expo;  // 지수(차수)
    struct term *next;
} Term;

typedef struct polynomial {
    char name;
    Term *first;  // 다항식을 구성하는 "첫번째 항"의 주소
    int size;     // 항의 갯수
} Polynomial;

// -> 하나의 다항식 : Polynomial

// 위와 같은 다항식이 여러개 -> Polynomail 포인터 타입 배열
Polynomial *polys[MAX_POLYS];
int n = 0;  // 저장된 다항식 갯수

Term *create_term_instance() {
    Term *t = (Term *)malloc(sizeof(Term));
    t->coef = 0;
    t->expo = 0;
    return t;
}

Polynomial *create_polinomial_instance(char name) {
    Polynomial *p = (Polynomial *)malloc(sizeof(Polynomial));
    p->name = name;
    p->first = NULL;
    p->size = 0;
    return p;
}

void add_term(int c, int e, Polynomial *poly) {
    // 계수가 0인 경우 추가가 의미 없으므로 예외처리
    if (c == 0) {
        return;
    }
    Term *p = poly->first, *q = NULL;
    while (p != NULL && p->expo > e) {
        q = p;
        p = p->next;
    }

    // p가 NULL인 경우에 대한 예외처리도 필요(하나도 항이 없는 경우)
    if (p != NULL && p->expo == e) {    // <- 동일 차수의 항이 존재하는 경우
        p->coef += c;
        if (p->coef == 0) {     // <- 더했더니 계수가 0이 되는 경우
            if (q == NULL) {
                //  (p가 최고차항인 경우)
                poly->first = p->next;      // <- q가 NULL이면 첫번째 노드를 삭제한다
            }
            else
                q->next = p->next;      // <- 그렇지 않다면, q의 다음 노드를 삭제한다
            poly->size--;   // <- poly의 크기를 줄인다
            free(p);    // <- 불필요해진 노드를 free 처리
        }
        return;
    }

    Term *term = create_term_instance();
    // 동적메모리이므로 값(매개 변수 인자) 새롭게 할당 필요!
    term->coef = c;
    term->expo = e;

    if (q == NULL) {        // <- 맨 앞에 삽입하는 경우
        poly->first = term;
        term->next = p;
    }
    else {      // <- q의 뒤, p의 앞에 삽입(p는 NULL일 수도 있지만, 조건 분기 불필요)
        term->next = p;
        q->next = term;
    }
    poly->size++;
}

int eval(Polynomial *poly, int x) {     // <- 다항식의 값을 계산하는 함수
    int sum = 0;
    Term *p = poly->first;
    while (p != NULL)
    {
        sum += p->coef * pow(x, p->expo);   // <- 각각의 항의 값을 계산하여 더함
        // sum += eval_term(p, x);
        p = p->next;
    }
    return (int)sum;
}

int eval_term(Term *term, int x) {       // <- 하나의 항의 값을 계산하는 함수
    int result = term->coef;
    for (int i = 0; i < term->expo; i++) {
        result *= x;
    }
    return result;
}

// TODO 함수 개선해보기!
void print_term(Term *t) {      // TODO <- 이 함수는 완벽하지 않다. (ex. 3x^2 + -5x^1 + 3x^0)
    printf("%dx^%d", t->coef, t->expo);
}

void print_poly(Polynomial *p) {
    printf("%c = ", p->name);
    Term *t = p->first;
    while (t != NULL) {
        print_term(t);
        printf(" + ");
        t = t->next;
    }
    printf("\n"); 
}


int read_line(FILE *fp, char *command, int limit) {
    if (fp == NULL) {
        printf("File Not Exists.\n");
        return 0;
    }

    int i, ch = 0;
    while((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (i == limit - 1)
            break;
        command[i++] = ch;
    }
    command[i] = '\0';
    return i;
}

Polynomial *search_poly(char name);

void handle_print(char name);

void handle_calc(char name, char *x_str);

void handle_definition(char *expression);


void process_command() {
    char command_line[BUFFER_LENGTH];
    char copied[BUFFER_LENGTH];
    char *command, *arg1, *arg2;

    while (1)
    {
        printf("$ ");
        if (read_line(stdin, command_line, BUFFER_LENGTH) <= 0)
            continue;
        strcpy(copied, command_line);   // <- 입력 라인을 복사해 둔다
        command = strtok(command_line, " ");
        if (strcmp(command, "print")) {
            arg1 = strtok(NULL, " ");
            if (arg1 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            handle_print(arg1[0]);  // <- 함수는 한 문자(char)이므로, 문자열이 아닌 char 타입을 넘겨주도록
        }  
        if (strcmp(command, "calc") == 0) {
            arg1 = strtok(NULL, " ");
            arg2 = strtok(NULL, " ");
            if (arg1 == NULL || arg2 == NULL) {
                printf("Invalid arguments.\n");
                continue;
            }
            handle_calc(arg1[0], arg2);
        }
        if (strcmp(command, "exit") == 0) {
            break;
        }
        else
            handle_definition(copied);   // <- 다항식을 입력받아 정의하는 일 (1. 다항식 정의 / 2. 두 다항식 덧셈, 뺄셈)
    }
}


Polynomial *search_poly(char name) {
    for (int i = 0; i < n; i++) {
        if (polys[i]->name == name)
            return polys[i];
    }
    return NULL;
}

void handle_print(char name) {
    Polynomial *p = search_poly(name);
    if (p == NULL) {
        printf("Error: '%c' named polynomial no exists.\n", name);
        return;
    }
    print_poly(p);
}

void handle_calc(char name, char *x_str) {
    int result;

    Polynomial *p = search_poly(name);
    if (p == NULL) {
        printf("Error: '%c' named polynomial no exists.\n", name);
        return;
    }
    int n = atoi(x_str);
    result = eval(p, n);
    printf("%d\n", result);
}

// <- 문자배열에서 모든 공백 문자을 제거하여 "압축"한다. 문자열 끝에 '\0'를 추가해준다.
// TODO  항상 c언어에선 문자열 마지막 끝에 '\0' null 문자를 삽입해서 문자열의 끝임을 표시해야 함!
void erase_blanks(char *expression) {
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
}

int parse_and_add_term(char *expr, int begin, int end, Polynomial *p_poly) {
    
    int i = begin;
    int sign_coef = 1, coef = 0, expo = 1;
    if (expr[i] == '+')     // <- +, - 기호로 부터 계수의 부호를 결정 / 첫 번째 항의 경우 +,- 부호가 없을 수도 있다
        i++;
    else if (expr[i] == '-') {
        sign_coef = -1;
        i++;
    }
    while (i < end && expr[i] >= '0' && expr[i] <= '9') {   // <- digit들을 읽어서 계수의 절댓값을 계산
        coef = coef * 10 + (int)(expr[i] - '0');            // <- digit들이 하나도 없을 수도 있다(계수가 1 or -1인 경우)
                            // TODO 아스키코드를 그대로 계산하면 안되고 '0'빼서 type casting 해야 함...
        i++;
    }
    if (coef == 0)      // <- TODO coef가 0인 경우(위 while에서 바로 빠져 나온 경우) 계수는 0이 아니라 1 혹은 -1이다...
        coef = sign_coef;
    else
        coef *= sign_coef;

    if (i >= end) {     // <- 계수 부분만 있는 항(항을 구성하는 문자가 없는 경우). 상수항!
        add_term(coef, 0, p_poly);
        return 1;
    }
    
    if (expr[i] != 'x')     // <- 계수 다음에 x가 아닌 다른 문자가 등장해서는 안된다. 오류.
        return 0;
    i++;
    if (i >= end) {     // <- 계수 다음에 x만 오고 문자열이 끝난다. 1차항!
        add_term(coef, 1, p_poly);
        return;
    }

    if (expr[i] != '^')     // <- x다음에 ^가 아닌 다른 문자가 등장하면 안된다. 오류!
        return 0;
    i++;

    expo = 0;
    while (i < end && expr[i] >= '0' && expr[i] <= '9') {   // <- 지수 부분 읽기
        expo = expo * 10 + (int)(expr[i] - '0');
        i++;
    }
    add_term(coef, expo, p_poly);
    return 1;
}


Polynomial *create_by_parse_polynimail(char name, char *body) {

    Polynomial *p = create_polinomial_instance(name);

    int i = 0, begin_term = 0;
    while (i < strlen(body)) {
        if (body[i] == '+' || body[i] == '-')  // <- +,- 기호를 읽음(항의 맨 앞 기호)
            i++;
        while (i < strlen(body) && body[i] != '+' && body[i] != '-') // <- 하나의 항이 끝날때까지 전진
            i++;
        int result = parse_and_add_term(body, begin_term, i, p);    // <- body[begin_term, i) 까지가 하나의 항.
        if (result == 0) {      // <- 잘못된 항일 경우 0을 반환
            destrop_polynomial(p);      //<- 불필요해진 다항식 free 해주는 역할
            return NULL;
        begin_term = i;     // <- 다음 항의 시작 위치가 i가 된다 
        }
    }
    return p;
}

void destroy_polynomial(Polynomial *p) {
    if (p == NULL) {
        return;
    }
    Term *t = p->first, *tmp;
    while (t != NULL) {
        tmp = t;
        t = tmp->next;  // <- TODO 다항식에 속한 모든 항들을 free시켜야 한다!!! 하나씩 찾아가면서...
        free(tmp);

        // free(t)
        // t = tmp->next; -> 불가능! t가 free되는 순간 tmp도 free되어 메모리 접근 불가해짐!
    }
    free(p);
}

Polynomial *create_by_add_two_polynomials(char name, char f, char g) {
    Polynomial *p1 = search_poly(f);
    Polynomial *p2 = search_poly(g);
    if (p1 == NULL || p2 == NULL)
        return NULL;

    Polynomial *new_p = create_polinomial_instance(name);

    Term *tp1 = p1->first;
    while (tp1 != NULL) {
        add_term(tp1->coef, tp1->expo, new_p);
        tp1 = tp1->next;
    }
    Term *tp2 = p2->first;
    while (tp2 != NULL) {
        add_term(tp2->coef, tp2->expo, new_p);
        tp2 = tp2->next;
    }
    destroy_polynomial(p1);
    destroy_polynomial(p2);
    return new_p;
}

void insert_polynomial(Polynomial *ptr_poly) {
    for (int i = 0; i < n; i++) {
        if (polys[i]->name == ptr_poly->name) {
            destrop_polynomial(polys[i]);   // <- 다항식을 덮어쓸 경우 기존 다항식 객체를 free시켜줘야 한다
            polys[i] = ptr_poly;
            return;
        }
    }
    polys[n++] = ptr_poly;  // <- 맨 마지막에 새로운 다항식 객체를 추가함
}


void handle_definition(char *expression) {

    // TODO 공백 문자 규칙이 있지 않은 이상, 모든 공백을 제거하고 parsing하는게 편한 경우가 많음!
    erase_blanks(expression);   // <- 모든 공백 문자들을 제거한다.

    // 예외 처리 부분
    char *f_name = strtok(expression, "=");
    if (f_name == NULL || strlen(f_name) != 1) {
        printf("Unsupported command.\n");
        return;
    }
    char *exp_body = strtok(NULL, "=");
    if (exp_body == NULL || strlen(exp_body) <= 0) {
        printf("Invalid expression format.\n");
        return;
    }

    // 1. 기존 두 다항식의 합
    if (exp_body[0] >= 'a' && exp_body[0] <= 'z' && exp_body[0] != 'x') {
        char *former = strtok(exp_body, "+");
        if (former == NULL || strlen(former) != 1) {
            printf("Invalid expression format.\n");
            return;
        }
        char *later = strtok(NULL, "+");
        if (later == NULL || strlen(later) != 1) {
            printf("Invalid expression format.\n");
            return;
        }
        // TODO 새로 정의 필요!
        Polynomial *p = create_by_add_two_polynomials(f_name[0], former[0], later[0]);
        if (p == NULL) {
            printf("Invalid expression foramt.\n");
            return;
        }
        // TODO 새로 정의 필요!
        insert_polynomial(p);
    }

    // 2. 새로 정의하는 경우
    else {
        // TODO 새로 정의 필요!
        Polynomial *p = create_by_parse_polynomial(f_name[0], exp_body);
        if (p == NULL) {
            printf("Invalid expression foramt.\n");
            return;
        }
        insert_polynomial(p);
    }
}


int main() {

    process_command();

    return 0;
}