#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

LinkedList *linked_list_create() {
    LinkedList *list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->size = 0;
    return list;
}

void linked_list_append(LinkedList *list, char *value) {
    LinkedListNode *new_node = malloc(sizeof(LinkedListNode));
    new_node->value = value;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        LinkedListNode *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }

    list->size++;
}

void linked_list_free(LinkedList *list) {
    LinkedListNode *current = list->head;
    LinkedListNode *next_node;

    while (current) {
        next_node = current->next;
        free(current->value);
        free(current);
        current = next_node;
    }

    free(list);
}

void print_commands(LinkedList *commands) {
    LinkedListNode *current_node = commands->head;

    while (current_node != NULL) {
        Command *cmd = (Command *)current_node->value;
        printf("Command: %s\n", cmd->name);
        printf("Arguments:\n");

        LinkedListNode *arg_node = cmd->args->head;
        while (arg_node != NULL) {
            printf("\t%s\n", (char *)arg_node->value);
            arg_node = arg_node->next;
        }

        if (cmd->input_file) {
            printf("Input file: %s\n", cmd->input_file);
        }
        if (cmd->output_file) {
            printf("Output file: %s\n", cmd->output_file);
            if (cmd->append_output) {
                printf("Output mode: Append\n");
            } else {
                printf("Output mode: Overwrite\n");
            }
        }

        printf("\n");
        current_node = current_node->next;
    }
}


void linked_list_print(LinkedList *list) {
    LinkedListNode *current = list->head;
    printf("[");

    while (current) {
        printf("'%s'", current->value);
		printf("'%u'", current->type);
        if (current->next) {
            printf(", ");
        }
        current = current->next;
    }

    printf("]\n");
}
