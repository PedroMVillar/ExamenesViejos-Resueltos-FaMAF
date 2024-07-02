#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "character/character.h"
#include "pqueue.h"

/* ============================================================================
STRUCTS!
============================================================================ */
struct s_pqueue {
  struct s_node* front; // Puntero a la cola de prioridades //
  unsigned int size; // Tamaño de la cola de prioridades //
};
struct s_node {
  Character character; // Caracter epresenta a cada personaje con sus atributos  //
  float priority; // Pioridad/Iniciativa del personaje //
  struct s_node* next; // Puntero al siguiente //
};
/* ============================================================================
INVREP
============================================================================ */
static bool invrep(pqueue q) {
  bool res = q != NULL;
  if(q->size == 0)res = q->front == NULL;
  else if (q->size > 0){
    struct s_node *aux = q->front;
    while(aux->next != NULL){
      res = aux->priority >= aux->next->priority; aux = aux->next;
    }   
  }
  return res;
}
/* ============================================================================
NEW
============================================================================ */

pqueue pqueue_empty(void) {
  /* PRE: {true} */
  pqueue q = malloc(sizeof(struct s_pqueue));
  /* INICIALIZACIÓN DE VALORES */
  q->size = 0u;
  q->front = NULL;
  /* POS: {q --> pqueue && pqueue_is_empty(q)} */
  assert(invrep(q) && pqueue_is_empty(q));
  return q;
}

/* ============================================================================
ENQUEUE
============================================================================ */

static float calculate_priority(Character character) {
  charttype_t type = character_ctype(character);
  unsigned int agility = (float) character_agility(character);
  bool alive = character_is_alive(character);
  float priority = 0.0;
  if (alive){
    priority = agility;
    (type == agile) ? priority*= 1.5 : 0;
    (type == tank) ? priority*= 0.8 : 0;
  }
  return priority;
}

static struct s_node *create_node(Character character) {
  struct s_node *new_node = NULL;
  float priority = calculate_priority(character);
  new_node = malloc(sizeof(struct s_node));
  assert(new_node != NULL);
  new_node->priority = priority;
  new_node->character = character;
  new_node->next = NULL;
  return new_node;
}

pqueue pqueue_enqueue(pqueue q, Character character) {
  /* PRE: {q --> pqueue} */
  assert(invrep(q));
  struct s_node *new_node = create_node(character);
  /* encolar, posiciono el nodo en la prioridad correspondiente y encolo antes */
  if (pqueue_is_empty(q)){
    q->front = new_node;
  } else if (new_node->priority >= q->front->priority){
    struct s_node *aux = q->front;
    q->front = new_node; new_node->next = aux;
  } else {
    struct s_node *aux = q->front;
    while (aux->next != NULL && aux->next->priority >= new_node->priority)aux = aux->next;
    if (aux->next == NULL) {
      aux->next = new_node;
    } else {
      new_node->next = aux->next;
      aux->next = new_node;
    }
  }
  q->size++;
  /* POS: {q --> pqueue && !pqueue_is_empty()} */
  assert(invrep(q) && !pqueue_is_empty(q));
  return q;
}

/* ============================================================================
IS EMPTY?
============================================================================ */

bool pqueue_is_empty(pqueue q) {
  /* PRE: {q --> pqueue} */
  assert(invrep(q));
  return q->front == NULL;
}

/* ============================================================================
PEEKS
============================================================================ */

Character pqueue_peek(pqueue q) {
  /* PRE: {q --> pqueue && !pqueue_is_empty(q)} */
  assert(invrep(q) && !pqueue_is_empty(q));
  Character res = q->front->character;
  /* POS: {q --> pqueue} */
  assert(invrep(q));
  return res;
}

float pqueue_peek_priority(pqueue q) {
  /* PRE: {q --> pqueue && !pqueue_is_empty(q)} */
  assert(invrep(q) && !pqueue_is_empty(q));
  float res = q->front->priority;
  /* POS: {q --> pqueue} */
  assert(invrep(q));
  return res;
}

/* ============================================================================
SIZE
============================================================================ */

unsigned int pqueue_size(pqueue q) {
  /* PRE: {q --> pqueue} */
  assert(invrep(q));
  unsigned int size = q->size;
  /* POS: {q --> pqueue} */
  return size;
}

/* ============================================================================
COPY
============================================================================ */

pqueue pqueue_copy(pqueue q) {
  /* PRE: {q --> pqueue} */
  assert(invrep(q));
  pqueue copy = pqueue_empty();
  /* ---------------------------------------------------- */
  if (!pqueue_is_empty(q)){
  struct s_node *aux = q->front;
   while(aux != NULL){
      Character copy_ea = character_copy(aux->character);
      copy = pqueue_enqueue(copy, copy_ea); 
      aux = aux->next;
   }
  } 
  /* ---------------------------------------------------- */
  copy->size = q->size;
  /* POS: {q --> pqueue && copy --> pqueue} */
  assert(invrep(q) & (q->size == copy->size));
  return copy;
} 



/* ============================================================================
DESTROY!
============================================================================ */
static struct s_node *destroy_node(struct s_node *node) {
  assert(node != NULL);
  node->character = character_destroy(node->character);
  free(node);
  node = NULL;
  assert(node == NULL);
  return node;
}

pqueue pqueue_dequeue(pqueue q) {
  /* PRE: {q --> pqueue && !pqueue_is_empty(q)} */
  assert(invrep(q) & !pqueue_is_empty(q));
  if (pqueue_is_empty(q)){
    return q;
  } else if (q->size == 1){
    q->front = destroy_node(q->front);
  } else {
    struct s_node *killme = q->front;
    q->front = q->front->next;
    killme = destroy_node(killme);
  }
  q->size--;
  /* POS: {q --> pqueue} */
  assert(invrep(q));
  return q;
}

pqueue pqueue_destroy(pqueue q) {
  assert(invrep(q));
  if(q->size > 0){
    while(!pqueue_is_empty(q)){
      q = pqueue_dequeue(q);
    }
  }
  free(q);
  return NULL;
}
