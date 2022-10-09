#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>

// directory
#include <dirent.h>

#include <fcntl.h>

// stat
#include  <sys/stat.h>	
#include <unistd.h>	

#include <string.h>

#include <time.h>

//define
#define TRUE 0
#define FALSE -1
#define SUCCESS 0
#define FAILURE -1
#define NOT_FOUND -1    //potrebbe essere inutile
#define READ_ERROR -1   //potrebbe essere inutile

//DA MODIFICARE: la macro non deve portare alla terminazione del programma,
//               ma (se possibile) solamente notificare il problema proseguendo
//               l'esecuzione del software, altrimenti eliminare la macro
//macro per la gestione degli errori
#define errors_handler(msg) do {perror(msg); exit(EXIT_FAILURE);} while(0)

//strutture dati

    //Processo
    typedef struct proc {
        pid_t pid;
        char status;
        long priority;
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

    //Imposta i valori degli attributi del proc
    proc* set_proc(pid_t pid, char status, long priority, float cpu_u, float mem_u);
    //Imposta il valore dell'attributo 'position' del processo proc
    void setPosition(const proc_list* l, proc* p);
    
    //struct proc_list
    /*funzioni caratteristiche delle liste*/

    //Inizializzazione lista vuota
    void list_init(proc_list* l);
    //Ricerca del processo p all'interno della proc_list l
    proc* proc_finder(const proc_list* l, proc* p);
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

    //altro
    int is_NaN(const char* s);           //probabilmente inutile
    void local_time();                   
    void file_reader(const char* path);  //probabilmente inutile

    char* read_row(int fd, int row, char* info);
