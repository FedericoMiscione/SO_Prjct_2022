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

    //Lista di processi o...
    
    typedef struct proc_list {
        proc* head;
        proc* tail;
        int size;
    } proc_list;


    //...è più efficiente un array di processi???


//prototipi

    //I metodi sono dichiarati extern perchè sono definiti in moduli esterni

    //struct proc
    /* definiti nel modulo libs.c */
    pid_t getPID(const proc* p);
    char getStatus(const proc* p);
    long getPriority(const proc* p);
    float getCPUUsage(const proc* p);
    float getMemUsage(const proc* p);
    proc* getPrev(const proc* p);
    proc* getNext(const proc* p);

    //forse potrebbero essere implementate in un unico metodo
    proc* set_proc(pid_t pid, char status, long priority, float cpu_u, float mem_u);
    
    //struct proc_list
    /*funzioni caratteristiche delle liste*/
    void list_init(proc_list* l);
    proc* proc_finder(const proc_list* l, proc* p);
    void insert(proc_list* l, proc* p);
    void remove_elem(proc_list* l, proc* p);
    void list_destroyer(proc_list* l);
    void list_printer(proc_list* l);

    //altro
    extern int is_NaN(const char* s);           //probabilmente inutile
    extern void local_time();                   
    extern void file_reader(const char* path);  //probabilmente inutile

    char* read_row(FILE* fd, int row /*, char* info */);
