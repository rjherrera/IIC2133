Element null_element;

struct trie_node{
    // int n;
    int n_kids;
    Element note_1;
    Element note_2;
    char* value;
    int passes;
    int level;
    struct trie_node **kids;
    struct trie_node *parent;
};
typedef struct trie_node TrieNode;


TrieNode* create_node(char* value, int potential_elements, Element note_1, Element note_2){
    TrieNode* node = malloc(sizeof(TrieNode));
    node -> n_kids = 0;
    node -> value = malloc(strlen(value) + 1);
    // node -> note_1 = malloc(sizeof(Element));
    // node -> note_2 = malloc(sizeof(Element));
    strcpy(node -> value, value);
    node -> note_1 = note_1;
    node -> note_2 = note_2;
    node -> kids = malloc(sizeof(TrieNode) * potential_elements);
    node -> passes = 1;
    node -> level = 0;
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
    TrieNode* leaf = create_node("$", 1, null_element, null_element);
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
    TrieNode* element_node = create_node(element, 1, null_element, null_element);
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
        TrieNode* element_node = create_node(element, 1, null_element, null_element);
        int present = is_in(element_node, root -> kids, root -> n_kids);
        if (present) root = root -> kids[present - 1];
        else return false;
    }
    *appearances = count_leafs(root);
    return true;
}


void print_trie(TrieNode *root, int level){
    printf("%d) Node(%s,%d)\n", level, root -> value, root -> passes);
    for (int i = 0; i < root -> n_kids; ++i){
        print_trie(root -> kids[i], level + 1);
    }
}

void level_trie(TrieNode *root, int level){
    root -> level = level;
    for (int i = 0; i < root -> n_kids; ++i){
        level_trie(root -> kids[i], level + 1);
    }
}


TrieNode* build_trie(int size, char **string_array, Element *notes_array, int mu, TrieNode **candidates_array, int *candidates){
    TrieNode* root = create_node("ROOT", size, null_element, null_element);
    TrieNode* current = root;
    for (int i = 0; i < size; i++) {
        current = root;
        int level = 0;
        // printf("Vuelta al ROOT\n");
        for (int j = i; j < size; j++){
            // printf("Current: %s\n", current -> value);
            char* value = string_array[j];
            Element note_1 = notes_array[j];
            Element note_2;
            if (j + 1 < size) note_2 = notes_array[j + 1];
            else note_2 = null_element;
            TrieNode* potential_node = create_node(value, size, note_1, note_2);
            int k = current -> n_kids;
            int present = is_in(potential_node, current -> kids, current -> n_kids);
            if (!present){
                current -> kids[k] = potential_node;
                current -> n_kids = current -> n_kids + 1;
                // digo quien es mi padre
                current -> kids[k] -> parent = current;
                // printf("  Hijos de %s: %d\n", current->value, current -> n_kids);
                // printf("  Nuevo in %s: Node(%s)\n", current->value, value);
            }
            else {
                // printf("  Ya estaba: %s en Node(%s)[%d]\n", value, current -> value, present);
                k = present - 1;
                // si ya estaba, no es necesario decir qn es mi padre;
                current -> kids[k] -> passes++;
                // if (current -> passes >= mu){
                //     candidates_array[*candidates] = current -> kids[k];
                //     *candidates = *candidates + 1;
                //     printf("estetienemasdemu(%d): %s\n", mu, current -> kids[k] -> value);
                // }
            }
            current = current -> kids[k];
            level++;
            if (level >= mu - 1 && current -> passes > 1){
                current -> level = level;
                candidates_array[*candidates] = current;
                *candidates = *candidates + 1;
                printf("Candidato de prof: %d (%s)\n", level, current -> value);
            }
        }
    }
    return root;
}

void print_route(TrieNode *node, int *route_length){
    TrieNode* current = node;
    while (strcmp(current -> value, "ROOT")){
        printf("%s", current -> value);
        current = current -> parent;
        *route_length = *route_length + 1;
    }
    printf("[::-1]\n");
}