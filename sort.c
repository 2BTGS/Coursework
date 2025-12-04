#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

static Node *split_list(Node *head)
{
    Node *fast = head;
    Node *slow = head;
    Node *temp;

    if (head == NULL || head->next == NULL) {
        return NULL;
    }

    while (fast->next != NULL && fast->next->next != NULL) {
        fast = fast->next->next;
        slow = slow->next;
    }

    temp = slow->next;
    slow->next = NULL;
    if (temp != NULL) {
        temp->prev = NULL;
    }

    return temp;
}

static Node *merge_lists(Node *first, Node *second, int (*compare)(void *, void *))
{
    if (first == NULL) return second;
    if (second == NULL) return first;

    if (compare(first->data, second->data) <= 0) {
        first->next = merge_lists(first->next, second, compare);
        if (first->next != NULL) {
            first->next->prev = first;
        }
        first->prev = NULL;
        return first;
    } else {
        second->next = merge_lists(first, second->next, compare);
        if (second->next != NULL) {
            second->next->prev = second;
        }
        second->prev = NULL;
        return second;
    }
}

static Node *merge_sort_recursive(Node *head, int (*compare)(void *, void *))
{
    Node *second_half;

    if (head == NULL || head->next == NULL) {
        return head;
    }

    second_half = split_list(head);

    head = merge_sort_recursive(head, compare);
    second_half = merge_sort_recursive(second_half, compare);

    return merge_lists(head, second_half, compare);
}

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
    Node *current;

    if (list == NULL || list->head == NULL || list->head->next == NULL) {
        return;
    }

    list->head = merge_sort_recursive(list->head, compare);

    current = list->head;
    if (current != NULL) {
        while (current->next != NULL) {
            current = current->next;
        }
    }
    list->tail = current;
}
