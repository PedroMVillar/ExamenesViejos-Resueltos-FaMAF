#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <assert.h>

#include "stringbuffer.h"

struct s_stringbuffer {
    struct s_node *first;
    struct s_node *last;
    unsigned int length;
};

struct s_node {
    char c;
    struct s_node *nextchar;
};

typedef struct s_node * node_t;

// Representation invariant
static bool invrep(stringbuffer sb) {
    bool valid=true;
    if(sb == NULL){
        valid = false;
    }
    else if(stringbuffer_is_empty(sb)){
        valid = sb->length == 0;
    }
    return valid;
}


// Creates a single node in dynamic memory with a given char
static node_t create_node(char c) {
    node_t newnode=NULL;    
    newnode = malloc(sizeof(*newnode));
    assert(newnode!=NULL);
    newnode->c = c;
    newnode->nextchar = NULL;
    return newnode;
}


// Destroy a single node
static node_t destroy_node(node_t node) {
    if(node != NULL){
        free(node);
    }

    //assert(node==NULL);
    return NULL;
}


// Public functions of the Abstract Data Type
stringbuffer stringbuffer_empty(void) {
    stringbuffer sb = NULL;
    sb = malloc(sizeof(*sb));
    assert(sb != NULL);
    sb->first = NULL;
    sb->last = NULL;
    sb->length = 0u;

    return sb;
}


stringbuffer stringbuffer_create(const char *word) {
    assert(word!=NULL);

    stringbuffer sb = NULL;
    sb = stringbuffer_empty();
    unsigned int len = strlen(word);

    if(len == 0){
        return sb;
    }
    else{
        for(unsigned int i=0; i<len;i++){
            sb = stringbuffer_append(sb, word[i]);
        }
    }

    assert(invrep(sb) && !stringbuffer_is_empty(sb));
    return sb;
}


bool stringbuffer_is_empty(stringbuffer sb){
    return ((sb->first == NULL) && (sb->last == NULL));
}


stringbuffer stringbuffer_append(stringbuffer sb, const char c) {
    node_t node = create_node(c);

    if(stringbuffer_is_empty(sb)){
        sb->first = node;
        sb->last = node;
    }
    else{
        sb->last->nextchar = node;
        sb->last = node;
    }

    sb->length++;
    return sb;
}


char stringbuffer_char_at(stringbuffer sb, unsigned int index) {
    assert(invrep(sb) && !stringbuffer_is_empty(sb) && index<stringbuffer_length(sb));
    
    unsigned pos = 0;
    node_t node = sb->first;
    while(pos < index) {
        node = node->nextchar;
        pos++;
    }

    return node->c;
}

stringbuffer stringbuffer_remove(stringbuffer sb, unsigned int index) {
    assert(invrep(sb));

    if (index >= sb->length || stringbuffer_is_empty(sb)) {
        return sb;
    }

    node_t node = sb->first, prev = NULL;

    for (unsigned int pos = 0; pos < index; pos++) {
        prev = node;
        node = node->nextchar;
    }

    if (prev == NULL){ // elimino el primero
        sb->first = node->nextchar;
        if (sb->length == 1) { // si es el unico
            sb->last = NULL;
        }
    }
    else{
        prev->nextchar = node->nextchar;
        if (node->nextchar == NULL) { // si tenia que eliminar el ultimo
            sb->last = prev;
        }
    }

    destroy_node(node);
    sb->length--;
    assert(invrep(sb));
    return sb;
}



stringbuffer stringbuffer_replace(stringbuffer sb, const char c, unsigned int index) {
    if(index <= sb->length){

        unsigned pos = 0;
        node_t node = sb->first;
        while(pos < index) {
            node = node->nextchar;
            pos++;
        }
        node->c = c;
    }
    return sb;
}


unsigned int stringbuffer_length(stringbuffer sb) {
    return sb->length;
}


char* stringbuffer_to_string(stringbuffer sb) {
    char *string = NULL;
    unsigned int len = sb->length;

    if(len > 0){
        string = calloc(len+1,sizeof(char));
        node_t p = sb->first;

        for(unsigned int pos = 0; pos < len && p != NULL;pos++){
            string[pos] = p->c;
            p = p->nextchar;
        }
        
        if(p==NULL){
            string[len] = '\0';
        }
    }

    return string;
}


void stringbuffer_dump(stringbuffer sb) {
    assert(invrep(sb));
    node_t node = sb->first;
    while (node != NULL) {
        printf("%c", node->c);
        node = node->nextchar;
    }
    printf("\n");
}


stringbuffer stringbuffer_destroy(stringbuffer sb) {
    while(!stringbuffer_is_empty(sb)){
        sb = stringbuffer_remove(sb, 0);
    }

    free(sb);
    return NULL;
}
