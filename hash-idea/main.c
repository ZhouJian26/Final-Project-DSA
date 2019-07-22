#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define modulo 100007
/*
    PROGETTO DI API
    NOTA: non esiste il delete di una relazione ma solo di nodi e edge

    ToDo List:
        - organizzazione info nodi
        - organizzazione info relazioni(edges)
    -> info nodi:
        - salvati in strutture dati, contenenti:
            - stringa nome, dinamica in base alla lunghezza della stringa
            - lista di *relazioni container*
    -> gestione nodi:
        - salvati in una hashtable, con una struttura dati del tipo:
            - puntatore a *info nodi*
            - puntatore al prossimo elemento della hashtable
    -> relazioni container:
        - struttura dati, contenente:
            - tipo relazione stringa
            - relazioni out(lista di *info relazioni*)
            - relazioni in(lista di *info relazioni*)
            - puntatore a *ranking node*
            - puntatore al prossimo 
    -> info relazioni:
        - struttura dati, contentente:
            - sorgente stringa
            - next *info relazioni* sorgente
            - destinazione stringa
            - next *info relazioni* destinazione
    -> gestione edge:
        - salvati in una hashtable, una per tipo relazione, con una struttura dati del tipo:
            - puntatore a *info relazioni*
            - puntatore al prossimo elemento della hashtable
    -> ranking node
        - struttura dati, contenete:
            - puntatore a *info nodo*
            - puntatore al prossimo elemento della lista
            - puntatore al precedente elemento della lista
            - puntatore al *floor ranking*
    -> floor ranking
        - struttura dati, contenete:
            - count(indica il numero degli edge entranti dei nodi del piano)
            - flag isSorted, HINT: prelevo stringa e puntatore a ranking node e creo una lista da sortare 
                e poi ri-collego la lista
            - puntatore al next *floor ranking*
            - puntatore al prev *floor ranking*
            - puntatore a *container floor's ranking*
            - lista di *ranking node*
    -> container floor's ranking 
        - struttura dati, contenente:
            - nome relazione stringa
            - puntatore al max *floor ranking*
            - puntatore al min *floor ranking*
            - puntatore al *docker ranking*
    -> docker ranking
        - puntatore a *container floor's ranking*
        - puntatore al prossimo elemento della lista
        - puntatore al precedente elemento della lista
    -> flag isDockerSorted
 */
typedef struct info_entity
{
    char *entity_name;
    struct container_relation *container_relation_head_p;
} info_entity_t;
typedef struct container_relation
{
    char *rel_name;
    struct info_relation *out_relation_list_head_p;
    struct info_relation *in_relation_list_head_p;
    struct info_entity *entity_p;
    struct container_relation *next_p;
    struct container_relation *prev_p;
    struct ranking_node *ranking_node_p;
} container_relation_t;
typedef struct info_relation
{
    char *src;
    char *dest;
    struct container_relation *src_container_relation_p;
    struct container_relation *dest_container_relation_p;
    struct info_relation *src_next_p;
    struct info_relation *src_prev_p;
    struct info_relation *dest_next_p;
    struct info_relation *dest_prev_p;
    struct info_relation_menager *info_relation_menager_p;
} info_relation_t;

typedef struct info_entity_menager
{
    struct info_entity *entity_p;
    struct info_entity_menager *next_p;
} info_entity_menager_t;
typedef struct info_relation_menager
{
    struct info_relation *info_relation_p;
    struct info_relation_menager *next_p;
    struct info_relation_menager *prev_p;
} info_relation_menager_t;
typedef struct ranking_node
{
    struct info_entity *entity_p;
    struct ranking_node *next_p;
    struct ranking_node *prev_p;
    struct floor_ranking *floor_ranking_p;
} ranking_node_t;
typedef struct floor_ranking
{
    unsigned long int count;
    char flag;
    struct floor_ranking *next_p;
    struct floor_ranking *prev_p;
    struct ranking_node *ranking_node_head_p;
    struct container_floor_ranking *container_floor_ranking_p;
} floor_ranking_t;
typedef struct container_floor_ranking
{
    char *rel_name;
    struct floor_ranking *max_floor_ranking_p;
    struct floor_ranking *min_floor_ranking_p;
    struct docker_ranking *docker_ranking_p;
} container_floor_ranking_t;
typedef struct docker_ranking
{
    struct info_relation_menager_t *hashtable[modulo];
    struct container_floor_ranking *container_floor_ranking_p;
    struct docker_ranking *next_p;
    struct docker_ranking *prev_p;
} docker_ranking_t;
unsigned long int hash(char *str)
{
    //DONE
    unsigned long int hash = 5281;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c;
    return hash;
}
void add_entity(char *str, info_entity_menager_t *hashtable[])
{
    //***   DONE!   ***
    // creo le cose e lo aggiungo alle entità
    //calcolo hashvalue
    int value = hash(str) / modulo;
    int trovato = 0;
    info_entity_menager_t *temp_iem = hashtable[value];
    //controllo se esiste l'elemento
    while (temp_iem != NULL)
    {
        if (strcmp(str, temp_iem->entity_p->entity_name))
        {
            //esiste già
            trovato++;
            break;
        }
        temp_iem = temp_iem->next_p;
    }
    if (trovato == 0)
    {
        //se non esiste, creo
        info_entity_menager_t *new_entity_info_menager_p = malloc(sizeof(info_entity_menager_t));
        info_entity_t *new_entity_info_p = malloc(sizeof(info_entity_t));
        new_entity_info_p->container_relation_head_p = NULL;
        new_entity_info_p->entity_name = malloc(sizeof(char) * strlen(str));
        strcpy(new_entity_info_p->entity_name, str);
        new_entity_info_menager_p->entity_p = new_entity_info_p;
        new_entity_info_menager_p->next_p = hashtable[value];
        hashtable[value] = new_entity_info_menager_p;
    }
}
void delete_entity(char *str, info_entity_menager_t *hashtable[], docker_ranking_t *docker_ranking_head_p)
{
    //cerco entità, se esite elimino altrimenti nulla
    //calcolo hashvalue
    int value = hash(str) / modulo;
    int trovato = 0;
    info_entity_menager_t *temp_iem = hashtable[value];
    //controllo se esiste l'elemento
    while (temp_iem != NULL)
    {
        if (strcmp(str, temp_iem->entity_p->entity_name))
        {
            //esiste, procedo a cancellare
        }
        temp_iem = temp_iem->next_p;
    }
}
void delete_relation(char *src_str, char *dest_str, char *rel_str, docker_ranking_t *docker_ranking_head_p)
{
    //si passa direttamente la docker giusta
    char *all_in_str = malloc(sizeof(char) * (strlen(src_str) + strlen(dest_str)));
    strcpy(all_in_str, src_str);
    strcat(all_in_str, dest_str);
    //calcolo hashvalue
    int value = hash(all_in_str) / modulo;
    int trovato = 0;
    info_relation_menager_t *temp_iem = docker_ranking_head_p->hashtable[value];
    while (temp_iem != NULL)
    {
        if (strcmp(src_str, temp_iem->info_relation_p->src) && strcmp(dest_str, temp_iem->info_relation_p->dest))
        {
            //esiste, procedo a cancellare
        }
        temp_iem = temp_iem->next_p;
    }
}
void add_relation(char *src_str, char *dest_str, char *rel_str, info_relation_menager_t *hashtable[], docker_ranking_t *docker_ranking_head_p)
{
    //si passa direttamente la docker giusta
    char *all_in_str = malloc(sizeof(char) * (strlen(src_str) + strlen(dest_str)));
    strcpy(all_in_str, src_str);
    strcat(all_in_str, dest_str);
    //calcolo hashvalue
    int relation_hash_value = hash(all_in_str) / modulo;
    int src_hash_value = hash(src_str) / modulo;
    int dest_hash_value = hash(dest_str) / modulo;
}
void report() {}
//todo fare funzione per ricerca docker
int main()
{
    //TODO
}
