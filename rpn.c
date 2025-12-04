#include "rpn.h"
#include "stack.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int get_precedence(char c)
{
    switch (c) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

int is_left_associative(char c)
{
    return c != '^';
}

/* convert an infix expression to postfix (rpn) using the */
/* shunting yard algorithm. */
/* return a queue containing the postfix expression. */
/* if an error occurs during evaluation, return silently with NULL. */
Queue *infix_to_postfix(char *expr)
{
    Queue *queue = initialise_queue();
    Stack *stack = initialise_stack();
    int i = 0;
    int j = 0;
    int expecting_operand = 1;
    char *top_op_str;
    char current_op_str[2];
    char num_buf[64];
    int found_paren;

    if (!queue || !stack) {
        return NULL;
    }

    while (expr[i]) {
        if (expr[i] == ' ') {
            i++;
            continue;
        }

        if (isdigit(expr[i]) || expr[i] == '.' ||
           ((expr[i] == '+' || expr[i] == '-') && expecting_operand)) {

            j = 0;
            if (expr[i] == '+' || expr[i] == '-') {
                num_buf[j++] = expr[i++];
            }

            while (isdigit(expr[i]) || expr[i] == '.') {
                if (j < 63) num_buf[j++] = expr[i];
                i++;
            }
            num_buf[j] = '\0';

            if (j == 1 && (num_buf[0] == '+' || num_buf[0] == '-')) {
                free_queue(queue);
                free_stack(stack);
                return NULL;
            }

            push_queue(queue, num_buf, j + 1);
            expecting_operand = 0;
            continue;
        }

        if (expr[i] == '(') {
            push_stack(stack, "(", 2);
            expecting_operand = 1;
            i++;
        }
        else if (expr[i] == ')') {
            found_paren = 0;
            while ((top_op_str = (char *)peek_stack(stack)) != NULL) {
                if (strcmp(top_op_str, "(") == 0) {
                    free(pop_stack(stack));
                    found_paren = 1;
                    break;
                }
                push_queue(queue, top_op_str, strlen(top_op_str) + 1);
                free(pop_stack(stack));
            }
            if (!found_paren) {
                free_queue(queue);
                free_stack(stack);
                return NULL;
            }
            expecting_operand = 0;
            i++;
        }
        else if (is_operator(expr[i])) {
            while ((top_op_str = (char *)peek_stack(stack)) != NULL) {
                if (strcmp(top_op_str, "(") == 0) break;

                if ((get_precedence(top_op_str[0]) > get_precedence(expr[i])) ||
                    (get_precedence(top_op_str[0]) == get_precedence(expr[i]) && is_left_associative(expr[i]))) {
                    push_queue(queue, top_op_str, 2);
                    free(pop_stack(stack));
                } else {
                    break;
                }
            }
            current_op_str[0] = expr[i];
            current_op_str[1] = '\0';
            push_stack(stack, current_op_str, 2);
            expecting_operand = 1;
            i++;
        }
        else {
            free_queue(queue);
            free_stack(stack);
            return NULL;
        }
    }

    while ((top_op_str = (char *)peek_stack(stack)) != NULL) {
        if (strcmp(top_op_str, "(") == 0) {
            free(pop_stack(stack));
            free_queue(queue);
            free_stack(stack);
            return NULL;
        }
        push_queue(queue, top_op_str, 2);
        free(pop_stack(stack));
    }

    free_stack(stack);
    return queue;
}

/* evaluate the rpn expression given in *queue. */
/* return the value of the evaluated expression. */
/* if an error occurs during evaluation, return silently with HUGE_VAL. */
/* assume a precision of eight decimal places when performing arithmetic. */
double evaluate_rpn(Queue *queue)
{
    Stack *stack = initialise_stack();
    char *token;
    double *val1_ptr, *val2_ptr, *res_ptr;
    double v1, v2, res_val;
    char *endptr;

    if (!queue || !stack) {
        return HUGE_VAL;
    }

    while ((token = (char *)pop_queue(queue)) != NULL) {

        if (is_operator(token[0]) && strlen(token) == 1) {
            val2_ptr = (double *)pop_stack(stack);
            val1_ptr = (double *)pop_stack(stack);

            if (!val1_ptr || !val2_ptr) {
                if (val2_ptr) free(val2_ptr);
                if (val1_ptr) free(val1_ptr);
                free(token);
                free_stack(stack);
                return HUGE_VAL;
            }

            v1 = *val1_ptr;
            v2 = *val2_ptr;
            free(val1_ptr);
            free(val2_ptr);

            switch (token[0]) {
                case '+': res_val = v1 + v2; break;
                case '-': res_val = v1 - v2; break;
                case '*': res_val = v1 * v2; break;
                case '/': res_val = v1 / v2; break;
                case '^': res_val = pow(v1, v2); break;
                default: res_val = 0.0; break;
            }
            push_stack(stack, &res_val, sizeof(double));

        } else {
            res_val = strtod(token, &endptr);
            if (endptr == token) {
                free(token);
                free_stack(stack);
                return HUGE_VAL;
            }
            push_stack(stack, &res_val, sizeof(double));
        }
        free(token);
    }

    res_ptr = (double *)pop_stack(stack);
    if (!res_ptr) {
        free_stack(stack);
        return HUGE_VAL;
    }

    res_val = *res_ptr;
    free(res_ptr);

    if (peek_stack(stack) != NULL) {
        free_stack(stack);
        return HUGE_VAL;
    }

    free_stack(stack);
    return res_val;
}
