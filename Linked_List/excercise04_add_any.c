#include <stdlib.h>

struct node {
    char *data;
    struct node *next;
};

typedef struct node Node;

Node *head = NULL;

int main(void) {

    Node *prev;

    Node *tmp = (Node *)malloc(sizeof(Node));
    tmp->data = "Data";
    tmp->next = prev->next;
    prev->next = tmp;

}

// 1. 삽입

void add_first(char *item) {
    Node *p = (Node *)malloc(sizeof(Node));
    p->data = item;
    p->next = head;
    head = p;
}


// 삽입에 성공하면 1, 아니면 0을 반환한다
int add_after(Node *prev, char *item) {

    if (prev == NULL) {
        return 0;
    }

    Node *tmp = (Node *)malloc(sizeof(Node));
    tmp->data = item;
    tmp->next = prev->next;
    prev->next = tmp;

    return 1;
}

// 2. 삭제
// head는 전역변수일 때
// 삭제되 노드 주소 반환
Node * remove_first() {
    if (head == NULL)
        return NULL;

    Node *tmp = head;
    head = head->next;
    return tmp;
    // 삭제된 노드 메모리를 추후 free 시켜주어야 함
    // 따라서 노드의 주소를 반환!
}


// 삭제된 노드 주소 반환
Node * remove_after(Node *prev) {
    Node *tmp = prev->next;
    if (tmp == NULL)
        return NULL;
    
    prev->next = tmp->next;
    return tmp;
}

// 3. 순회
// 연결리스트 순회하기
Node *find(char *word) {
    Node *p = head;

    while (p != NULL)
    {
        if (strcmp(p->data, word) == 0)
            return p;
        p = p->next;
    }
    return NULL;
}


// 4. index로 연결리스트 조작하기
// index에 따라 순회하기
Node *get_node(int index) {
    if (index < 0)
        return NULL;
    Node *p = head;
    for (int i = 0; i < index && p != NULL; i++) {
        p = p->next;
    }
    return p;
}

// index위치에 새로운 노드 만들어 삽입하기
int add(int index, char *item) {
    if (index < 0)
        return 0;
    
    // 기존에 만든 add_first / add_after를 사용하려면 아래 Node 만드는 작업은 불필요!
    Node *tmp = (Node *)malloc(sizeof(Node));
    tmp->data = item;
    tmp->next = NULL;

    // 사실상 아래 식과 같은 것
/*     Node *p = head;
    // index 하나 전 노드에 삽입해야 함 (index번 노드 다음에 삽입하면, 사실상 index + 1 노드가 되는 것)
    for (int i = 0; i < index - 1 && p != NULL; i++) {
        p = p->next;
    }
    if (p == NULL)
        return 0;
    if (index = 0) {
        tmp->next = head(p);
        head(p) = tmp;
        return 1;
    }
    else {
        tmp->next = p->next;
        p->next = tmp;
        return 1;
    } */

    // 연결리스트에 노드를 추가할 때
    // 1. index = 0 번째인 경우
    if (index == 0) {
        add_first(item); // -> Node 자체적으로 만들기 때문에 중복
        return 1;
    }

    // 2. index != 0 번째인 경우 구분해서 처리해야 함!
    Node *prev = get_node(index - 1);
    if (prev != NULL)
        add_after(prev, item);
    else {
        return 0;
    }
}

// index번째 노드 삭제
Node *remove(int index) {
    if (index < 0)
        return NULL;
    
    // 1. index 가 0인 경우
    if (index == 0)
        return remove_first();
    
    // 2. index가 0이 아닌 경우 나눠서 생각!
    Node *prev = get_node(index - 1);
    if (prev == NULL)
        return NULL;
    return remove_after(prev);
}

// 아이템에 해당하는 노드 삭제
Node *remove(char *item) {
    Node *p = head;
    Node *q = NULL;
    while (p != NULL && strcmp(p->data, item) != 0) {
        q = p;  // 직전노드를 계속 q에 할당함
        p = p->next;  // p가 다음 노드로 전진
    }
    if (p == NULL)
        return NULL;
    if (q == NULL)
        return remove_first();
    return remove_after(q);
}

// 5. Ordered_List
// 순서대로 저장된 리스트에 노드 추가
// 성공하면 1 실패하면 0 반환
int add_to_ordered_list(char *item) {
    Node *p = head;
    Node *q = NULL;
    while (p != NULL && strcmp(p->data, item) < 0) {
        q = p;
        p = p->next;
    }
    // if (p == NULL)
    //     return NULL; -> 필요 없음! head가 NULL이라는건 결국 "empty list"에 첫번째 원소 삽입하는 것
    if (q == NULL)
        add_first(item);
        return 1;
        // return add_first(item) -> 잘못된 표현! void 함수이므로 return 반환값 뒤에 함수가 오면 안됨
    return add_after(q, item);
}
