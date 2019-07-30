#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define modulo 10007

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
} info_relation_t;

typedef struct info_entity_menager
{
    struct info_entity *entity_p;
    struct info_entity_menager *next_p;
} info_entity_menager_t;
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
    struct container_floor_ranking *container_floor_ranking_p;
    struct docker_ranking *next_p;
    struct docker_ranking *prev_p;
} docker_ranking_t;
docker_ranking_t *docker_search(char *str, docker_ranking_t *docker_ranking_head_p);
void sys_call_print_entity_hashtable(info_entity_menager_t *hashtable[]);
void ranking_node_scala(container_relation_t *target_cr, int move);
void delete_ranking_node(ranking_node_t *target);
docker_ranking_t *delete_docker(docker_ranking_t *docker_curr_head, docker_ranking_t *target_to_delete);
void delete_container(container_relation_t *cr_to_delete);
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
    Funzione Gestita per aggiungere una entità
    - puntatore costante per prendere l'input
    - puntatore ad hashtable nodi
*/
void sys_call_add_entity(char *str, info_entity_menager_t *hashtable[])
{
    /* ========================
        Tasks: 
            1) Lettura input
            2) Verifica esisteza entità nella Hashtable entità
                2.1) Esiste, nulla
                2.2) Non Esiste, aggiungo l'entità
    ======================== */

    /* ======================== INIZIO REFACTOR ======================== */

    /* 1) Lettura input */

    scanf("%s", str);
    // Calcolo Hashvalue dell'entità
    int entity_hash = hash(str) % modulo;
    info_entity_menager_t *entity_iem = hashtable[entity_hash];

    /* 2) Verifica esisteza entità nella Hashtable entità */

    while (entity_iem != NULL && strcmp(str, (entity_iem->entity_p)->entity_name) != 0)
        entity_iem = entity_iem->next_p;

    if (entity_iem == NULL)
    {
        /* 2.2) Non Esiste, aggiungo l'entità */
        entity_iem = malloc(sizeof(info_entity_menager_t));
        entity_iem->entity_p = malloc(sizeof(info_entity_t));
        (entity_iem->entity_p)->entity_name = malloc(sizeof(char) * (strlen(str) + 1));
        entity_iem->next_p = hashtable[entity_hash];
        hashtable[entity_hash] = entity_iem;
        (entity_iem->entity_p)->container_relation_head_p = NULL;
        strcpy((entity_iem->entity_p)->entity_name, str);
    }

    /* ======================== FINE REFACTOR ======================== */
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
                    printf("            %s => %s\n", temp_ir->src_container_relation_p->entity_p->entity_name, temp_ir->dest_container_relation_p->entity_p->entity_name);
                    temp_ir = temp_ir->dest_next_p;
                }
                temp_ir = temp_cr->out_relation_list_head_p;
                printf("        Stampo relazioni out:\n");
                while (temp_ir != NULL)
                {
                    printf("            %s => %s\n", temp_ir->src_container_relation_p->entity_p->entity_name, temp_ir->dest_container_relation_p->entity_p->entity_name);
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
docker_ranking_t *delete_entity(info_entity_t *entity_to_delete_info, docker_ranking_t *docker_ranking_head_p)
{
    /* ======================== 
        Tasks:
            1) Scorro i Container dell'entità
            2) Per ogni Container
                2.1) Elimino le relazioni in uscita
                    2.1.1) Gestione ranking node dell'entità destinazione
                    2.1.2) Gestione "se necessario" del container della destinazione
                2.2) Elimino le relazioni in entrata
                    2.2.1) Gestione "se necessario" del container sorgente
                2.3) Elimino il ranking node dell'entità da eliminare
                2.4) "Se necessario" gestione Docker corrente del container di riferimento
                2.5) Elimino info del Container
            3) Elimino info allocate
    ======================== */

    /* ======================== INIZIO REFACTOR ======================== */

    container_relation_t *temp_cr, *target_container_to_delete = entity_to_delete_info->container_relation_head_p, *to_free_cr;
    container_floor_ranking_t *target_floor_container_to_delete;
    info_relation_t *target_info_relation_to_delete;
    /* 1) Scorro i Container dell'entità */
    while (target_container_to_delete != NULL)
    {
        /* 2) Per ogni Container */

        // Provo a cercare il Docker target
        target_floor_container_to_delete = NULL;
        if (target_container_to_delete->ranking_node_p != NULL)
            target_floor_container_to_delete = ((target_container_to_delete->ranking_node_p)->floor_ranking_p)->container_floor_ranking_p;

        /* 2.1) Elimino le relazioni in uscita */
        target_info_relation_to_delete = target_container_to_delete->out_relation_list_head_p;

        // Se vi è una relazione in uscita allora sicuramente esiste il Docker, nel caso sopra avesse fallito
        if (target_info_relation_to_delete != NULL)
            target_floor_container_to_delete = (((target_info_relation_to_delete->dest_container_relation_p)->ranking_node_p)->floor_ranking_p)->container_floor_ranking_p;

        while (target_info_relation_to_delete != NULL)
        {
            // Deferenzio la relazione nel container della destinazione
            temp_cr = target_info_relation_to_delete->dest_container_relation_p;

            if (target_info_relation_to_delete->dest_prev_p != NULL)
                (target_info_relation_to_delete->dest_prev_p)->dest_next_p = target_info_relation_to_delete->dest_next_p;
            else
                temp_cr->in_relation_list_head_p = target_info_relation_to_delete->dest_next_p;

            if (target_info_relation_to_delete->dest_next_p != NULL)
                (target_info_relation_to_delete->dest_next_p)->dest_prev_p = target_info_relation_to_delete->dest_prev_p;

            /* 2.1.1) Gestione ranking node dell'entità destinazione */
            ranking_node_scala(target_info_relation_to_delete->dest_container_relation_p, 0);

            /* 2.1.2) Gestione "se necessario" del container della destinazione */
            if (temp_cr->in_relation_list_head_p == NULL && temp_cr->out_relation_list_head_p == NULL && temp_cr != target_container_to_delete)
                // Container da eliminare, ma non è lo stesso in cui sto esegendo le operazioni
                delete_container(temp_cr);
            target_info_relation_to_delete = target_info_relation_to_delete->src_next_p;
        }

        /* 2.2) Elimino le relazioni in entrata */
        target_info_relation_to_delete = target_container_to_delete->in_relation_list_head_p;

        while (target_info_relation_to_delete != NULL)
        {
            // Deferenzio la relazione nel container della sorgente
            temp_cr = target_info_relation_to_delete->src_container_relation_p;

            if (target_info_relation_to_delete->src_prev_p != NULL)
                (target_info_relation_to_delete->src_prev_p)->src_next_p = target_info_relation_to_delete->src_next_p;
            else
                temp_cr->out_relation_list_head_p = target_info_relation_to_delete->src_next_p;

            if (target_info_relation_to_delete->src_next_p != NULL)
                (target_info_relation_to_delete->src_next_p)->src_prev_p = target_info_relation_to_delete->src_prev_p;

            /* 2.2.1) Gestione "se necessario" del container sorgente */
            if (temp_cr->in_relation_list_head_p == NULL && temp_cr->out_relation_list_head_p == NULL && temp_cr != target_container_to_delete)
                // Container da eliminare, ma non è lo stesso in cui sto esegendo le operazioni
                delete_container(temp_cr);
            target_info_relation_to_delete = target_info_relation_to_delete->dest_next_p;
        }

        /* 2.3) Elimino il ranking node dell'entità da eliminare */
        if (target_container_to_delete->ranking_node_p != NULL)
            delete_ranking_node(target_container_to_delete->ranking_node_p);

        /* 2.4) "Se necessario" gestione Docker corrente del container di riferimento */
        if (target_floor_container_to_delete->max_floor_ranking_p == NULL)
            // Elimino il docker
            docker_ranking_head_p = delete_docker(docker_ranking_head_p, target_floor_container_to_delete->docker_ranking_p);

        /* 2.5) Elimino info del Container */
        to_free_cr = target_container_to_delete;
        target_container_to_delete = target_container_to_delete->next_p;
        free(to_free_cr);
    }
    free(entity_to_delete_info->entity_name);
    free(entity_to_delete_info);
    return docker_ranking_head_p;
    /* ======================== FINE REFACTOR ======================== */
}
/*
    Funzione Gestita: legge l'entità da cancellare e la cancella gestendo tutte le relative referenze.
    Restituisce l'head del docker.
*/
docker_ranking_t *sys_call_delete_entity(char *str, info_entity_menager_t *hashtable[], docker_ranking_t *docker_ranking_head_p)
{
    /* ===================== 
        Tasks:
            1) Lettura Input
            2) Ricerca esistenza entità da eliminare
                2.1) Esiste, procedo alla eliminazione
                2.2) Non Esiste, nulla 
    ===================== */

    /* ===================== INIZIO REFACTOR ===================== */

    /* 1) Lettura Input */
    scanf("%s", str);
    // Inizializzo variabili
    int value = hash(str) % modulo;
    info_entity_menager_t *temp_iem = hashtable[value], *prev_iem = NULL;

    /* 2) Ricerca esistenza entità da eliminare */
    while (temp_iem != NULL && strcmp(str, (temp_iem->entity_p)->entity_name) != 0)
    {
        prev_iem = temp_iem;
        temp_iem = temp_iem->next_p;
    }
    if (temp_iem != NULL)
    {
        /* 2.1) Esiste, procedo alla eliminazione */

        // Deferenzio l'entità dalla Hashtable
        if (prev_iem != NULL)
            prev_iem->next_p = temp_iem->next_p;
        else
            hashtable[value] = temp_iem->next_p;
        docker_ranking_head_p = delete_entity(temp_iem->entity_p, docker_ranking_head_p);
        free(temp_iem);
    }
    return docker_ranking_head_p;
    /* ===================== FINE REFACTOR ===================== */
}
/*
    *** Status: Completed **
    Restituisce ranking_node_t creato, aggiungendo l'entita nel sistema di tracking di rank
    - info_entity_p : puntatore all'entità
    - rel_str : nome della relazione da tracciare per l'entità
    - docker_ranking_head_p : puntatore all'head dei docker
 */
void sys_create_ranking_node(ranking_node_t *rn_to_add, container_floor_ranking_t *container_floor_ranking_p)
{
    /* ========================= 
        Task:
            1) Aggiungo in coda il ranking-node
                1.1) Gestione minimo e massimo
    ========================= */

    /* ========================= INIZIO REFACTOR ========================= */
    floor_ranking_t *temp_fr = container_floor_ranking_p->min_floor_ranking_p, *new_fr;
    rn_to_add->prev_p = NULL;
    /* 1) Aggiungo in coda il ranking-node */
    if (temp_fr == NULL || temp_fr->count != 1)
    {
        // Non esiste nessun piano oppure il minimo non è quello che ci serve
        new_fr = malloc(sizeof(floor_ranking_t));
        new_fr->count = 1;
        new_fr->flag = 's';
        new_fr->container_floor_ranking_p = container_floor_ranking_p;
        new_fr->prev_p = NULL;
        new_fr->ranking_node_head_p = rn_to_add;
        new_fr->next_p = temp_fr;
        rn_to_add->next_p = NULL;
        rn_to_add->floor_ranking_p = new_fr;
        if (temp_fr != NULL)
            temp_fr->prev_p = new_fr;
        container_floor_ranking_p->min_floor_ranking_p = new_fr;
        if (container_floor_ranking_p->max_floor_ranking_p == NULL)
            container_floor_ranking_p->max_floor_ranking_p = new_fr;
    }
    else
    {
        // Esiste già il piano giusto
        rn_to_add->floor_ranking_p = temp_fr;
        rn_to_add->next_p = temp_fr->ranking_node_head_p;
        //if (temp_fr->ranking_node_head_p != NULL)
        (temp_fr->ranking_node_head_p)->prev_p = rn_to_add;
        temp_fr->ranking_node_head_p = rn_to_add;
        temp_fr->flag = 'd';
    }
    /* ========================= FINE REFACTOR ========================= */
}
/*
    *** Status: Completed **
    Toglie dal Sistema di Monitoraggio Rank il target
    - target : il nodo da non monitorare in rank
    NOTA : non gestisce il docker, quindi va gestito. Gestisce l'eliminazione del piano se necessario
 */
void delete_ranking_node(ranking_node_t *target_ranking_node)
{
    /* ====================== 
        Tasks: 
            1) Deferenzio il ranking node
            2) Gestito il floor e massimi e minimi
            3) Libero il nodo
    ====================== */

    /* ====================== INIZIO REFACTOR ====================== */

    floor_ranking_t *target_container_floor = target_ranking_node->floor_ranking_p;

    /* 1) Deferenzio il ranking node */

    if (target_ranking_node->prev_p != NULL)
        (target_ranking_node->prev_p)->next_p = target_ranking_node->next_p;
    else
        (target_ranking_node->floor_ranking_p)->ranking_node_head_p = target_ranking_node->next_p;
    if (target_ranking_node->next_p != NULL)
        (target_ranking_node->next_p)->prev_p = target_ranking_node->prev_p;

    /* 2) Gestito il floor e massimi e minimi */
    if (target_container_floor->ranking_node_head_p == NULL)
    {
        // ranking floor vuoto, da eliminare
        // Deferenzio il piano
        if (target_container_floor->prev_p != NULL)
            (target_container_floor->prev_p)->next_p = target_container_floor->next_p;
        else
            (target_container_floor->container_floor_ranking_p)->min_floor_ranking_p = target_container_floor->next_p;

        if (target_container_floor->next_p != NULL)
            (target_container_floor->next_p)->prev_p = target_container_floor->prev_p;
        else
            (target_container_floor->container_floor_ranking_p)->max_floor_ranking_p = target_container_floor->prev_p;

        free(target_container_floor);
    }
    free(target_ranking_node);
    /* ====================== FINE REFACTOR ====================== */
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
    /* =============================
        Tasks:
            0) Deferenzio il ranking node dalla lista attuale
            1) Up Move
                1.1) Verifico se il piano esiste
                    1.1.1) Non Esiste, lo creo e verifico se nuovo massimo
                    1.1.2) Esiste, sposto il nodo e setto a dirty
                1.2) Gestione last floor
            2) Down Move
                2.1) Verifico se il piano esiste
                    2.1.1) Non Esiste, se va sotto 1 allora libero altrimenti creo
                    2.1.2) Esiste, sposto il nodo e setto a dirty
                2.2) Gestione last floor
    ============================= */

    /* =================== INIZIO REFACTOR =================== */
    ranking_node_t *to_move_ranking_node = target_cr->ranking_node_p;

    floor_ranking_t *temp_fr = to_move_ranking_node->floor_ranking_p, *last_fr = to_move_ranking_node->floor_ranking_p;
    /* 0) Deferenzio il ranking node dalla lista attuale */

    if (to_move_ranking_node->prev_p == NULL)
        (to_move_ranking_node->floor_ranking_p)->ranking_node_head_p = to_move_ranking_node->next_p;
    else
        (to_move_ranking_node->prev_p)->next_p = to_move_ranking_node->next_p;

    if (to_move_ranking_node->next_p != NULL)
        (to_move_ranking_node->next_p)->prev_p = to_move_ranking_node->prev_p;

    to_move_ranking_node->prev_p = NULL;

    if (move == 1)
    {
        /* 1) Up Move */

        if (temp_fr->next_p != NULL && (temp_fr->next_p)->count == temp_fr->count + 1)
        {
            /* 1.1.2) Esiste, sposto il nodo e setto a dirty */

            temp_fr = temp_fr->next_p;

            to_move_ranking_node->floor_ranking_p = temp_fr;
            to_move_ranking_node->next_p = temp_fr->ranking_node_head_p;

            temp_fr->flag = 'd';
            (temp_fr->ranking_node_head_p)->prev_p = to_move_ranking_node;
            temp_fr->ranking_node_head_p = to_move_ranking_node;
        }
        else
        {
            /* 1.1.1) Non Esiste, lo creo e verifico se nuovo massimo */

            to_move_ranking_node->next_p = NULL;
            temp_fr = malloc(sizeof(floor_ranking_t));
            temp_fr->count = last_fr->count + 1;
            temp_fr->container_floor_ranking_p = last_fr->container_floor_ranking_p;
            temp_fr->ranking_node_head_p = to_move_ranking_node;
            temp_fr->flag = 's';
            to_move_ranking_node->floor_ranking_p = temp_fr;
            temp_fr->prev_p = last_fr;
            temp_fr->next_p = last_fr->next_p;
            last_fr->next_p = temp_fr;
            if (temp_fr->next_p != NULL)
                (temp_fr->next_p)->prev_p = temp_fr;
            else
                (temp_fr->container_floor_ranking_p)->max_floor_ranking_p = temp_fr;
        }

        /* 1.2) Gestione last floor */
        if (last_fr->ranking_node_head_p == NULL)
        {
            // Deferenzio il floor ranking
            if (last_fr->prev_p != NULL)
                (last_fr->prev_p)->next_p = last_fr->next_p;
            else
                (last_fr->container_floor_ranking_p)->min_floor_ranking_p = last_fr->next_p;

            (last_fr->next_p)->prev_p = last_fr->prev_p;
            free(last_fr);
        }
    }
    else
    {
        /* 2) Down Move */
        if (temp_fr->prev_p != NULL && (temp_fr->prev_p)->count == temp_fr->count - 1)
        {
            /* 2.1.2) Esiste, sposto il nodo e setto a dirty */
            temp_fr = temp_fr->prev_p;

            to_move_ranking_node->floor_ranking_p = temp_fr;
            to_move_ranking_node->next_p = temp_fr->ranking_node_head_p;
            //if (temp_fr->ranking_node_head_p != NULL)
            (temp_fr->ranking_node_head_p)->prev_p = to_move_ranking_node;
            temp_fr->ranking_node_head_p = to_move_ranking_node;
            temp_fr->flag = 'd';
        }
        else
        {
            /* 2.1.1) Non Esiste, se va sotto 1 allora libero altrimenti creo */
            if (temp_fr->count != 1)
            {
                to_move_ranking_node->next_p = NULL;
                temp_fr = malloc(sizeof(floor_ranking_t));
                to_move_ranking_node->floor_ranking_p = temp_fr;
                temp_fr->ranking_node_head_p = to_move_ranking_node;
                temp_fr->count = last_fr->count - 1;
                temp_fr->flag = 's';
                temp_fr->container_floor_ranking_p = last_fr->container_floor_ranking_p;
                temp_fr->prev_p = last_fr->prev_p;
                temp_fr->next_p = last_fr;
                last_fr->prev_p = temp_fr;
                if (temp_fr->prev_p != NULL)
                    (temp_fr->prev_p)->next_p = temp_fr;
                else
                    (temp_fr->container_floor_ranking_p)->min_floor_ranking_p = temp_fr;
            }
            else
            {
                // cancello il nodo
                target_cr->ranking_node_p = NULL;
                free(to_move_ranking_node);
            }
        }

        /* 2.2) Gestione last floor */

        if (last_fr->ranking_node_head_p == NULL)
        {
            // Deferenzio il floor ranking
            if (last_fr->next_p != NULL)
                (last_fr->next_p)->prev_p = last_fr->prev_p;
            else
                (last_fr->container_floor_ranking_p)->max_floor_ranking_p = last_fr->prev_p;

            if (last_fr->prev_p != NULL)
                (last_fr->prev_p)->next_p = last_fr->next_p;
            else
                (last_fr->container_floor_ranking_p)->min_floor_ranking_p = last_fr->next_p;

            free(last_fr);
        }
    }
    /* =================== FINE REFACTOR =================== */
}

/*
    *** Status: Completed **
    Funzione Gestita per: Cancellare una relazione.
    - src_str : nome sorgente
    - dest_srt : nome destinazione
    - rel_str : nome relazione
    - docker_ranking_head_p : head dei docker
*/
void delete_container(container_relation_t *cr_to_delete)
{
    /* ======================= 
        Tasks:
            1) Deferenziare il Container target
            2) Liberare la memoria
    ======================= */

    /* ======================= INIZIO REFACTOR ======================= */

    /* 1) Deferenziare il Container target */

    if (cr_to_delete->prev_p != NULL)
        (cr_to_delete->prev_p)->next_p = cr_to_delete->next_p;
    else
        (cr_to_delete->entity_p)->container_relation_head_p = cr_to_delete->next_p;

    if (cr_to_delete->next_p != NULL)
        (cr_to_delete->next_p)->prev_p = cr_to_delete->prev_p;

    /* 2) Liberare la memoria */
    if (cr_to_delete->ranking_node_p != NULL)
        delete_ranking_node(cr_to_delete->ranking_node_p);

    free(cr_to_delete);
    /* ======================= FINE REFACTOR ======================= */
}
void deferece_relation(info_relation_t *ir_to_deference)
{
    /* ================
        Tasks: 
            1) Deferenzia da Container Dest
            2) Deferenzia da Container Src
        NOTA: 
            1) Non cancella il container in caso di Empty Relation
    ================ */

    /* ================ INIZIO REFACTOR ================ */

    /* 1) Deferenzia da Container Dest */

    if (ir_to_deference->dest_prev_p != NULL)
        (ir_to_deference->dest_prev_p)->dest_next_p = ir_to_deference->dest_next_p;
    else
        (ir_to_deference->dest_container_relation_p)->in_relation_list_head_p = ir_to_deference->dest_next_p;
    if (ir_to_deference->dest_next_p != NULL)
        (ir_to_deference->dest_next_p)->dest_prev_p = ir_to_deference->dest_prev_p;

    /* 2) Deferenzia da Container Src */

    if (ir_to_deference->src_prev_p != NULL)
        (ir_to_deference->src_prev_p)->src_next_p = ir_to_deference->src_next_p;
    else
        (ir_to_deference->src_container_relation_p)->out_relation_list_head_p = ir_to_deference->src_next_p;
    if (ir_to_deference->src_next_p != NULL)
        (ir_to_deference->src_next_p)->src_prev_p = ir_to_deference->src_prev_p;

    /* ================ FINE REFACTOR ================ */
}
docker_ranking_t *delete_docker(docker_ranking_t *docker_curr_head, docker_ranking_t *target_to_delete)
{
    /* =================== 
        Task:
            1) Deferenziare il Target Docker da eliminare
            2) Aggiornare se necessario l'head docker
            3) Libero il Docker
    =================== */

    /* =================== INIZIO REFACTOR =================== */

    /* 1) Deferenziare il Target Docker da eliminare */

    if (target_to_delete->prev_p != NULL)
        (target_to_delete->prev_p)->next_p = target_to_delete->next_p;
    else
        /*  2) Aggiornare se necessario l'head docker */
        docker_curr_head = target_to_delete->next_p;
    if (target_to_delete->next_p != NULL)
        (target_to_delete->next_p)->prev_p = target_to_delete->prev_p;

    /* 3) Libero il Docker */
    free(target_to_delete->rel_name);
    free(target_to_delete->container_floor_ranking_p);
    free(target_to_delete);

    return docker_curr_head;

    /* =================== FINE REFACTOR =================== */
}
docker_ranking_t *sys_delete_relation(char *src_str, char *dest_str, char *rel_str, docker_ranking_t *docker_ranking_head_p, info_entity_menager_t *hashtable[])
{
    /* ========================= 
        Tasks:
            1) Lettura Input
            2) Verifica esistenza sorgente
            3) Verifica esistenza relazione
                3.1) Esistenza cancello (Nota #1)
        NOTA:
            1) Gestione head docker
    ========================= */

    /* ========================= INIZIO REFACTOR ========================= */

    /* 1) Lettura Input */

    scanf("%s %s %s", src_str, dest_str, rel_str);

    int src_hash_value = hash(src_str) % modulo, dest_hash_value = hash(dest_str) % modulo;
    info_entity_menager_t *src_iem = hashtable[src_hash_value];
    container_relation_t *temp_cr;
    info_relation_t *temp_ir;
    container_floor_ranking_t *target_container_floor;

    /* 2) Verifica esistenza sorgente */

    if (src_iem != NULL && hashtable[dest_hash_value] != NULL)
    {
        while (src_iem != NULL && strcmp(src_str, (src_iem->entity_p)->entity_name) != 0)
            src_iem = src_iem->next_p;
        if (src_iem != NULL)
        {
            /* 3) Verifica esistenza relazione */
            temp_cr = (src_iem->entity_p)->container_relation_head_p;
            while (temp_cr != NULL && strcmp(rel_str, temp_cr->rel_name) != 0)
                temp_cr = temp_cr->next_p;

            if (temp_cr != NULL)
            {
                temp_ir = temp_cr->out_relation_list_head_p;
                //todo ottimizzare strcmp
                while (temp_ir != NULL && strcmp(dest_str, ((temp_ir->dest_container_relation_p)->entity_p)->entity_name) != 0)
                    temp_ir = temp_ir->src_next_p;
                if (temp_ir != NULL)
                {
                    /* 3.1) Esistenza cancello relazione */
                    // Deferenzio la relazione
                    deferece_relation(temp_ir);

                    target_container_floor = (((temp_ir->dest_container_relation_p)->ranking_node_p)->floor_ranking_p)->container_floor_ranking_p;

                    // Gestione ranking node dell'entità destinazione
                    ranking_node_scala(temp_ir->dest_container_relation_p, 0);
                    // Controllo stato container dest
                    if (temp_ir->dest_container_relation_p != temp_ir->src_container_relation_p && (temp_ir->dest_container_relation_p)->in_relation_list_head_p == NULL && (temp_ir->dest_container_relation_p)->out_relation_list_head_p == NULL)
                        // Container relazioni dell'entità destinazione vuota
                        delete_container(temp_ir->dest_container_relation_p);
                    // Controllo stato container src
                    if ((temp_ir->src_container_relation_p)->in_relation_list_head_p == NULL && (temp_ir->src_container_relation_p)->out_relation_list_head_p == NULL)
                        // Container relazioni dell'entità sorgente vuota
                        delete_container(temp_ir->src_container_relation_p);
                    // Controllo stato Docker
                    if (target_container_floor->max_floor_ranking_p == NULL)
                        // Elimino il Docker e Aggiorno se necessario l'Head
                        docker_ranking_head_p = delete_docker(docker_ranking_head_p, target_container_floor->docker_ranking_p);
                    free(temp_ir);
                }
            }
        }
    }
    return docker_ranking_head_p;
    /* ========================= FINE REFACTOR ========================= */
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
    /* ==========================
        Tasks:
            1) Cerco se esiste il Docker in questione
                1.1) Esiste, non faccio nulla
                1.2) Non Esiste, creo il docker (Vedesi Nota #1)
            2) Cerco tra i container dell'entità destinazione se vi è o no quello della relazione in questione
                2.1) Esiste, aggiungo la nuova relazione
                2.2) Non Esiste, aggiungo un nuovo container
                2.3) Gestione ranking node
            3) Cerco tra i container dell'entità sorgente se vi è o no quello della relazione in questione
                2.1) Esiste, aggiungo la nuova relazione
                2.2) Non Esiste, aggiungo un nuovo container
        Nota:
            1) Viene passato l'head, aggiornare se viene creato un nuovo docker
     ========================== */

    /* ========================== INIZIO REFACTOR ========================== */
    /* 1) Cerco se esiste il Docker in questione */
    container_relation_t *temp_cr = NULL;
    docker_ranking_t *target_docker = docker_search(rel_str, docker_ranking_head_p);

    if (target_docker == NULL)
    {
        /* 1.2) Non Esiste, creo il docker */

        target_docker = malloc(sizeof(docker_ranking_t));
        target_docker->container_floor_ranking_p = malloc(sizeof(container_floor_ranking_t));
        target_docker->rel_name = malloc(sizeof(char) * (strlen(rel_str) + 1));
        strcpy(target_docker->rel_name, rel_str);
        (target_docker->container_floor_ranking_p)->docker_ranking_p = target_docker;
        (target_docker->container_floor_ranking_p)->max_floor_ranking_p = NULL;
        (target_docker->container_floor_ranking_p)->min_floor_ranking_p = NULL;
        target_docker->next_p = docker_ranking_head_p;
        target_docker->prev_p = NULL;
        if (docker_ranking_head_p != NULL)
            docker_ranking_head_p->prev_p = target_docker;
        docker_ranking_head_p = target_docker;
    }
    // Inizializzo la relazione da aggiungere

    info_relation_t *to_add_info_relation = malloc(sizeof(info_relation_t));
    to_add_info_relation->dest_prev_p = NULL;
    to_add_info_relation->src_prev_p = NULL;

    /* 2) Cerco tra i container dell'entità destinazione se vi è o no quello della relazione in questione */

    temp_cr = dest_ie->container_relation_head_p;

    while (temp_cr != NULL && strcmp(rel_str, temp_cr->rel_name) != 0)
        temp_cr = temp_cr->next_p;

    if (temp_cr == NULL)
    {

        /* 2.2) Non Esiste, aggiungo un nuovo container */

        temp_cr = malloc(sizeof(container_relation_t));
        temp_cr->rel_name = target_docker->rel_name;
        temp_cr->in_relation_list_head_p = NULL;
        temp_cr->out_relation_list_head_p = NULL;
        temp_cr->entity_p = dest_ie;
        temp_cr->prev_p = NULL;
        temp_cr->ranking_node_p = NULL;
        temp_cr->next_p = dest_ie->container_relation_head_p;
        if (dest_ie->container_relation_head_p != NULL)
            (dest_ie->container_relation_head_p)->prev_p = temp_cr;
        dest_ie->container_relation_head_p = temp_cr;
    }

    /* 2.1) Esiste, aggiungo la nuova relazione */

    // Continuous inizializzazione info relation da aggiungere
    to_add_info_relation->dest_container_relation_p = temp_cr;
    to_add_info_relation->dest_next_p = temp_cr->in_relation_list_head_p;
    if (temp_cr->in_relation_list_head_p != NULL)
        (temp_cr->in_relation_list_head_p)->dest_prev_p = to_add_info_relation;
    temp_cr->in_relation_list_head_p = to_add_info_relation;

    /* 2.3) Gestione ranking node */
    if (temp_cr->ranking_node_p == NULL)
    {
        ranking_node_t *rn_to_add = malloc(sizeof(ranking_node_t));
        rn_to_add->entity_p = dest_ie;
        temp_cr->ranking_node_p = rn_to_add;
        sys_create_ranking_node(rn_to_add, target_docker->container_floor_ranking_p);
    }
    else
        ranking_node_scala(temp_cr, 1);

    /* 3) Cerco tra i container dell'entità sorgente se vi è o no quello della relazione in questione */

    temp_cr = src_ie->container_relation_head_p;

    while (temp_cr != NULL && strcmp(rel_str, temp_cr->rel_name) != 0)
        temp_cr = temp_cr->next_p;

    if (temp_cr == NULL)
    {

        /* 3.2) Non Esiste, aggiungo un nuovo container */

        temp_cr = malloc(sizeof(container_relation_t));
        temp_cr->rel_name = target_docker->rel_name;
        temp_cr->in_relation_list_head_p = NULL;
        temp_cr->out_relation_list_head_p = NULL;
        temp_cr->entity_p = src_ie;
        temp_cr->prev_p = NULL;
        temp_cr->ranking_node_p = NULL;
        temp_cr->next_p = src_ie->container_relation_head_p;
        if (src_ie->container_relation_head_p != NULL)
            (src_ie->container_relation_head_p)->prev_p = temp_cr;
        src_ie->container_relation_head_p = temp_cr;
    }
    /* 3.1) Esiste, aggiungo la nuova relazione */

    // Continuous inizializzazione info relation da aggiungere
    to_add_info_relation->src_container_relation_p = temp_cr;
    to_add_info_relation->src_next_p = temp_cr->out_relation_list_head_p;
    if (temp_cr->out_relation_list_head_p != NULL)
        (temp_cr->out_relation_list_head_p)->src_prev_p = to_add_info_relation;
    temp_cr->out_relation_list_head_p = to_add_info_relation;

    return docker_ranking_head_p;
    /* ========================== FINE REFACTOR ========================== */
}
docker_ranking_t *sys_call_add_relation(char *src_str, char *dest_str, char *rel_str, info_entity_menager_t *hashtable[], docker_ranking_t *docker_ranking_head_p)
{
    /* =====================
        Tasks:
            1) Lettura Input
            2) Ricerca esistenza entità:
                2.1) Entità Sorgente
                2.2) Entità Destinazione
            3) Ricerca esistenza relazione
                3.1) Non esiste, allora aggiungo la relazione
                3.2) Esiste, allora non faccio nulla
        Nota: 
            1) Viene passata l'head del docker, quindi va restituito aggiornato in cui si crei un nuovo docker
     ===================== */

    /* ================== INIZIO REFACTOR ================== */
    /* 1) Lettura Input */
    // Prendo Input da tastiera e calcolo i rispettivi "Hashvalue"
    scanf("%s %s %s", src_str, dest_str, rel_str);
    int src_hash_value = hash(src_str) % modulo;
    int dest_hash_value = hash(dest_str) % modulo;

    // Accedo alla list nella Hashtable entità
    info_entity_menager_t *src_iem = hashtable[src_hash_value], *dest_iem = hashtable[dest_hash_value];

    /* 2) Ricerca esistenza entità */
    /* 2.1) Entità Sorgente */
    // Cerco entità sorgente
    while (src_iem != NULL && strcmp(src_str, (src_iem->entity_p)->entity_name) != 0)
        src_iem = src_iem->next_p;
    if (src_iem != NULL)
    {
        // Esiste l'entità sorgente quindi procedo alla ricerca dell'entità destinazione

        /* 2.2) Entità Destinazione */
        // Cerco entità destinazione
        while (dest_iem != NULL && strcmp(dest_str, (dest_iem->entity_p)->entity_name) != 0)
            dest_iem = dest_iem->next_p;

        if (dest_iem != NULL)
        {
            // Esiste anche l'entità destinazione oltre all'entità sorgente, procedo...

            /* 3) Ricerca esistenza relazione */
            container_relation_t *temp_cr = (dest_iem->entity_p)->container_relation_head_p;
            while (temp_cr != NULL && (strcmp(rel_str, temp_cr->rel_name) != 0))
                temp_cr = temp_cr->next_p;

            if (temp_cr != NULL)
            {
                // Esiste il container relazione nell'entità destinazione, va verificato se tra quelle in entrata vi è o no quello in aggiunta
                info_relation_t *temp_ir = temp_cr->in_relation_list_head_p;
                //todo forse poco efficiente "strcmp"
                // Cerco relazione nel container dell'entità sorgente
                while (temp_ir != NULL && strcmp(src_str, ((temp_ir->src_container_relation_p)->entity_p)->entity_name) != 0)
                    temp_ir = temp_ir->dest_next_p;

                if (temp_ir == NULL)
                {
                    // Non esiste la relazione, aggiungo la relazione
                    // Nota: restituisco docker aggiornato
                    docker_ranking_head_p = add_relation(src_iem->entity_p, dest_iem->entity_p, rel_str, docker_ranking_head_p);
                }
            }
            else
            {
                // Non esiste il container relazione nell'entità sorgente, aggiungo la relazione
                // Nota: restituisco docker aggiornato
                docker_ranking_head_p = add_relation(src_iem->entity_p, dest_iem->entity_p, rel_str, docker_ranking_head_p);
            }
        }
    }
    return docker_ranking_head_p;
    /* ================== FINE REFACTOR ================== */
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
    while (docker_ranking_head_p != NULL && strcmp(str, docker_ranking_head_p->rel_name) != 0)
        docker_ranking_head_p = docker_ranking_head_p->next_p;
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
    char input[512] = "addrel", input2[512], input3[512];
    int i = 1;
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
                        docker_head = sys_call_add_relation(input, input2, input3, entity_hashtable, docker_head); // V Refactor 1.0
                    else
                        sys_call_add_entity(input, entity_hashtable); // V Refactor 1.0
                }
                else
                {
                    if (input[3] == 'e')
                        docker_head = sys_call_delete_entity(input, entity_hashtable, docker_head); // Refactor 1.0
                    else
                        docker_head = sys_delete_relation(input, input2, input3, docker_head, entity_hashtable); // V Refactor 1.0
                }
            }
            else
            {
                docker_head = sys_call_report(docker_head); // 1.0.0
                //sys_call_print_docker(docker_head);
                //sys_call_print_entity_hashtable(entity_hashtable);
            }
            //printf("%d\n", i++);
            input[0] = 'r';
        }
    }
}
