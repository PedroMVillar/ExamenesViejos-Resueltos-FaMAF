#include <stdio.h>
#include <stdlib.h>
#include "walk.h"
#include <assert.h>


#define STEP_NORTH 0
#define STEP_EAST 1
#define STEP_NORTHEAST 2
#define STEP_UNDEF 3

typedef unsigned int step_t;
typedef unsigned int uint_t;

struct _node_t {
    step_t step;
    struct _node_t *next;
};

typedef struct _node_t *node_t;

struct _walk_t {
    node_t init;
    node_t last;
    uint_t length;
    uint_t width;
    uint_t height;
};

walk_t walk_empty(void) {
    struct _walk_t *new_walk= malloc(sizeof(struct _walk_t));
    new_walk->init = NULL;
    new_walk->last = NULL;
    new_walk->length = 0u;
    new_walk->width = 0u;
    new_walk->height = 0u;
    /* POS: {walk --> walk_t && walk_length(walk) == 0} */
    assert(walk_length(new_walk) == 0);
    return new_walk;
}

static node_t create_node(void){
    struct _node_t *new_node = malloc(sizeof(struct _node_t));
    assert(new_node != NULL);
    new_node->step = STEP_UNDEF;
    new_node->next = NULL;
    return new_node;
}

static walk_t walk_add(walk_t walk , node_t new_node) {
    if(walk->init == walk->last){
        walk->init = new_node; walk->last = new_node;
    } else {
        struct _node_t *aux = walk->init;
        while (aux->next != NULL) aux = aux->next;
        aux->next = new_node; walk->last = new_node;
    }
    walk->length++;
    return walk;
}

walk_t walk_north(walk_t walk) {
    struct _node_t *node = create_node();
    node->step = STEP_NORTH;
    walk_add(walk, node);
    walk->height++;
    /* POS: { walk_length(walk) > 0} */
    assert(walk_length(walk) > 0);
    return walk;
}

walk_t walk_east(walk_t walk) {
    struct _node_t *node = create_node();
    node->step = STEP_EAST;
    walk_add(walk, node);
    walk->width++;
    /* POS: { walk_length(walk) > 0} */
    assert(walk_length(walk) > 0);
    return walk;
}

walk_t walk_northeast(walk_t walk) {
    struct _node_t *node = create_node();
    node->step = STEP_NORTHEAST;
    walk_add(walk, node);
    walk->height++;
    walk->width++;
    /* POS: { walk_length(walk) > 0} */
    assert(walk_length(walk) > 0);
    return walk;
}

unsigned int walk_length(walk_t walk) {
    return walk->length;
}

unsigned int walk_height(walk_t walk) {
    return walk->height;
}

unsigned int walk_width(walk_t walk) {
    return walk->width;
}

walk_t walk_extend(walk_t walk, unsigned int east, unsigned int north) {
    for (unsigned int i= 0u; i < east; i++){
        walk = walk_east(walk);
    }
    for (unsigned int i= 0u; i < north; i++){
        walk = walk_north(walk);
    }
    /* POS: {walk --> walk_t && (walk_length(walk) > 0 || (e == 0 && n == 0))} */
    assert(walk_length(walk) > 0 || (east == 0 && north == 0));
    return walk;
}

static char step_to_char (step_t step) {
    char c = '\0';
    switch (step) {
        case STEP_NORTH : c = 'N'; break;
        case STEP_EAST  : c = 'E' ; break;
        case STEP_NORTHEAST : c = 'X' ; break;
        default : c = 'U' ; break;
    }
    return c;
}

void walk_dump(walk_t walk) {
    assert(walk != NULL);
    node_t current = NULL;
    current = walk -> init;
    printf("[");
    while (current != NULL) {
        printf("%c",step_to_char(current->step));
        if(current->next != NULL) {
            printf(" -> ");
        }
        current = current -> next;
    }
    printf("] (L=%u,W=%u,H=%u)\n",walk->length,walk->width,walk->height);
}

walk_t walk_destroy(walk_t walk) {
    struct _node_t *aux = walk->init;
    struct _node_t *killme;
    if(aux == NULL){
        free(walk); 
        walk = NULL;
        assert(walk == NULL);
        return walk;
    }
    while(aux->next != NULL){
        killme = aux;
        aux = aux->next;
        killme->next = NULL;
        free(killme);
    }
    aux->next = NULL;
    free(aux);
    walk->init = walk->last;
    walk->last = NULL;
    return(walk);
}

