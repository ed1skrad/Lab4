#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Tree.h"

void save_tree(FILE *file, node *current_node) {
    if (current_node == NULL) {
        fprintf(file, "#\n");
        return;
    }
    fprintf(file, "%s\n", current_node->question);
    save_tree(file, current_node->yes);
    save_tree(file, current_node->no);
}

node* load_tree(FILE *file) {
    char question[256];
    if (fgets(question, 256, file) == NULL) {
        return NULL;
    }

    if (strcmp(question, "#\n") == 0) {
        return NULL;
    }

    question[strlen(question)-1] = '\0';

    node *new_node = create_node(strdup(question));

    new_node->yes = load_tree(file);
    new_node->no = load_tree(file);

    return new_node;
}

void printTree(node *currentNode) {
    if (currentNode == NULL) {
        return;
    }
    printf("%s\n", currentNode->question);
    printTree(currentNode->yes);
    printTree(currentNode->no);
}

void log_message(char *message) {
    time_t current_time = time(NULL);
    const struct tm *local_time = localtime(&current_time);
    char timestamp[20];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local_time);
    FILE *log_file = fopen("log.txt", "a");
    fprintf(log_file, "[%s] %s\n", timestamp, message);
    fclose(log_file);
}

node* create_node(char *question) {
    node *new_node = malloc(sizeof(node));
    new_node->question = question;
    new_node->yes = NULL;
    new_node->no = NULL;
    return new_node;
}

void add_question(node *current_node, char *new_question, char *new_object) {
    char *old_question = current_node->question;
    current_node->question = new_question;
    current_node->no = create_node(old_question);
    current_node->yes = create_node(new_object);
    log_message("Added new question and object to tree");
}

void ask_question(node *current_node) {
    if(current_node == NULL){
        printf("Current node is NULL.\n");
        return;
    }
    if (current_node->yes == NULL && current_node->no == NULL) {
        printf("I think you are thinking of: %s\n", current_node->question);
        printf("Is that correct?\n");
        char answer[4];
        scanf("%3s%*c", answer);
        if (strcmp(answer, "yes") == 0) {
            printf("I guess!\n");
            return;
        } else {
            printf("I didn't guess which object you made up?\n");
            char object[256];
            fgets(object, 256, stdin);
            if(strlen(object) == 0){
                printf("You must enter name of the object!\n");
                return;
            }
            if(strlen(object) > 0){
                object[strlen(object)] = '\0';
            }
            printf("What is a question that distinguishes %s from %s?\n", object, current_node->question);
            char question[256];
            fgets(question, 256, stdin);
            question[strlen(question)-1] = '\0';
            add_question(current_node, strdup(question), strdup(object));
            printf("Thank you! I'll remember that for next time.\n");
            return;
        }
    }
    printf("%s\n", current_node->question);
    char answer[4];
    scanf("%3s%*c", answer);
    if (strcmp(answer, "yes") == 0) {
        ask_question(current_node->yes);
    } else if (strcmp(answer, "no") == 0) {
        ask_question(current_node->no);
    } else {
        printf("Answer must be 'yes' or 'no'\n");
        ask_question(current_node);
    }
}
