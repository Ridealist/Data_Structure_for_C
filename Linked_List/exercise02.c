#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    char *str;
    struct node *next;
} Node;

Node *head = NULL;

int main() {

    Node *q = (Node *)malloc(sizeof(Node));
    q->str = "Wednesday";
    q->next = NULL;

    Node *r = (Node *)malloc(sizeof(Node));
    r->str = "Thurday";
    r->next = NULL;
    q->next = r;

    r = (Node *)malloc(sizeof(Node));
    r->str = "Monday";
    r->next = q;
    head = r;

    r = (Node *)malloc(sizeof(Node));
    r->str = "Tuesday";
    r->next = head->next;
    head->next = r;

    r = (Node *)malloc(sizeof(Node));
    r->str = "Friday";
    r->next = NULL;
    head->next->next->next->next = r;

    q = head;
    while (q != NULL)
    {
        printf("%s\n", q->str);
        q = q->next;
    }
    return 0;
}