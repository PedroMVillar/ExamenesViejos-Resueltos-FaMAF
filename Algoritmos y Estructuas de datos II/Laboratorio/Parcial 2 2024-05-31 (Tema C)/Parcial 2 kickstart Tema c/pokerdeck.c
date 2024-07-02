#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <assert.h>

#include "pokerdeck.h"

struct s_pokerdck {
    unsigned int size;
    struct s_node *first;
    struct s_node *last;
};

struct s_node {
    cardnum_t num;
    cardsuit_t suit;
    struct s_node *nextcard;
};

typedef struct s_node * node_t;

// Auxiliar dump functions
static void dump_suit(cardsuit_t suit) {
    printf("%s", suit==spades   ? "♠️  ":
                 suit==hearts   ? "♥️  ":
                 suit==diamonds ? "♦️  ":
                 suit==clubs    ? "♣️  ": "???");
}

static void dump_num(cardnum_t num) {
    if (1 < num && num < 11) {
        printf("%-2d", num);
    } else {
        printf("%-2s", num==1 ? "A":
                       num==11 ? "J":
                       num==12 ? "Q":
                       num==13 ? "K": "???");

    }
}

// Representation invariant
static bool invrep(pokerdeck deck) {
    bool valid=true;
    
    if(deck == NULL){
        valid = false;
    }



    // FALTA

    return valid;
}

// Creates a single node in dynamic memory with a given card
static node_t create_node(cardnum_t num, cardsuit_t suit) {
    node_t newnode=NULL;
    
    newnode = malloc(sizeof(*newnode));
    newnode->suit = suit;
    newnode->num = num;
    newnode->nextcard = NULL;

    assert(newnode!=NULL);
    return newnode;
}

// Destroy a single node
static node_t destroy_node(node_t node) {
    if(node != NULL){
        free(node);
    }

    //assert(node==NULL);
    return node;
}

// Public functions of the Abstract Data Type
pokerdeck pokerdeck_empty(void) {
    
    pokerdeck deck = NULL;
    deck = malloc(sizeof(*deck));
    deck->first = NULL;
    deck->last = NULL;
    deck->size = 0;
    
    return deck;
}

bool pokerdeck_is_empty(pokerdeck deck) {
    bool b = (deck->first == NULL) && (deck->last == NULL) ;
    return b;
}

pokerdeck pokerdeck_add(pokerdeck deck, cardnum_t num, cardsuit_t suit) {
    
    node_t node = NULL;
    node = create_node(num, suit);

    if(pokerdeck_is_empty(deck)){
        deck->first = node;
        deck->last = node;
    }
    else{
        deck->last->nextcard = node;
        deck->last = node;
    }
    
    deck->size++;
    return deck;
}

pokerdeck pokerdeck_push(pokerdeck deck,  cardnum_t num, cardsuit_t suit) {
    assert(invrep(deck));
    node_t newnode = create_node(num, suit);

    if(pokerdeck_is_empty(deck)){
        deck->first = newnode;
        deck->last = newnode;
    }
    else{
        newnode->nextcard = deck->first;
        deck->first = newnode;
    }

    deck->size++;
    assert(invrep(deck) && !pokerdeck_is_empty(deck));
    return deck;
}

pokerdeck pokerdeck_pop(pokerdeck deck, cardnum_t *popped_num, cardsuit_t *popped_suit) {

    if(pokerdeck_is_empty(deck)){
        return deck;
    }
    else{
        cardnum_t num;
        cardsuit_t suit;

        node_t p = NULL;
        p = deck->first;
        num = p->num;
        suit = p->suit;

        if(deck->size == 1){
            deck->first = NULL;
            deck->last = NULL;
            p = destroy_node(p);
        }
        else{
            deck->first = deck->first->nextcard;
            p = destroy_node(p);
        }

        if (popped_num!=NULL){
            *popped_num= num;
        } 
        if (popped_suit!=NULL){
            *popped_suit=suit;
        } 
    }
    deck->size--;
    return deck;
}

unsigned int pokerdeck_length(pokerdeck deck) {
    return deck->size;
}

pokerdeck pokerdeck_remove(pokerdeck deck, cardnum_t num, cardsuit_t suit) {
    if(pokerdeck_is_empty(deck)){
        return deck;
    }
    else{
        node_t p = NULL;
        p = deck->first;
        
        if(p->num == num && p->suit == suit){
            deck = pokerdeck_pop(deck, NULL, NULL);
        }
        else{
            while(p->nextcard != NULL && p->num != num && p->suit != suit){
                p = p->nextcard;
            }
            
            if(p->nextcard == NULL){
                return deck;
            }
            else{
                node_t killme = p->nextcard;
                if(killme == deck->last){
                    deck->last = p;
                    killme = destroy_node(killme);
                    killme = NULL;
                }
                else{
                    p->nextcard = killme->nextcard;
                    killme = destroy_node(killme);
                    killme = NULL;
                }
            }
        }
    }

    deck->size--;
    return deck;
}

unsigned int pokerdeck_count(pokerdeck deck, cardsuit_t suit) {
    unsigned int counter = 0u;
    node_t p = deck->first;

    for(unsigned int i=0u; p->nextcard!=NULL && i < deck->size ;i++){
        if(p->suit == suit){
            counter++;
        }
        p = p->nextcard;
    }
    return counter;
}

struct card * pokerdeck_to_array(pokerdeck deck) {
    struct card *array = NULL;
    array = calloc(deck->size, sizeof(struct card));
    
    node_t p = deck->first;

    for(unsigned int i=0; i < deck->size && p!=NULL ;i++){
        struct card card_to_array;
        card_to_array.num = p->num;
        card_to_array.suit = p->suit;
        array[i] = card_to_array;
        p = p->nextcard;
    }

    return array;
}

void card_dump(cardnum_t num, cardsuit_t suit) {
    printf("|");
    dump_num(num);
    printf(" ");
    dump_suit(suit);
    printf("|");
}

void pokerdeck_dump(pokerdeck deck) {
    assert(invrep(deck));
    node_t node = deck->first;
    while (node != NULL) {
        card_dump(node->num, node->suit);
        printf(" ");
        node = node->nextcard;
    }
    printf("\n");
}

pokerdeck pokerdeck_destroy(pokerdeck deck) {

    while(!pokerdeck_is_empty(deck)){
        deck = pokerdeck_pop(deck, NULL, NULL);
    }

    free(deck);

    return NULL;
}
