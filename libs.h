#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/sysinfo.h>

// directory
#include <dirent.h>

#include <fcntl.h>

// stat
#include  <sys/stat.h>	
#include <unistd.h>	

#include <signal.h>

#include <time.h>

#define TRUE 0
#define FALSE -1
//define di possibili option per la funzione proc_finder
#define NO_OPT -1
#define PID_FNDR 1

#define MAX_BUFFER_DIM 1024

#define ONE_KiB 1024         // 1 KiB
#define ONE_MiB 1048576      // 1 MiB

#define print_error(msg)   do { perror(msg); printf("\n"); } while(0)

//strutture dati

    //Processo
    typedef struct proc {
        pid_t pid;
        char* command;
        char status;
        int group_id;
        long priority;
        unsigned long vsize;
        long int rss;
        float cpu_u;
        float mem_u;
        struct proc* prev;
        struct proc* next;
    } proc;

    //Lista di processi    
    typedef struct proc_list {
        proc* head;
        proc* tail;
        int size;
    } proc_list;


//prototipi

    //I metodi sono dichiarati extern perchè sono definiti in moduli esterni

    //struct proc
    /* definiti nel modulo libs.c */
    
    //Restituisce il PID del proc p
    pid_t getPID(const proc* p);
    //Restituisce lo status del proc p
    char getStatus(const proc* p);
    //Restituisce la priorità del proc p
    long getPriority(const proc* p);
    //Restituisce l'utilizzo della CPU da parte del proc p
    float getCPUUsage(const proc* p);
    //Restituisce l'utilizzo della memoria da parte del proc p
    float getMemUsage(const proc* p);
    //Restituisce l'eventuale predecessore del proc p
    proc* getPrev(const proc* p);
    //Restituisce l'eventuale successore del proc p
    proc* getNext(const proc* p);
    //Stampa il proc p
    void proc_printer(const proc* p);

    //Setta i valori degli attributi del proc
    proc* setProc(char** fields);// proc* setProc(char** fields, char** mem_info);
        
    //struct proc_list
    /*funzioni caratteristiche delle liste*/

    //Inizializzazione lista vuota
    void list_init(proc_list* l);
    //Ricerca del processo p all'interno della proc_list l
    proc* proc_finder(const proc_list* l, proc* p, int option);
    //Inserimento in coda del proc p in proc_list l [riportare a void]
    void insert(proc_list* l, proc* p);
    //Rimozione del proc p dalla proc_list l [riportare a void]
    void remove_elem(proc_list* l, proc* p);
    //Distruttore della proc_list l [riportare a void]
    void list_destroyer(proc_list* l);
    //Stampa la proc_list l
    void list_printer(proc_list* l);
    //Ordina la proc_list l
    proc_list* sort(proc_list* l);

    //Verifica che una directory nel filesystem '/proc' rappresenta un processo
    int is_PIDFolder(char* path, int* pid);
    //Lettura dei campi di interesse con posizione contenuta in field_pos dal file con filename path 
    char** read_fields_from_file(char* path, int* field_pos, int lenght);
    //Restituisce la proc_list di tutti i processi presenti nel filesystem '/proc'
    proc_list* listing_proc();    
    //Classifica i processi in base al proprio stato
    void tasks_info(const proc_list* src);

    //Stampa a terminale l'ora locale
    void local_time();
    //Stampa le informazioni di sistema [uptime, mem stats, swap stats]
    void get_sys_info(struct sysinfo *info);

    //Handler di segnali
    void signal_handler(int signal);
