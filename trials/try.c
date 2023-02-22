#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node* next;
};

int main() {
    // Create a linked list with three nodes
    struct node* head = (struct node*) malloc(sizeof(struct node));
    struct node* second = (struct node*) malloc(sizeof(struct node));
    struct node* third = (struct node*) malloc(sizeof(struct node));

    head->data = 1;
    head->next = second;

    second->data = 2;
    second->next = third;

    third->data = 3;
    third->next = NULL;

    // Iterate over the linked list
    struct node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }

    return 0;
}
