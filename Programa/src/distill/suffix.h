struct trie_node{
    // int n;
    int n_kids;
    char* value;
    struct trie_node **kids;
};
typedef struct trie_node TrieNode;


TrieNode* create_node(char* value, int potential_elements){
    TrieNode* node = malloc(sizeof(TrieNode));
    node -> n_kids = 0;
    node -> value = malloc(strlen(value) + 1);
    strcpy(node -> value, value);
    node -> kids = malloc(sizeof(TrieNode) * potential_elements);
    return node;
}


bool node_equality(TrieNode *node_a, TrieNode *node_b){
    return !strcmp(node_a -> value, node_b -> value);
}


int is_in(TrieNode *node, TrieNode **node_array, int size){
    // printf(" Kids:");
    // for (int i = 0; i < size; ++i) printf(" %s", node_array[i] -> value);
    // printf("\n");
    for (int i = 0; i < size; i++) {
        // printf(" Esta: %d |%s==%s\n", node_equality(node_array[i], node), node_array[i] -> value, node -> value);
        if (node_equality(node_array[i], node)) return i + 1;
    }
    return 0;
}


int count_leafs(TrieNode *root){
    TrieNode* leaf = create_node("$", 1);
    int count = 0;
    for (int i = 0; i < root -> n_kids; ++i){
        if (node_equality(leaf, root -> kids[i])) count++;
        else count += count_leafs(root -> kids[i]);
    }
    return count;
}


bool substring_exists_naive(TrieNode *root, char **substring, int elements_left){
    char* element = substring[0];
    if (!elements_left) return true;
    TrieNode* element_node = create_node(element, 1);
    for (int i = 0; i < root -> n_kids; ++i){
        // printf("¿N(%s)==%s?\n", root -> kids[i] -> value, element_node -> value);
        if (node_equality(root -> kids[i], element_node)){
            if (substring_exists_naive(root -> kids[i], &substring[1], elements_left - 1)) return true;
        }
    }
    return false;
}


bool substring_exists(TrieNode *root, char **substring, int size, int* appearances){
    for (int i = 0; i < size; ++i){
        char* element = substring[i];
        TrieNode* element_node = create_node(element, 1);
        int present = is_in(element_node, root -> kids, root -> n_kids);
        if (present) root = root -> kids[present - 1];
        else return false;
    }
    *appearances = count_leafs(root);
    return true;
}


void print_trie(TrieNode *root, int level){
    printf("%d) Node(%s)\n", level, root -> value);
    for (int i = 0; i < root -> n_kids; ++i){
        print_trie(root -> kids[i], level + 1);
    }
}


TrieNode* build_trie(int size, char **string_array){
    TrieNode* root = create_node("ROOT", size);
    TrieNode* current = root;
    for (int i = 0; i < size; i++) {
        current = root;
        // printf("Vuelta al ROOT\n");
        for (int j = i; j < size; j++){
            // printf("Current: %s\n", current -> value);
            char* value = string_array[j];
            TrieNode* potential_node = create_node(value, size);
            int k = current -> n_kids;
            int present = is_in(potential_node, current -> kids, current -> n_kids);
            if (!present){
                current -> kids[k] = potential_node;
                current -> n_kids = current -> n_kids + 1;
                // printf("  Hijos de %s: %d\n", current->value, current -> n_kids);
                // printf("  Nuevo in %s: Node(%s)\n", current->value, value);
            }
            else {
                // printf("  Ya estaba: %s en Node(%s)[%d]\n", value, current -> value, present);
                k = present - 1;
            }
            current = current -> kids[k];
        }
    }
    return root;
}