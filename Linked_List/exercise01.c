#include <stdio.h>
#include <stdlib.h>

struct node {
    char *data;
    struct node *next;
};

typedef struct node Node;

Node *head = NULL;

int main() {

    head = (Node *)malloc(sizeof(Node));  // 먼저 노드를 만듦
    head->data = "Tuesday";
    head->next = NULL;

    Node *q = (Node *)malloc(sizeof(Node));
    q->data = "Thursday";
    q->next = NULL;
    head->next = q;

    q = (Node *)malloc(sizeof(Node));
    q->data = "Monday";
    q->next = head;
    head = q;

    Node *p = head;
    while(p != NULL) {
        printf("%s\n", p->data);
        p = p->next;
    }
}