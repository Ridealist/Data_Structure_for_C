#include <stdio.h>
#include <stdlib.h>


typedef struct node {
    char *data;
    struct node *next;
    struct node *prev;
} Node;

Node *head;
Node *tail;
int size = 0;   // -> 일반적으로 노드의 크기를 전역변수로 관리하는게 효율적이다.


// p가 가리키는 노드 앞에 새로운 노드 삽입
void add_before(Node *p) {
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = "June";

    new->next = p;
    new->prev = p->prev;

    p->prev->next = new;
    p->prev = new;

}

// 모든 경우를 한 번에 커버하는 원 큐 함수 만들기!
void add_after(Node *pre, char *item) {
    Node *new = (Node *)malloc(sizeof(Node));
    new->data = item;
    new->prev = NULL;
    new->next = NULL;
    // 1. to empty list
    if (pre == NULL && head == NULL) {
        head = new;
        tail = new;
        return ;
    }
    // 2. at the head -> pre가 NULL이면 연결리스트 맨 앞에 삽입하라는 뜻...  
    else if (pre == NULL) {
        new->next = head;
        head->prev = new;
        head = new;
    }
    // 3. at the tail
    else if (pre == tail) {
        new->prev = tail;
        tail->next = new;
        tail = new;
    }
    // 4. in the middel
    else {
        new->next = pre->next;
        new->prev = pre;

        pre->next->prev = new;
        pre->next = new;
    }
    size++;
}


// p가 가리키는 노드 삭제
void remove(Node *p) {
    // 1. p가 유일한 노드인 경우 -> head, tail 모두 변경
    if (p == head && p == tail) {
        head = NULL;
        tail = NULL;
    }
    // 2. p is head  -> head 변경
    else if (p == head) {
        head->next->prev = NULL;
        head = head->next;
    }
    // 3. p is tail  -> tail 변경
    else if (p == tail) {
        tail->prev->next = NULL;
        tail = tail->prev;
    }
    // 4. etc...  -> 노드의 멤버만 변경
    else {
        p->next->prev = p->prev;  // p 다음 노드의 prev 필드
        p->prev->next = p->next;  // p 이전 노드의 next 필드
    }
    free(p);
    size--;
}

void add_ordered_list(char *item) {
    Node *p = tail;
    while (p != NULL && strcmp(p->data, item) > 0)
        p = p->prev;
    add_after(p, item);
}

// 1. empty list인 경우 -> tail, head 모두 null
// 2. item이 가장 클 경우 (맨 뒤에 와야 될 경우)  -> p = tail인 경우
// 3. item이 가장 작을 경우 (맨 앞에 와야 될 경우) -> p = null로 나옴 && head는 null이 아님
// 4. 가운데 삽입될 경우(일반적)

// -> 모두 커버 가능!