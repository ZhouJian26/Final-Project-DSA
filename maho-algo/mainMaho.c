#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define modulo 10007
#define modulo_rel 107

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
    //serve un puntatore al docker?
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
    struct floor_ranking *max_floor_ranking_p;
    struct floor_ranking *min_floor_ranking_p;
    struct docker_ranking *docker_ranking_p;
} container_floor_ranking_t;
typedef struct docker_ranking
{
    char *rel_name;
    struct info_relation_menager *hashtable[modulo_rel];
    struct container_floor_ranking *container_floor_ranking_p;
    struct docker_ranking *next_p;
    struct docker_ranking *prev_p;
} docker_ranking_t;
docker_ranking_t *docker_search(char *str, docker_ranking_t *docker_ranking_head_p);
void sys_call_print_entity_hashtable(info_entity_menager_t *hashtable[]);
void ranking_node_scala(container_relation_t *target, int move);
void delete_ranking_node(ranking_node_t *target);
/*
    *** Status: Completed **
    Dato una stringa restituisce un intero.
 */
unsigned long int hash(char *str)
{
    //DONE
    unsigned long int hash = 5281;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}
/*
    *** Status: Completed **
    Aggiunge una entità al Sistema di Monitoraggio.
    - str è il nome dell'entità
    - hashtable è la hashtable delle entità
*/
void add_entity(char *str, info_entity_menager_t *hashtable[])
{
    //***   DONE!   ***
    // creo le cose e lo aggiungo alle entità
    //calcolo hashvalue

    //printf("Traking add_entity\n");
    int value = hash(str) % modulo;
    info_entity_menager_t *temp_iem = hashtable[value];
    //controllo se esiste l'elemento
    while (temp_iem != NULL)
    {
        if (strcmp(str, (temp_iem->entity_p)->entity_name) == 0)
            break;
        temp_iem = temp_iem->next_p;
    }
    if (temp_iem == NULL)
    {
        //se non esiste, creo
        info_entity_menager_t *new_entity_info_menager_p = malloc(sizeof(info_entity_menager_t));
        info_entity_t *new_entity_info_p = malloc(sizeof(info_entity_t));
        new_entity_info_p->container_relation_head_p = NULL;
        new_entity_info_p->entity_name = malloc(sizeof(char) * (strlen(str) + 1));
        strcpy(new_entity_info_p->entity_name, str);
        new_entity_info_menager_p->entity_p = new_entity_info_p;
        new_entity_info_menager_p->next_p = hashtable[value];
        hashtable[value] = new_entity_info_menager_p;
    }
    //printf("Traking add_entity end\n");
}
/*
    Funzione Gestita per aggiungere una entità
    - puntatore costante per prendere l'input
    - puntatore ad hashtable nodi
*/
void sys_call_add_entity(char *str, info_entity_menager_t *hashtable[])
{

    //printf("Traking sys_call_add_entity\n");
    scanf("%s", str);
    add_entity(str, hashtable); // V 1.0.0
    //printf("Traking sys_call_add_entity End\n");
}
/*
    Funzione Gestita per stampare i dati dei nodiin hashtable
*/
void sys_call_print_entity_hashtable(info_entity_menager_t *hashtable[])
{
    int i = 0;
    info_entity_menager_t *temp_iem;
    printf("Print Entity:\n");
    while (i < modulo)
    {
        temp_iem = hashtable[i];
        while (temp_iem != NULL)
        {
            printf("Nome Entità: %s\n", temp_iem->entity_p->entity_name);
            /* 
            printf("Indirizzo prossimo in hashtable nodi: %p\n", temp_iem->next_p);
            printf("Indirizzo head container relazioni: %p\n", temp_iem->entity_p->container_relation_head_p);
            printf("Indirizzo backtrack a hashtable nodi: %p\n", temp_iem->entity_p->info_entity_menager_back_p);
            */
            container_relation_t *temp_cr = temp_iem->entity_p->container_relation_head_p;
            printf("Stampo Container Relazioni:\n");
            if (temp_cr == NULL)
                printf("    Nessun Container Relazioni...\n");
            while (temp_cr != NULL)
            {

                printf("    Indirizzo prev in lista: %p\n", temp_cr->prev_p);
                printf("    Indirizzo next in lista: %p\n", temp_cr->next_p);
                printf("    Nome relazione: %s\n", temp_cr->rel_name);
                /*
                printf("Indirizzo ranking node: %p\n", temp_cr->ranking_node_p);
                printf("Indirizzo in relation: %p\n", temp_cr->in_relation_list_head_p);
                printf("Indirizzo out relation: %p\n", temp_cr->out_relation_list_head_p);
                */
                if (temp_cr->ranking_node_p != NULL)
                    printf("        Rank: %lu\n", temp_cr->ranking_node_p->floor_ranking_p->count);
                else
                    printf("        Rank: -1\n");
                info_relation_t *temp_ir = temp_cr->in_relation_list_head_p;
                printf("        Stampo relazioni in:\n");
                while (temp_ir != NULL)
                {
                    printf("            %s => %s\n", temp_ir->src, temp_ir->dest);
                    temp_ir = temp_ir->dest_next_p;
                }
                temp_ir = temp_cr->out_relation_list_head_p;
                printf("        Stampo relazioni out:\n");
                while (temp_ir != NULL)
                {
                    printf("            %s => %s\n", temp_ir->src, temp_ir->dest);
                    temp_ir = temp_ir->src_next_p;
                }
                temp_cr = temp_cr->next_p;
            }
            printf("Fine\n");
            temp_iem = temp_iem->next_p;
        }
        i++;
    }
}
/*
    Funzione Gestita: cancella una entità e le referenze. NON deferenzia da hashtable entità.
*/
docker_ranking_t *delete_entity(info_entity_menager_t *entity_to_delete_info_menager, docker_ranking_t *docker_ranking_head_p)
{

    //printf("Traking delete_entity\n");
    info_entity_t *to_delete_info_entity = entity_to_delete_info_menager->entity_p;
    container_relation_t *to_free_dcr, *to_delete_container_relation = to_delete_info_entity->container_relation_head_p;
    info_relation_t *to_delete_info_relation = NULL, *to_free_ir;
    docker_ranking_t *target_docker_ranking = NULL, *head_to_return = docker_ranking_head_p;
    //libero i container dell'entità
    while (to_delete_container_relation != NULL)
    {
        // Libero i container
        // Delete in relation
        //deferenzio il nodo relazione e lo cancello
        target_docker_ranking = docker_search(to_delete_container_relation->rel_name, head_to_return);
        to_delete_info_relation = to_delete_container_relation->in_relation_list_head_p;
        //cancello le relazioni in entrata
        while (to_delete_info_relation != NULL)
        {
            //NON devo deferenziare i puntatori di dest
            //deferenzio & delete info relation menager
            //todo delete hash per relazioni
            //delete_relation_menager_fromDocker(to_delete_info_relation->info_relation_menager_p, target_docker_ranking);
            // deferenzio i puntatori del src e gestisco varie cose

            if (to_delete_info_relation->src_prev_p != NULL)
            {
                (to_delete_info_relation->src_prev_p)->src_next_p = to_delete_info_relation->src_next_p;
                if (to_delete_info_relation->src_next_p != NULL)
                    (to_delete_info_relation->src_next_p)->src_prev_p = to_delete_info_relation->src_prev_p;
            }
            else
            {
                (to_delete_info_relation->src_container_relation_p)->out_relation_list_head_p = to_delete_info_relation->src_next_p;
                if (to_delete_info_relation->src_next_p != NULL)
                    (to_delete_info_relation->src_next_p)->src_prev_p = NULL;
                else
                {
                    if ((to_delete_info_relation->src_container_relation_p)->in_relation_list_head_p == NULL)
                    {
                        //Cancello il container della sorgente essendo vuota in/out
                        //Deferenzio il container dal master
                        if ((to_delete_info_relation->src_container_relation_p)->prev_p != NULL)
                            ((to_delete_info_relation->src_container_relation_p)->prev_p)->next_p = (to_delete_info_relation->src_container_relation_p)->next_p;
                        else
                            ((to_delete_info_relation->src_container_relation_p)->entity_p)->container_relation_head_p = (to_delete_info_relation->src_container_relation_p)->next_p;

                        if ((to_delete_info_relation->src_container_relation_p)->next_p != NULL)
                            ((to_delete_info_relation->src_container_relation_p)->next_p)->prev_p = (to_delete_info_relation->src_container_relation_p)->prev_p;
                        //Cancello il container
                        free((to_delete_info_relation->src_container_relation_p)->rel_name);
                        free(to_delete_info_relation->src_container_relation_p);
                    }
                }
            }
            to_free_ir = to_delete_info_relation;
            to_delete_info_relation = to_delete_info_relation->dest_next_p;
            free(to_free_ir->dest);
            free(to_free_ir->src);
            free(to_free_ir);
        }
        //Delete out relations
        to_delete_info_relation = to_delete_container_relation->out_relation_list_head_p;
        while (to_delete_info_relation != NULL)
        {
            //NON devo aggiornare i puntatori di src
            //deferenzio & delete info relation menager

            //todo delete hash per relazioni
            //delete_relation_menager_fromDocker(to_delete_info_relation->info_relation_menager_p, target_docker_ranking);
            // deferenzio i puntatori del dest e gestisco varie cose
            ranking_node_scala(to_delete_info_relation->dest_container_relation_p, 0);
            //se zero info devo settare a null il ranking
            if (to_delete_info_relation->dest_prev_p != NULL)
            {
                (to_delete_info_relation->dest_prev_p)->dest_next_p = to_delete_info_relation->dest_next_p;
                if (to_delete_info_relation->dest_next_p != NULL)
                    (to_delete_info_relation->dest_next_p)->dest_prev_p = to_delete_info_relation->dest_prev_p;
            }
            else
            {
                (to_delete_info_relation->dest_container_relation_p)->in_relation_list_head_p = to_delete_info_relation->dest_next_p;
                if (to_delete_info_relation->dest_next_p != NULL)
                    (to_delete_info_relation->dest_next_p)->dest_prev_p = NULL;
                else
                {
                    //l'entità destinazione della relazione non ha più edge in entrata
                    (to_delete_info_relation->dest_container_relation_p)->ranking_node_p = NULL;
                    if ((to_delete_info_relation->dest_container_relation_p)->out_relation_list_head_p == NULL)
                    {
                        //Cancello il container della destinazione essendo vuota in/out
                        //Deferenzio il container dal master
                        if ((to_delete_info_relation->dest_container_relation_p)->prev_p != NULL)
                            ((to_delete_info_relation->dest_container_relation_p)->prev_p)->next_p = (to_delete_info_relation->dest_container_relation_p)->next_p;
                        else
                            ((to_delete_info_relation->dest_container_relation_p)->entity_p)->container_relation_head_p = (to_delete_info_relation->dest_container_relation_p)->next_p;

                        if ((to_delete_info_relation->dest_container_relation_p)->next_p != NULL)
                            ((to_delete_info_relation->dest_container_relation_p)->next_p)->prev_p = (to_delete_info_relation->dest_container_relation_p)->prev_p;
                        //Cancello il container
                        free((to_delete_info_relation->dest_container_relation_p)->rel_name);
                        free(to_delete_info_relation->dest_container_relation_p);
                    }
                }
            }
            to_free_ir = to_delete_info_relation;
            to_delete_info_relation = to_delete_info_relation->src_next_p;
            free(to_free_ir->dest);
            free(to_free_ir->src);
            free(to_free_ir);
        }
        //Delete ranking node
        if (to_delete_container_relation->ranking_node_p != NULL)
            delete_ranking_node(to_delete_container_relation->ranking_node_p); //rn gia freerato
        if ((target_docker_ranking->container_floor_ranking_p)->max_floor_ranking_p == NULL)
        {
            //cancello il docker
            //deferenzio il docker
            if (target_docker_ranking->prev_p != NULL)
                (target_docker_ranking->prev_p)->next_p = target_docker_ranking->next_p;
            else
                head_to_return = target_docker_ranking->next_p;

            if (target_docker_ranking->next_p != NULL)
                (target_docker_ranking->next_p)->prev_p = target_docker_ranking->prev_p;
            //cancello il docker
            free(target_docker_ranking->rel_name);
            free(target_docker_ranking->container_floor_ranking_p);
            free(target_docker_ranking);
        }
        to_free_dcr = to_delete_container_relation;
        to_delete_container_relation = to_delete_container_relation->next_p;
        free(to_free_dcr->rel_name);
        free(to_free_dcr);
    }
    free(to_delete_info_entity->entity_name);
    free(entity_to_delete_info_menager); //libero nodo della hashtable, è stato deferenziato?
    free(to_delete_info_entity);         //libero nodo dell'info

    //printf("Traking delete_entity end\n");
    return head_to_return;
}
/*
    Funzione Gestita: legge l'entità da cancellare e la cancella gestendo tutte le relative referenze.
    Restituisce l'head del docker.
*/
docker_ranking_t *sys_call_delete_entity(char *str, info_entity_menager_t *hashtable[], docker_ranking_t *docker_ranking_head_p)
{
    //printf("Traking sys_call_delete_entity\n");
    scanf("%s", str);
    //cerco entità, se esite elimino altrimenti nulla
    //calcolo hashvalue dell'entità da cancellare
    int value = hash(str) % modulo;
    docker_ranking_t *head_docker_update = docker_ranking_head_p;
    info_entity_menager_t *temp_iem = hashtable[value], *prev_iem = NULL;
    while (temp_iem != NULL)
    {
        if (strcmp(str, (temp_iem->entity_p)->entity_name) == 0)
        {
            //deferenzio da hashtable entità
            if (prev_iem != NULL)
                prev_iem->next_p = temp_iem->next_p;
            else
                hashtable[value] = temp_iem->next_p;
            head_docker_update = delete_entity(temp_iem, docker_ranking_head_p);
            break;
        }
        prev_iem = temp_iem;
        temp_iem = temp_iem->next_p;
    }
    //printf("Traking sys_call_delete_entity end\n");
    return head_docker_update;
}
/*
    *** Status: Complete[?] **
    Cancella le referenze del target dalla hashtable(relazioni)
    - value : index del target in hashtable relazioni
    - target : nodo da deferenziare dalla hashtable
    - docker_ranking_head_p : docker relativo alla relazione
 */
void delete_info_relation_menager(int value, info_relation_menager_t *target, docker_ranking_t *docker_ranking_head_p)
{
    //Tolgo le referenze del container/nodo dalla hashtable

    //printf("Traking delete_info_relation_menager\n");
    if (target->prev_p == NULL)
        docker_ranking_head_p->hashtable[value] = target->next_p;
    else
        (target->prev_p)->next_p = target->next_p;
    if (target->next_p != NULL)
        (target->next_p)->prev_p = target->prev_p;
    //printf("Traking delete_info_relation_menager end\n");
}
/*
    *** Status: Complete[?] **
    Cancella le referenze del target dalle liste di relazioni in e out
    - target : nodo da deferenziare
 */
void delete_info_relation(info_relation_t *target)
{
    //** Sistemo i puntatori della lista di src **

    //printf("Traking delete_info_relation\n");
    if (target->src_prev_p == NULL)
        (target->src_container_relation_p)->out_relation_list_head_p = target->src_next_p;
    else
        (target->src_prev_p)->src_next_p = target->src_next_p;

    if (target->src_next_p != NULL)
        (target->src_next_p)->src_prev_p = target->src_prev_p;

    //** Sistemo i puntatori della lista di dest **
    if (target->dest_prev_p == NULL)
        (target->dest_container_relation_p)->in_relation_list_head_p = target->dest_next_p;
    else
        (target->dest_prev_p)->dest_next_p = target->dest_next_p;

    if (target->dest_next_p != NULL)
        (target->dest_next_p)->dest_prev_p = target->dest_prev_p;

    //printf("Traking delete_info_relation end\n");
}
/*
    *** Status: Completed **
    Aggiunge un ranking-node al docker-ranking
    - target : ranking-node da aggiungere
    - docker_ranking_p : docker target in cui aggiungere
    Gestisce la creazione di ranking-floor se necessari
 */
void add_ranking_node(ranking_node_t *target, docker_ranking_t *docker_ranking_p)
{

    //printf("Traking add_ranking_node\n");
    //Si presumo il docker corretto in cui aggiungere
    container_floor_ranking_t *target_container_floor_ranking = docker_ranking_p->container_floor_ranking_p;

    if ((docker_ranking_p->container_floor_ranking_p)->min_floor_ranking_p == NULL)
    {
        floor_ranking_t *new_floor_ranking = malloc(sizeof(floor_ranking_t));
        new_floor_ranking->count = 1;
        new_floor_ranking->flag = 's';
        new_floor_ranking->container_floor_ranking_p = target_container_floor_ranking;
        new_floor_ranking->prev_p = NULL;
        new_floor_ranking->next_p = NULL;
        new_floor_ranking->ranking_node_head_p = target;
        target_container_floor_ranking->min_floor_ranking_p = new_floor_ranking;
        target_container_floor_ranking->max_floor_ranking_p = new_floor_ranking;
        target->floor_ranking_p = new_floor_ranking;
        target->next_p = NULL;
        target->prev_p = NULL;
    }
    else
    {
        if ((target_container_floor_ranking->min_floor_ranking_p)->count == 1)
        {
            floor_ranking_t *target_floor_ranking = target_container_floor_ranking->min_floor_ranking_p;
            target_floor_ranking->flag = 'd';
            (target_floor_ranking->ranking_node_head_p)->prev_p = target;
            target->next_p = target_floor_ranking->ranking_node_head_p;
            target->prev_p = NULL;
            target->floor_ranking_p = target_floor_ranking;
            target_floor_ranking->ranking_node_head_p = target;
        }
        else
        {
            floor_ranking_t *new_floor_ranking = malloc(sizeof(floor_ranking_t));
            new_floor_ranking->count = 1;
            new_floor_ranking->flag = 's';
            new_floor_ranking->container_floor_ranking_p = target_container_floor_ranking;
            new_floor_ranking->prev_p = NULL;
            new_floor_ranking->next_p = target_container_floor_ranking->min_floor_ranking_p;
            (new_floor_ranking->next_p)->prev_p = new_floor_ranking;
            new_floor_ranking->ranking_node_head_p = target;
            target_container_floor_ranking->min_floor_ranking_p = new_floor_ranking;
            target->floor_ranking_p = new_floor_ranking;
            target->next_p = NULL;
            target->prev_p = NULL;
        }
    }

    //printf("Traking add_ranking_node end\n");
}
/*
    *** Status: Completed **
    Restituisce ranking_node_t creato, aggiungendo l'entita nel sistema di tracking di rank
    - info_entity_p : puntatore all'entità
    - rel_str : nome della relazione da tracciare per l'entità
    - docker_ranking_head_p : puntatore all'head dei docker
 */
docker_ranking_t *sys_create_ranking_node(ranking_node_t *rn_to_add, char *rel_str, docker_ranking_t *docker_ranking_head_p)
{

    //printf("Traking sys_create_ranking_node\n");
    docker_ranking_t *to_pass = docker_search(rel_str, docker_ranking_head_p), *to_res = docker_ranking_head_p;
    if (to_pass == NULL)
    {
        // Non esiste il docker
        to_pass = malloc(sizeof(docker_ranking_t));
        to_pass->rel_name = malloc(sizeof(char) * (strlen(rel_str) + 1));
        strcpy(to_pass->rel_name, rel_str);
        to_pass->prev_p = NULL;
        to_pass->next_p = docker_ranking_head_p;
        if (docker_ranking_head_p != NULL)
            docker_ranking_head_p->prev_p = to_pass;
        to_pass->container_floor_ranking_p = malloc(sizeof(container_floor_ranking_t));
        (to_pass->container_floor_ranking_p)->docker_ranking_p = to_pass;
        (to_pass->container_floor_ranking_p)->max_floor_ranking_p = NULL;
        (to_pass->container_floor_ranking_p)->min_floor_ranking_p = NULL;
        for (int i = 0; i < modulo_rel; i++)
            to_pass->hashtable[i] = NULL;
        to_res = to_pass;
    }
    add_ranking_node(rn_to_add, to_pass);
    //printf("Traking sys_create_ranking_node End\n");
    return to_res;
}
/*
    *** Status: Completed **
    Toglie dal Sistema di Monitoraggio Rank il target
    - target : il nodo da non monitorare in rank
    NOTA : non gestisce il docker, quindi va gestito. Gestisce l'eliminazione del piano se necessario
 */
void delete_ranking_node(ranking_node_t *target)
{
    //se return == 1 allora va eliminato il docker

    //printf("Traking delete_ranking_node\n");
    // Deferenzio il target
    if (target->prev_p == NULL)
        (target->floor_ranking_p)->ranking_node_head_p = target->next_p;
    else
        (target->prev_p)->next_p = target->next_p;
    if (target->next_p != NULL)
        (target->next_p)->prev_p = target->prev_p;
    // controllo piano dislocato
    if ((target->floor_ranking_p)->ranking_node_head_p == NULL)
    {
        //deferenzio il piano
        floor_ranking_t *target_floot_ranking = target->floor_ranking_p;
        if (target_floot_ranking->next_p == NULL)
        {
            if (target_floot_ranking->prev_p == NULL)
            {
                //non esiste nessuno dei 2
                (target_floot_ranking->container_floor_ranking_p)->max_floor_ranking_p = NULL;
                (target_floot_ranking->container_floor_ranking_p)->min_floor_ranking_p = NULL;
            }
            else
            {
                //esiste solo il prev
                (target_floot_ranking->prev_p)->next_p = NULL;
                (target_floot_ranking->container_floor_ranking_p)->max_floor_ranking_p = target_floot_ranking->prev_p;
            }
        }
        else
        {
            if (target_floot_ranking->prev_p == NULL)
            {
                //esiste solo il next
                (target_floot_ranking->next_p)->prev_p = NULL;
                (target_floot_ranking->container_floor_ranking_p)->min_floor_ranking_p = target_floot_ranking->next_p;
            }
            else
            {
                //esistono entrambi
                (target_floot_ranking->next_p)->prev_p = target_floot_ranking->prev_p;
                (target_floot_ranking->prev_p)->next_p = target_floot_ranking->next_p;
            }
        }
        free(target_floot_ranking);
    }
    free(target);
    //printf("Traking delete_ranking_node end\n");
}

/*
    *** Status: Completed **
    Gestisce il rank del nodo target
    - target : rank_node da gestire
    - move : movimento da compiere
    * 1 => +1 in rank
    * 0 => -1 in rank
    Gestisce la creazione dei ranking_floor se necessario, e cancella se necessario anche il ranking_node nel caso non avesse più relazioni
*/
void ranking_node_scala(container_relation_t *target_cr, int move)
{

    //printf("Traking ranking_node_scala\n");
    ranking_node_t *target = target_cr->ranking_node_p;
    /* Sistemo i puntatori della lista del floor_ranking */
    if (target->prev_p == NULL)
        (target->floor_ranking_p)->ranking_node_head_p = target->next_p;
    else
        (target->prev_p)->next_p = target->next_p;
    if (target->next_p != NULL)
        (target->next_p)->prev_p = target->prev_p;
    /*  */
    if (move == 1)
    {
        //printf("Up move\n");
        //up move
        floor_ranking_t *target_floor = target->floor_ranking_p;
        if (target_floor->next_p == NULL)
        {
            //creo nuovo piano
            floor_ranking_t *new_floor_ranking = malloc(sizeof(floor_ranking_t));
            new_floor_ranking->count = target_floor->count + 1;
            new_floor_ranking->flag = 's'; //sorted
            new_floor_ranking->next_p = NULL;
            new_floor_ranking->prev_p = target_floor;
            new_floor_ranking->ranking_node_head_p = target;
            new_floor_ranking->container_floor_ranking_p = target_floor->container_floor_ranking_p;
            target->floor_ranking_p = new_floor_ranking;
            target->next_p = NULL;
            target->prev_p = NULL;
            target_floor->next_p = new_floor_ranking;
            (new_floor_ranking->container_floor_ranking_p)->max_floor_ranking_p = new_floor_ranking;
        }
        else
        {
            if (((target->floor_ranking_p)->next_p)->count == (target->floor_ranking_p)->count + 1)
            {
                floor_ranking_t *target_floor_toin = (target->floor_ranking_p)->next_p;
                target->prev_p = NULL;
                target->next_p = target_floor_toin->ranking_node_head_p;
                (target_floor_toin->ranking_node_head_p)->prev_p = target;
                target->floor_ranking_p = target_floor_toin;
                target_floor_toin->flag = 'd'; //dirty
                target_floor_toin->ranking_node_head_p = target;
            }
            else
            {
                floor_ranking_t *new_floor_ranking = malloc(sizeof(floor_ranking_t));
                new_floor_ranking->count = (target->floor_ranking_p)->count + 1;
                new_floor_ranking->flag = 's'; //sorted
                new_floor_ranking->next_p = target_floor->next_p;
                new_floor_ranking->prev_p = target_floor;
                new_floor_ranking->ranking_node_head_p = target;
                new_floor_ranking->container_floor_ranking_p = target_floor->container_floor_ranking_p;
                target_floor->next_p = new_floor_ranking;
                (new_floor_ranking->next_p)->prev_p = new_floor_ranking;
                target->floor_ranking_p = new_floor_ranking;
                target->next_p = NULL;
                target->prev_p = NULL;
            }
        }
        if (((target->floor_ranking_p)->prev_p)->ranking_node_head_p == NULL)
        {
            //floor ranking vuoto, da cancellare
            floor_ranking_t *to_delete_floor_ranking = (target->floor_ranking_p)->prev_p;
            (target->floor_ranking_p)->prev_p = to_delete_floor_ranking->prev_p;
            if ((to_delete_floor_ranking)->prev_p != NULL)
                ((to_delete_floor_ranking)->prev_p)->next_p = (to_delete_floor_ranking)->next_p;
            else
                ((to_delete_floor_ranking)->container_floor_ranking_p)->min_floor_ranking_p = target->floor_ranking_p;
            free(to_delete_floor_ranking);
        }
    }
    else
    {

        //printf("Down move\n");
        //down
        floor_ranking_t *target_floor = target->floor_ranking_p;
        if (target_floor->prev_p == NULL)
        {
            if (target_floor->count != 1)
            {
                floor_ranking_t *new_floor_ranking = malloc(sizeof(floor_ranking_t));
                new_floor_ranking->count = target_floor->count - 1;
                new_floor_ranking->container_floor_ranking_p = target_floor->container_floor_ranking_p;
                new_floor_ranking->flag = 's';
                new_floor_ranking->next_p = target_floor;
                new_floor_ranking->prev_p = NULL;
                new_floor_ranking->ranking_node_head_p = target;
                target->floor_ranking_p = new_floor_ranking;
                target->next_p = NULL;
                target->prev_p = NULL;
                (new_floor_ranking->container_floor_ranking_p)->min_floor_ranking_p = new_floor_ranking;
                target_floor->prev_p = new_floor_ranking;
            }
            else
            {
                //ranking node da eliminare
                if ((target->floor_ranking_p)->ranking_node_head_p == NULL)
                {
                    //floor ranking dislocato vuoto, cancello
                    floor_ranking_t *to_delete_floor_ranking = target->floor_ranking_p;
                    (to_delete_floor_ranking->container_floor_ranking_p)->min_floor_ranking_p = to_delete_floor_ranking->next_p;
                    if (to_delete_floor_ranking->next_p != NULL)
                        (to_delete_floor_ranking->next_p)->prev_p = NULL;
                    else
                        (to_delete_floor_ranking->container_floor_ranking_p)->max_floor_ranking_p = NULL;

                    free(to_delete_floor_ranking);
                }
                target_cr->ranking_node_p = NULL;
                free(target);
                target = NULL;
            }
        }
        else
        {
            if ((target_floor->prev_p)->count == target_floor->count - 1)
            {
                //esiste allora switcho
                floor_ranking_t *dest_floor_ranking = target_floor->prev_p;
                target->floor_ranking_p = dest_floor_ranking;
                target->next_p = dest_floor_ranking->ranking_node_head_p;
                target->prev_p = NULL;
                if (dest_floor_ranking->ranking_node_head_p != NULL)
                    (dest_floor_ranking->ranking_node_head_p)->prev_p = target;
                dest_floor_ranking->flag = 'd';
                dest_floor_ranking->ranking_node_head_p = target;
            }
            else
            {
                floor_ranking_t *new_floor_ranking = malloc(sizeof(floor_ranking_t));
                new_floor_ranking->count = target_floor->count - 1;
                new_floor_ranking->container_floor_ranking_p = target_floor->container_floor_ranking_p;
                new_floor_ranking->flag = 's';
                new_floor_ranking->next_p = target_floor;
                new_floor_ranking->prev_p = target_floor->prev_p;
                new_floor_ranking->ranking_node_head_p = target;
                target_floor->prev_p = new_floor_ranking;
                (new_floor_ranking->prev_p)->next_p = new_floor_ranking;
                target->floor_ranking_p = new_floor_ranking;
                target->next_p = NULL;
                target->prev_p = NULL;
            }
        }
        if (target != NULL && ((target->floor_ranking_p)->next_p)->ranking_node_head_p == NULL)
        {
            //floor ranking dislocato vuoto, cancello
            floor_ranking_t *to_delete_floor_ranking = (target->floor_ranking_p)->next_p;
            (target->floor_ranking_p)->next_p = to_delete_floor_ranking->next_p;
            if (to_delete_floor_ranking->next_p != NULL)
                (to_delete_floor_ranking->next_p)->prev_p = to_delete_floor_ranking->prev_p;
            else //sto cancellando un massimo
                (to_delete_floor_ranking->container_floor_ranking_p)->max_floor_ranking_p = target->floor_ranking_p;
            free(to_delete_floor_ranking);
        }
    }
    //printf("Traking ranking_node_scala end\n");
}

docker_ranking_t *delete_relation_maho(char *src_str, char *dest_str, char *rel_str, docker_ranking_t *docker_ranking_head_p, info_entity_menager_t *hashtable_entity[])
{
    int src_hash = hash(src_str) % modulo, dest_hash = hash(dest_str) % modulo;
    info_entity_menager_t *src_iem = hashtable_entity[src_hash], *dest_iem = hashtable_entity[dest_hash];
    if (src_iem != NULL && dest_iem != NULL)
    {
        //potenzialmente esistono entrambi
        docker_ranking_t *docker_update = docker_ranking_head_p;
        docker_ranking_head_p = docker_search(rel_str, docker_ranking_head_p);

        container_relation_t *temp_cr = src_iem->entity_p->container_relation_head_p;
        while (temp_cr != NULL && strcmp(temp_cr->rel_name, rel_str) != 0)
            temp_cr = temp_cr->next_p;
        if (temp_cr != NULL)
        {
            //esiste il contenitore in src
            info_relation_t *temp_ir = temp_cr->out_relation_list_head_p;
            while (temp_ir != NULL && strcmp(temp_ir->dest, dest_str) != 0)
                temp_ir = temp_ir->src_next_p;
            if (temp_ir != NULL)
            {
                //procedo ad eliminare***

                //trovato!
                //Procedo a cancellare la relazione
                //ho il puntatore a *info_relation_menager
                info_relation_t *temp_r = temp_ir;
                //** Deferenzio da lista in hashtable relazioni**
                //delete_info_relation_menager(value, temp_iem, docker_ranking_head_p);
                //** Deferenzio da src & dest la relazione dai rispettivi container**
                delete_info_relation(temp_r);
                //Gestione ranking node per il dest
                ranking_node_scala(temp_r->dest_container_relation_p, 0);
                if (temp_r->src_container_relation_p != temp_r->dest_container_relation_p)
                    if ((temp_r->dest_container_relation_p)->in_relation_list_head_p == NULL && (temp_r->dest_container_relation_p)->out_relation_list_head_p == NULL)
                    {
                        //Entità destinazione con container della relazione vuota, va eliminata
                        //Deferenzio il Container relazione
                        if ((temp_r->dest_container_relation_p)->prev_p != NULL)
                        {
                            ((temp_r->dest_container_relation_p)->prev_p)->next_p = (temp_r->dest_container_relation_p)->next_p;
                            if ((temp_r->dest_container_relation_p)->next_p != NULL)
                                ((temp_r->dest_container_relation_p)->next_p)->prev_p = (temp_r->dest_container_relation_p)->prev_p;
                        }
                        else
                        {
                            ((temp_r->dest_container_relation_p)->entity_p)->container_relation_head_p = (temp_r->dest_container_relation_p)->next_p;
                            if ((temp_r->dest_container_relation_p)->next_p != NULL)
                                ((temp_r->dest_container_relation_p)->next_p)->prev_p = NULL;
                        }
                        free((temp_r->dest_container_relation_p)->rel_name);
                        free(temp_r->dest_container_relation_p);
                    }

                if ((temp_r->src_container_relation_p)->in_relation_list_head_p == NULL && (temp_r->src_container_relation_p)->out_relation_list_head_p == NULL)
                {
                    //Entità sorgente con container della relazione vuota, va eliminata
                    //Deferenzio il Container relazione
                    if ((temp_r->src_container_relation_p)->prev_p != NULL)
                    {
                        ((temp_r->src_container_relation_p)->prev_p)->next_p = (temp_r->src_container_relation_p)->next_p;
                        if ((temp_r->src_container_relation_p)->next_p != NULL)
                            ((temp_r->src_container_relation_p)->next_p)->prev_p = (temp_r->src_container_relation_p)->prev_p;
                    }
                    else
                    {
                        ((temp_r->src_container_relation_p)->entity_p)->container_relation_head_p = (temp_r->src_container_relation_p)->next_p;
                        if ((temp_r->src_container_relation_p)->next_p != NULL)
                            ((temp_r->src_container_relation_p)->next_p)->prev_p = NULL;
                    }
                    free((temp_r->src_container_relation_p)->rel_name);
                    free(temp_r->src_container_relation_p);
                }

                //Cancello la relazione
                free(temp_r->src);
                free(temp_r->dest);
                free(temp_r);
            }
        }

        //docker su cui si ha agito vuoto
        if ((docker_ranking_head_p->container_floor_ranking_p)->max_floor_ranking_p == NULL)
        {
            if (docker_ranking_head_p->prev_p == NULL) //in testa elimino
                docker_update = docker_ranking_head_p->next_p;
            else
                (docker_ranking_head_p->prev_p)->next_p = docker_ranking_head_p->next_p;
            if (docker_ranking_head_p->next_p != NULL)
                (docker_ranking_head_p->next_p)->prev_p = docker_ranking_head_p->prev_p;
            free(docker_ranking_head_p->rel_name);
            free(docker_ranking_head_p->container_floor_ranking_p);
            free(docker_ranking_head_p);
        }
        //printf("Traking delete_relation end\n");
        return docker_update;
    }
    else
    {
        return docker_ranking_head_p;
    }
}
/*
    *** Status: Completed **
    Funzione Gestita per: Cancellare una relazione.
    - src_str : nome sorgente
    - dest_srt : nome destinazione
    - rel_str : nome relazione
    - docker_ranking_head_p : head dei docker
*/
docker_ranking_t *sys_delete_relation(char *src_str, char *dest_str, char *rel_str, docker_ranking_t *docker_ranking_head_p, info_entity_menager_t *hashtable[])
{
    //prendo input

    //printf("Traking sys_delete_relation\n");

    scanf("%s %s %s", src_str, dest_str, rel_str);
    //cerco docker
    docker_ranking_t *target_docker = docker_search(rel_str, docker_ranking_head_p), *updateDocker = NULL;
    //se il docker esiste
    if (target_docker != NULL)
        //updateDocker = delete_relation(src_str, dest_str, rel_str, docker_ranking_head_p);
        //todo Changed
        updateDocker = delete_relation_maho(src_str, dest_str, rel_str, docker_ranking_head_p, hashtable);
    //printf("Traking sys_delete_relation end\n");
    if (updateDocker != target_docker)
        return updateDocker;
    else
        return docker_ranking_head_p;
}
/*
    *** Status: In Progress **
    Cancella una relazione.
    - src_str : nome sorgente
    - dest_srt : nome destinazione
    - rel_str : nome relazione
    - hashtable : hashtable delle entità
    - docker_ranking_head_p : docker head
*/
docker_ranking_t *add_relation(info_entity_t *src_ie, info_entity_t *dest_ie, char *rel_str, docker_ranking_t *docker_ranking_head_p)
{
    //si presuppone da aggiungere, quindi il controllo va fatto prima!!!
    //aggiungo la relazione a dest e src, implica la creazione di container se non esistono
    //aggiungo un ranking node a dest

    //printf("Traking add_relation\n");
    docker_ranking_t *docker_update = docker_ranking_head_p, *target_docker;
    container_relation_t *temp_cr, *dest_cr = dest_ie->container_relation_head_p, *src_cr = src_ie->container_relation_head_p;
    info_relation_t *ir_to_add = malloc(sizeof(info_relation_t));
    //cerco i container in dest e src
    while (dest_cr != NULL)
    {
        if (strcmp(rel_str, dest_cr->rel_name) == 0)
            break;
        dest_cr = dest_cr->next_p;
    }
    if (dest_cr == NULL)
    {
        //non esiste il container
        temp_cr = malloc(sizeof(container_relation_t));
        temp_cr->rel_name = malloc(sizeof(char) * (strlen(rel_str) + 1));
        strcpy(temp_cr->rel_name, rel_str);
        temp_cr->entity_p = dest_ie;
        temp_cr->prev_p = NULL;
        temp_cr->next_p = dest_ie->container_relation_head_p;
        if (dest_ie->container_relation_head_p != NULL)
            (dest_ie->container_relation_head_p)->prev_p = temp_cr;
        temp_cr->out_relation_list_head_p = NULL;
        temp_cr->in_relation_list_head_p = NULL;
        temp_cr->ranking_node_p = NULL;
        dest_ie->container_relation_head_p = temp_cr;
        dest_cr = temp_cr;
        if (strcmp(dest_ie->entity_name, src_ie->entity_name) == 0)
            src_cr = dest_cr;
    }
    while (src_cr != NULL)
    {
        if (strcmp(rel_str, src_cr->rel_name) == 0)
            break;
        src_cr = src_cr->next_p;
    }
    if (src_cr == NULL)
    {
        //non esiste il container
        temp_cr = malloc(sizeof(container_relation_t));
        temp_cr->rel_name = malloc(sizeof(char) * (strlen(rel_str) + 1));
        strcpy(temp_cr->rel_name, rel_str);
        temp_cr->entity_p = src_ie;
        temp_cr->prev_p = NULL;
        temp_cr->next_p = src_ie->container_relation_head_p;
        if (src_ie->container_relation_head_p != NULL)
            (src_ie->container_relation_head_p)->prev_p = temp_cr;
        temp_cr->out_relation_list_head_p = NULL;
        temp_cr->in_relation_list_head_p = NULL;
        temp_cr->ranking_node_p = NULL;
        src_ie->container_relation_head_p = temp_cr;
        src_cr = temp_cr;
    }

    if (dest_cr->ranking_node_p == NULL)
    {
        ranking_node_t *rn_to_add = malloc(sizeof(ranking_node_t));
        rn_to_add->entity_p = dest_ie;
        dest_cr->ranking_node_p = rn_to_add;
        docker_update = sys_create_ranking_node(rn_to_add, rel_str, docker_ranking_head_p);
    }
    else
        ranking_node_scala(dest_cr, 1);

    //ho entrambi i container, procedo ad aggiungere la relazione
    ir_to_add->dest_container_relation_p = dest_cr;
    ir_to_add->src_container_relation_p = src_cr;
    ir_to_add->src = malloc(sizeof(char) * (strlen(src_ie->entity_name) + 1));
    ir_to_add->dest = malloc(sizeof(char) * (strlen(dest_ie->entity_name) + 1));

    strcpy(ir_to_add->src, src_ie->entity_name);
    strcpy(ir_to_add->dest, dest_ie->entity_name);
    ir_to_add->dest_prev_p = NULL;
    ir_to_add->src_prev_p = NULL;
    // Aggiungo in lista alle relazioni

    ir_to_add->dest_next_p = dest_cr->in_relation_list_head_p;
    if (dest_cr->in_relation_list_head_p != NULL)
        (dest_cr->in_relation_list_head_p)->dest_prev_p = ir_to_add;
    dest_cr->in_relation_list_head_p = ir_to_add;

    ir_to_add->src_next_p = src_cr->out_relation_list_head_p;
    if (src_cr->out_relation_list_head_p != NULL)
        (src_cr->out_relation_list_head_p)->src_prev_p = ir_to_add;
    src_cr->out_relation_list_head_p = ir_to_add;
    // Aggiungo irm to docker hashtable

    target_docker = docker_search(rel_str, docker_update);
    //todo continua a verficare
    //printf("Traking add_relation End\n");
    return docker_update;
}
docker_ranking_t *sys_call_add_relation(char *src_str, char *dest_str, char *rel_str, info_entity_menager_t *hashtable[], docker_ranking_t *docker_ranking_head_p)
{

    //printf("Traking sys_call_add_relation\n");
    scanf("%s %s %s", src_str, dest_str, rel_str);
    docker_ranking_t *docker_update = docker_ranking_head_p;
    //calcolo hashvalue
    int src_hash_value = hash(src_str) % modulo;
    int dest_hash_value = hash(dest_str) % modulo;
    info_entity_menager_t *src_iem = hashtable[src_hash_value], *dest_iem = hashtable[dest_hash_value];
    //verifico esistenza src
    while (src_iem != NULL)
    {
        if (strcmp(src_str, (src_iem->entity_p)->entity_name) == 0)
            break;
        src_iem = src_iem->next_p;
    }
    if (src_iem != NULL)
    {
        // Verifico esistenza dest
        while (dest_iem != NULL)
        {
            if (strcmp(dest_str, (dest_iem->entity_p)->entity_name) == 0)
                break;
            dest_iem = dest_iem->next_p;
        }

        if (dest_iem != NULL)
        {
            //Esistono entrambi
            // Ricerca docker target per avvio funzione
            docker_ranking_t *target_docker = docker_search(rel_str, docker_ranking_head_p);
            if (target_docker == NULL)
            {
                //se non esiste il docker, aggiungo la relazione al docker creato
                docker_update = add_relation(src_iem->entity_p, dest_iem->entity_p, rel_str, docker_ranking_head_p);
            }
            else
            {
                //se esiste il docker, va verificato se esiste la relazione o no
                //todo Changed
                container_relation_t *temp_cr = src_iem->entity_p->container_relation_head_p;
                while (temp_cr != NULL && strcmp(temp_cr->rel_name, rel_str) != 0)
                    temp_cr = temp_cr->next_p;
                if (temp_cr != NULL)
                {
                    //esiste il container nel src
                    info_relation_t *temp_ir = temp_cr->out_relation_list_head_p;
                    while (temp_ir != NULL && strcmp(temp_ir->dest, dest_str) != 0)
                        temp_ir = temp_ir->src_next_p;
                    if (temp_ir == NULL)
                    {
                        //non esiste la relazione allora aggiungo
                        docker_update = add_relation(src_iem->entity_p, dest_iem->entity_p, rel_str, docker_ranking_head_p);
                    }
                }
                else
                {
                    //non esiste la relazione allora aggiungo
                    docker_update = add_relation(src_iem->entity_p, dest_iem->entity_p, rel_str, docker_ranking_head_p);
                }
            }
        }
    }
    //printf("Traking sys_call_add_relation End\n");
    return docker_update;
}
/*
    *** Status: Completes**
    Cerca il docker relativo e restituisce il puntatore
    - str : nome relazione da cercare
    - docker_ranking_head_p : head dei docker
*/
docker_ranking_t *docker_search(char *str, docker_ranking_t *docker_ranking_head_p)
{
    //restituisce il puntatore al docker giusto data la relazione
    while (docker_ranking_head_p != NULL)
    {
        if (strcmp(str, docker_ranking_head_p->rel_name) == 0)
        {
            break;
        }
        docker_ranking_head_p = docker_ranking_head_p->next_p;
    }
    return docker_ranking_head_p;
}
void sys_call_print_docker(docker_ranking_t *docker_head)
{
    floor_ranking_t *temp_fr;
    ranking_node_t *temp_rn;
    if (docker_head == NULL)
        printf("Nessuna relazione nei Docker\n");

    while (docker_head != NULL)
    {
        temp_fr = docker_head->container_floor_ranking_p->max_floor_ranking_p;
        printf("Docker relazione: %s\nPrev Docker: %p\nNext Docker: %p\n", docker_head->rel_name, docker_head->prev_p, docker_head->next_p);
        printf("Floors: \n");
        while (temp_fr != NULL)
        {
            temp_rn = temp_fr->ranking_node_head_p;
            printf("    Count: %lu\n    Status: %c\n    Prev floor: %p\n    Next floor: %p\n    Entity:\n", temp_fr->count, temp_fr->flag, temp_fr->prev_p, temp_fr->next_p);
            while (temp_rn != NULL)
            {
                printf("        %s\n", (temp_rn->entity_p)->entity_name);
                temp_rn = temp_rn->next_p;
            }
            temp_fr = temp_fr->prev_p;
        }

        docker_head = docker_head->next_p;
    }
}

/*
    Funzione che compara 2 stringhe, restituisce:
    -> 0 se il primo è più piccolo del secondo
    -> 1 se il secondo è più piccolo del primo
*/
int str_compare_order(char *str1, char *str2)
{
    //DONE
    //ritorna il più piccolo tra i 2 0 il primo e 1 il secondo
    //assumo che le stringhe siano tutte differenti
    int len = strlen(str1), i;
    if (str2 != NULL)
    {
        if (len > strlen(str2))
            len = strlen(str2);
        for (i = 0; i < len && str1[i] == str2[i]; i++)
            ;
        if (i == len)
            if (len == strlen(str1)) //vero se sono tutte diverse le stringhe
                return 0;
            else
                return 1;
        else
            return str1[i] > str2[i];
    }
    else
        return 0;
}
void docker_max_to_sort(container_floor_ranking_t *docker_to_sort)
{
    ranking_node_t *head_rn = (docker_to_sort->max_floor_ranking_p)->ranking_node_head_p, *compare_rn, *next_rn;
    (docker_to_sort->max_floor_ranking_p)->flag = 's';
    int change = 1;
    while (change != 0)
    {
        head_rn = (docker_to_sort->max_floor_ranking_p)->ranking_node_head_p;
        change = 0;
        while (head_rn != NULL)
        {
            next_rn = head_rn->next_p;
            compare_rn = next_rn;
            if (compare_rn != NULL)
            {
                while (compare_rn->next_p != NULL && str_compare_order((head_rn->entity_p)->entity_name, (compare_rn->entity_p)->entity_name) == 1)
                    compare_rn = compare_rn->next_p;
                if (compare_rn->next_p != NULL)
                {
                    if (head_rn->next_p != compare_rn)
                    {
                        //Deferenzio il rn-node
                        (head_rn->next_p)->prev_p = head_rn->prev_p;
                        if (head_rn->prev_p != NULL)
                            (head_rn->prev_p)->next_p = head_rn->next_p;
                        else
                            (docker_to_sort->max_floor_ranking_p)->ranking_node_head_p = head_rn->next_p;
                        //referenzio il rn-node
                        (compare_rn->prev_p)->next_p = head_rn;
                        head_rn->prev_p = compare_rn->prev_p;
                        compare_rn->prev_p = head_rn;
                        head_rn->next_p = compare_rn;
                        change++;
                    }
                }
                else
                {
                    if (str_compare_order((head_rn->entity_p)->entity_name, (compare_rn->entity_p)->entity_name) == 1)
                    {
                        //Deferenzio il rn-node
                        (head_rn->next_p)->prev_p = head_rn->prev_p;
                        if (head_rn->prev_p != NULL)
                            (head_rn->prev_p)->next_p = head_rn->next_p;
                        else
                            (docker_to_sort->max_floor_ranking_p)->ranking_node_head_p = head_rn->next_p;
                        //referenzio il rn-node
                        head_rn->next_p = NULL;
                        head_rn->prev_p = compare_rn;
                        compare_rn->next_p = head_rn;
                        change++;
                    }
                    else
                    {
                        if (head_rn->next_p != compare_rn)
                        {
                            //Deferenzio il rn-node
                            (head_rn->next_p)->prev_p = head_rn->prev_p;
                            if (head_rn->prev_p != NULL)
                                (head_rn->prev_p)->next_p = head_rn->next_p;
                            else
                                (docker_to_sort->max_floor_ranking_p)->ranking_node_head_p = head_rn->next_p;
                            //referenzio il rn-node
                            (compare_rn->prev_p)->next_p = head_rn;
                            head_rn->prev_p = compare_rn->prev_p;
                            compare_rn->prev_p = head_rn;
                            head_rn->next_p = compare_rn;
                            change++;
                        }
                    }
                }
            }
            head_rn = next_rn;
        }
    }
}
docker_ranking_t *sys_call_report(docker_ranking_t *docker_head)
{
    //printf("traking report\n");
    //scorro e controllo se sono in ordine, altrimenti sposto il docker
    docker_ranking_t *docker_head_to_res = docker_head, *sort_docker = docker_head, *temp_docker, *trapass_docker;
    ranking_node_t *to_print_node;
    if (sort_docker != NULL)
    {
        //oridino i docker
        int change = 1;
        while (change != 0)
        {
            sort_docker = docker_head_to_res;
            change = 0;
            while (sort_docker != NULL)
            {

                temp_docker = sort_docker->next_p;
                trapass_docker = temp_docker;
                if (temp_docker != NULL)
                {
                    //se vi è un next e che è minore del prev vado avanti fino a quando una delle due è falsa
                    while (temp_docker->next_p != NULL && str_compare_order(sort_docker->rel_name, temp_docker->rel_name) == 1)
                        temp_docker = temp_docker->next_p;
                    if (temp_docker->next_p != NULL)
                    {
                        //docker target trovato
                        if (temp_docker != sort_docker->next_p)
                        {

                            //deferenzio il docker da spostare
                            (sort_docker->next_p)->prev_p = sort_docker->prev_p;
                            if (sort_docker->prev_p != NULL)
                                (sort_docker->prev_p)->next_p = sort_docker->next_p;
                            else
                                docker_head_to_res = sort_docker->next_p;
                            //ri-referenzio il docker
                            (temp_docker->prev_p)->next_p = sort_docker;
                            sort_docker->prev_p = temp_docker->prev_p;
                            temp_docker->prev_p = sort_docker;
                            sort_docker->next_p = temp_docker;
                            change++;
                        }
                    }
                    else
                    {
                        //va fatto l'ultimo confronto
                        if (str_compare_order(sort_docker->rel_name, temp_docker->rel_name) == 1)
                        {
                            //allora diventa ultimo
                            (sort_docker->next_p)->prev_p = sort_docker->prev_p;
                            if (sort_docker->prev_p != NULL)
                                (sort_docker->prev_p)->next_p = sort_docker->next_p;
                            else
                                docker_head_to_res = sort_docker->next_p;
                            sort_docker->prev_p = temp_docker;
                            sort_docker->next_p = NULL;
                            temp_docker->next_p = sort_docker;
                            change++;
                        }
                        else
                        {
                            //sposto normalmente
                            if (temp_docker != sort_docker->next_p)
                            {
                                //deferenzio il docker da spostare
                                (sort_docker->next_p)->prev_p = sort_docker->prev_p;
                                if (sort_docker->prev_p != NULL)
                                    (sort_docker->prev_p)->next_p = sort_docker->next_p;
                                else
                                    docker_head_to_res = sort_docker->next_p;
                                //ri-referenzio il docker
                                (temp_docker->prev_p)->next_p = sort_docker;
                                sort_docker->prev_p = temp_docker->prev_p;
                                temp_docker->prev_p = sort_docker;
                                sort_docker->next_p = temp_docker;
                                change++;
                            }
                        }
                    }
                }
                sort_docker = trapass_docker;
            }
        }
        //ora ho i Docker ordinati

        //scorro i docker e ordino il max list
        // uso docker_head_to_res
        temp_docker = docker_head_to_res;
        while (temp_docker != NULL)
        {
            printf("%s", temp_docker->rel_name);
            if (((temp_docker->container_floor_ranking_p)->max_floor_ranking_p)->flag == 'd')
                docker_max_to_sort(temp_docker->container_floor_ranking_p);
            to_print_node = ((temp_docker->container_floor_ranking_p)->max_floor_ranking_p)->ranking_node_head_p;
            while (to_print_node != NULL)
            {
                printf(" %s", (to_print_node->entity_p)->entity_name);
                to_print_node = to_print_node->next_p;
            }
            printf(" %lu;", ((temp_docker->container_floor_ranking_p)->max_floor_ranking_p)->count);
            temp_docker = temp_docker->next_p;
            if (temp_docker != NULL)
                printf(" ");
        }
        printf("\n");
    }
    else
    {
        printf("none\n");
    }
    //printf("traking report End\n");
    return docker_head_to_res;
}
int main()
{
    info_entity_menager_t *entity_hashtable[modulo] = {NULL};
    docker_ranking_t *docker_head = NULL;
    char input[256] = "addrel", input2[256], input3[256];
    int i = 0;
    while (input[0] != 'e')
    {
        scanf("%s", input);
        if (input[0] != 'e')
        {
            if (input[0] != 'r')
            {
                if (input[0] == 'a')
                {
                    if (input[3] == 'r')
                        docker_head = sys_call_add_relation(input, input2, input3, entity_hashtable, docker_head); // V 1.0.0
                    else
                        sys_call_add_entity(input, entity_hashtable); // V 1.0.0
                }
                else
                {
                    if (input[3] == 'e')
                        docker_head = sys_call_delete_entity(input, entity_hashtable, docker_head); // V 1.0.0
                    else
                        docker_head = sys_delete_relation(input, input2, input3, docker_head, entity_hashtable); // V 1.0.0
                }
            }
            else
            {
                docker_head = sys_call_report(docker_head); // 1.0.0
                //sys_call_print_docker(docker_head);
                //sys_call_print_entity_hashtable(entity_hashtable);
            }
            input[0] = 'r';
        }
    }
}
