char* string_from_melody_deprecated(Melody *melody){
    int notes_number = melody -> length;

    // int node_size = 10 + 3 + 1 + 4; // note: uint8_t; length: uint32_t; type: 1 o 0;
    // (in characters 10 + 3 + 1 + 4 (<4294967295 and <255 and 1 and -128<x<128) and the 5 stands for extra pipes and parenthesis
    // int buffer_size = node_size * notes_number; // amount of nodes * size of node;
    int node_delta_size = 10 + 4 + 1 + 4;
    int buffer_delta_size = node_delta_size * notes_number;
    // char *buffer = malloc(buffer_size);
    // char *node = malloc(node_size);
    char *buffer_delta = malloc(buffer_delta_size);
    char *node_delta = malloc(node_delta_size);
    for (int i = 0; i < notes_number; i++) {
        Element element = melody -> element_array[i];
        int delta = 0;
        if (i + 1 < notes_number) {
            Element next_element = melody -> element_array[i + 1];
            int x = element.note;
            int y = next_element.note;
            delta = y - x;
        }
        // sprintf(node, "(%hhu|%u|%d)", element.note, element.length, !element.type);
        // strcat(buffer, node);
        sprintf(node_delta, "(%d|%u|%d)", delta, element.length, !element.type);
        strcat(buffer_delta, node_delta);
    }
    printf("%s\n", buffer_delta);
    return buffer_delta;
}

void string_from_melody(char **string, Melody *melody, int notes_number, char **orig_string){
    int node_delta_size = 10 + 4 + 1 + 4;
    char *node_delta = malloc(node_delta_size);
    for (int i = 0; i < notes_number; i++) {
        Element element = melody -> element_array[i];
        int delta = 0;
        if (i + 1 < notes_number) {
            Element next_element = melody -> element_array[i + 1];
            int x = element.note;
            int y = next_element.note;
            delta = y - x;
        }
        sprintf(node_delta, "(%d|%u|%d)", delta, element.length, element.type);
        string[i] = malloc(strlen(node_delta));
        strcpy(string[i], node_delta);
    }
}