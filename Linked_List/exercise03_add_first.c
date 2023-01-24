 #include <stdio.h>
#include <stdlib.h>

struct node {
    char *data;
    struct node *next;
};

typedef struct node Node;

Node *head = NULL;


// 1. head가 전역변수인 경우
void add_first(char *item) {
    Node *tmp = (Node *)malloc(sizeof(Node));   
    tmp->data = item;
    tmp->next = head;
    // head가 지역변수라면, 새로 생성된 노드의 주소를 가리키는 tmp를 분실하게 되버림. (외부의 head는 여전히 이전 노드를 가리키므로)
    head = tmp;      
}

// 2. head가 전역변수가 아닌 경우
// 2.1 포인터 변수 head의 주소 (pointer to pointer to head)를 매개변수로 받는다
void add_first(Node **ptr_head, char *item) {
    Node *tmp = (Node *)malloc(sizeof(Node));
    tmp->data = item;
    tmp->next = *ptr_head;
    *ptr_head = tmp;   
}
// add_first(&head, item_to_store); 형태로 호출


// 2.2 tmp를 반환값으로 설정한다  
Node * add_first(Node *head, char *item) {
    Node *tmp = (Node *)malloc(sizeof(Node));   
    tmp->data = item;
    tmp->next = head;
    return tmp;
}
// head = add_first(head, item_to_store); 형태로 호출

int main() {

    Node *q = (Node *)malloc(sizeof(Node));
    q->data = "Tom";
    q->next = NULL;
    head = q;

    q = (Node *)malloc(sizeof(Node));
    q->data = "Dick";
    q->next = NULL;
    head->next = q;

    q = (Node *)malloc(sizeof(Node));
    q->data = "Harry";
    q->next = NULL;
    head->next->next = q;


    //  데이터 추가
    Node *p = (Node *)malloc(sizeof(Node));   // 1. 새로운 노드 만들기 + 추가할 데이터 저장
    p->data = "Ann";
    p->next = head;   // 2. 새로운 노드의 next 필드가 현재의 노드의 head 노드를 가리키도록 함
    head = p;   // 3. 새로운 노드를 새로운 head 노드로 함     


    q = head;
    while (q != NULL)
    {
        printf("%s\n", q->data);
        q = q->next;
    }


    free(head->next->next->next);
    free(head->next->next);
    free(head->next);
    free(head);


    return 0;

}