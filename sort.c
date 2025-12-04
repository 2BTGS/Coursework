#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

/* sort linked list *list using merge insertion sort. */
/* upon success, the elements in *list will be sorted. */
/* return silently if *list is uninitialised or empty. */
/* the compare argument is a pointer to a function which returns */
/* less than 0, 0, or greater than 0 if first argument is */
/* less than, equal to, or greater than second argument respectively. */
void insertion_sort(LinkedList *list, int (*compare)(void *, void *))
{
    Node *sorted_head = NULL;
    Node *sorted_tail = NULL;
    Node *current = NULL;
    Node *next_node = NULL;
    Node *search = NULL;

    if (list == NULL || list->head == NULL || list->head->next == NULL) {
        return;
    }

    current = list->head;

    while (current != NULL) {
        next_node = current->next;

        current->next = NULL;
        current->prev = NULL;

        if (sorted_head == NULL) {
            sorted_head = current;
            sorted_tail = current;
        } else if (compare(current->data, sorted_head->data) < 0) {
            current->next = sorted_head;
            sorted_head->prev = current;
            sorted_head = current;
        } else {
            search = sorted_head;
            while (search != NULL && compare(search->data, current->data) <= 0) {
                search = search->next;
            }

            if (search == NULL) {
                sorted_tail->next = current;
                current->prev = sorted_tail;
                sorted_tail = current;
            } else {
                current->next = search;
                current->prev = search->prev;
                search->prev->next = current;
                search->prev = current;
            }
        }
        current = next_node;
    }

    list->head = sorted_head;
    list->tail = sorted_tail;
}

/* sort linked list *list using merge sort algorithm. */
/* upon success, the elements in *list will be sorted. */
/* return silently if *list is uninitialised or empty. */
/* the compare argument is a pointer to a function which returns */
/* less than 0, 0, or greater than 0 if first argument is */
/* less than, equal to, or greater than second argument respectively. */
void merge_sort(LinkedList *list, int (*compare)(void *, void *))
{

}
