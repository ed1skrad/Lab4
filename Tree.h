//
// Created by Artem on 12.04.2023.
//

#ifndef LAB4READY_TREE_H
#define LAB4READY_TREE_H
// Эта структура определяет узел в дереве принятия решений. Каждый узел имеет вопрос,
// указатель на дочерний узел "да" и указатель на дочерний узел "нет".
typedef struct node {
    char *question;
    struct node *yes;
    struct node *no;
} node;
node* create_node(char *question);
void add_question(node *current_node, char *new_question, char *new_object);
void ask_question(node *current_node);
void save_tree(FILE *file, node *current_node);
node* load_tree(FILE *file);
void printTree(node *currentNode);
void log_message(char *message);
#endif //LAB4READY_TREE_H
