#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Tree.h"
// Эта функция сохраняет дерево принятия решений в файл в порядке обхода в прямом порядке.
// Она принимает в качестве аргументов указатель на файл и указатель на текущий узел.
// Если текущий узел равен NULL, она записывает символ "#" в файл, чтобы указать
// пустой узел. В противном случае она записывает вопрос текущего узла в файл,
// затем дочерние узлы "да" и "нет".
void save_tree(FILE *file, node *current_node) {
    if (current_node == NULL) {
        fprintf(file, "#\n");
        return;
    }
    fprintf(file, "%s\n", current_node->question);
    save_tree(file, current_node->yes);
    save_tree(file, current_node->no);
}
// Эта функция загружает дерево принятия решений из файла и создает соответствующие узлы.
// Она принимает в качестве аргумента указатель на файл и считывает вопрос из файла.
// Если считанный вопрос равен "#\n", это означает пустой узел и функция возвращает NULL.
// В противном случае функция создает новый узел с считанным вопросом и рекурсивно загружает
// дочерние узлы "да" и "нет".
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
// Эта функция рекурсивно печатает дерево в порядке обхода в прямом порядке.
// Она принимает в качестве аргумента указатель на текущий узел.
// Если текущий узел равен NULL, функция ничего не делает.
// В противном случае она печатает вопрос текущего узла и рекурсивно вызывается
// для дочерних узлов "да" и "нет".
void printTree(node *currentNode) {
    if (currentNode == NULL) {
        return;
    }
    printf("%s\n", currentNode->question);
    printTree(currentNode->yes);
    printTree(currentNode->no);
}
// Эта функция записывает сообщение журнала в файл журнала с меткой времени.
// Она принимает в качестве аргумента сообщение для записи.
// Функция открывает файл журнала с именем "log.txt" в режиме добавления,
// записывает сообщение с меткой времени в файл и закрывает файл.
void log_message(char *message) {
    time_t current_time = time(NULL);
    char *timestamp = ctime(&current_time);
    timestamp[strlen(timestamp)-1] = '\0';
    FILE *log_file = fopen("log.txt", "a");
    fprintf(log_file, "[%s] %s\n", timestamp, message);
    fclose(log_file);
}
// Эта функция создает новый узел с заданным вопросом и устанавливает указатели "да" и "нет" в NULL.
// Она принимает в качестве аргумента вопрос для нового узла и возвращает указатель на новый узел.
node* create_node(char *question) {
    node *new_node = malloc(sizeof(node));
    new_node->question = question;
    new_node->yes = NULL;
    new_node->no = NULL;
    return new_node;
}
// Эта функция добавляет новый вопрос и объект в дерево, обновляя вопрос текущего узла
// и создавая два новых узла для ответов "да" и "нет".
// Она принимает в качестве аргументов указатель на текущий узел, новый вопрос и новый объект.
void add_question(node *current_node, char *new_question, char *new_object) {
    char *old_question = current_node->question;
    current_node->question = new_question;
    current_node->no = create_node(old_question);
    current_node->yes = create_node(new_object);
    log_message("Added new question and object to tree");
}
// Эта функция рекурсивно задает вопросы до тех пор, пока не достигнет листового узла,
// после чего делает предположение и спрашивает пользователя, правильно ли оно.
// Если предположение неверно, пользователь может добавить новый объект и вопрос в дерево.
// Функция принимает в качестве аргумента указатель на текущий узел.
void ask_question(node *current_node) {
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
            object[strlen(object)-1] = '\0';
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