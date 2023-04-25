#include <stdio.h>
#include "Tree.h"


int main() {
    log_message("Program started");
    node *root = NULL;

    FILE *file = fopen("database.txt", "r");

    if (file != NULL) {
        log_message("Loading tree from file");
        root = load_tree(file);
        fclose(file);
        log_message("Finished loading tree from file");
    } else {
        log_message("Creating new tree");
        root = create_node("Is it an VAG?");
        add_question(root, "Is it a VAG?", "Volkswagen");
        log_message("Finished creating new tree");
    }

    printf("Let's play.\n");

    ask_question(root);

    file = fopen("database.txt", "w");
    if(file == NULL){
        printf("Cant open file!");
        return 0;
    }

    log_message("Saving tree to file");
    save_tree(file, root);
    fclose(file);
    log_message("Finished saving tree to file");

    log_message("Program finished");

    return 0;
}