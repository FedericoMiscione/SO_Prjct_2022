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
        char pad[40];        //padding to 64 bit
    } proc_list;


//prototipi
/* definiti nel modulo libs.c */

    //struct proc    
    
    //Iniizializza i campi del proc p a NULL
    void initProc(proc* p);
    //Stampa il proc p
    void proc_printer(const proc* p);
    //Setta i valori degli attributi del proc
    void setProc(proc *p,char** fields);
        
    //struct proc_list

    //Inizializzazione lista vuota
    void list_init(proc_list* l);
    //Ricerca del processo p all'interno della proc_list l
    proc* proc_finder(const proc_list* l, pid_t pid);
    //Inserimento in coda del proc p in proc_list l
    void insert(proc_list* l, proc* p);
    //Rimozione del proc p dalla proc_list l
    void remove_elem(proc_list* l, proc* p);
    //Deallocazione della proc_list l
    void list_destroyer(proc_list* l);
    //Stampa la proc_list l
    void list_printer(proc_list* l);


    //Verifica che una directory nel filesystem '/proc' rappresenta un processo
    int is_PIDFolder(char* path, int* pid);
    //Lettura dei campi di interesse con posizione contenuta in field_pos dal file con filename path e separatore delim.
    //L'allocazione del char** ritornato avviene internamente. In caso di fallimento restituisce NULL.
    char** create_fields_from_file(char* path, int* field_pos, int lenght, char* delim);
    //Dealloca i fields creati per la raccolta dei dati dai file
    void destroy_fields(char** fields, int lenght);
    //Restituisce la proc_list di tutti i processi presenti nel filesystem '/proc'. La proc_list è allocata all'interno del metodo
    proc_list* listing_proc();    

    //Stampa l'ora locale
    void local_time();
    //Stampa le informazioni di sistema [uptime, mem stats, swap stats]
    void get_sys_info(struct sysinfo *info);
    //Stampa le statistiche riguardanti la CPU
    void get_cpu_stats();
    //Classifica i processi in base al proprio stato
    void tasks_info(const proc_list* src);
