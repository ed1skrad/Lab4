#include <stdio.h>
#include "Tree.h"


int main() {
    node *root = NULL;

    FILE *file = fopen("database.txt", "r");

    if (file != NULL) {
        root = load_tree(file);
        fclose(file);
    } else {
        root = create_node("Is it an VAG?");
        add_question(root, "Is it a VAG?", "Volkswagen");
    }

    printf("Let's play.\n");

    ask_question(root);

    file = fopen("database.txt", "w");

    save_tree(file, root);
    printTree(root);

    fclose(file);

    return 0;
}