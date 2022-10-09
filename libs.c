#include "libs.h"

//Funzione che stampa l'orario attuale
void local_time () {
    int h, min, sec;
    time_t t;
    //orario attuale
    time(&t);
    //orario locale
    struct tm* loc_t = localtime(&t);
    h = loc_t->tm_hour;
    min = loc_t->tm_min;
    sec = loc_t->tm_sec;
    printf("%02d:%02d:%02d", h, min, sec);
}

int is_NaN(const char* s) {
    int i = 0;
    while (s[i]) {
        if (s[i] >= '0' && s[i] <= '9')
            return FALSE;
        else return TRUE;
    }
    return FALSE;
}

void file_reader(const char* path) {
    char buf[200];
    char* res;

    FILE* fd;
    fd = fopen(path, "r");
    if (fd == NULL) {
        perror("Errore in apertura del file");
        exit(1);
    }

    while(1) {
        res = fgets(buf, 200, fd);
        if (res == NULL) {
            break;
        }
        
    }

    long x, y;
    fscanf(fd, "%li %li", &x, &y);
    
    //local time
    long x_sec = x%60;
    long x_min = x_sec/60;
    long x_hrs = x_min/60;

    //uptime
    long y_sec = y%60;
    long y_min = y_sec/60;
    long y_hrs = y_min/60;

    printf("local time: %li:%li:%li || uptime: %li:%li:%li ", x_hrs, x_min, x_sec, y_hrs, y_min, y_sec);

}


//Funzioni legate alla struttura dati 'proc'
int getPID(const proc* p) {
    return p->pid;
}

char getStatus(const proc* p) {
    return p->status;
}

long getPriority(const proc* p) {
    return p->priority;
}

float getCPUUsage(const proc* p) {
    return p->cpu_u;
}

float getMemUsage(const proc* p) {
    return p->mem_u;
}

proc* getPrev(const proc* p) {
    if (p->prev) return p->prev;
    else return NULL;
}

proc* getNext(const proc* p) {
    if (p->next) return p->next;
    else return NULL;
}


//DA MODIFICARE dato che le info dei processi vengono prelevate da file
//N.B.: implementazione momentanea per testare il corretto funzionamento dei metodi delle liste
proc* set_proc(pid_t pid, char status, long priority, float cpu_u, float mem_u) {
    proc* p = (proc*) malloc(sizeof(proc));
    p->pid = pid;
    p->status = status;
    p->priority = priority;
    p->cpu_u = cpu_u;
    p->mem_u = mem_u;
    p->prev = NULL;
    p->next = NULL;

    return p;
}

void list_init(proc_list* l)  {
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

//DA MODIFICARE
//Aggiungere elementi rilevanti per il programma e eventualmente cambiare precisione dei float
void list_printer(proc_list* l) {
    if (l->size == 0) {
        printf("La lista è vuota...\n\n");
    } else {
        proc* tmp = l->head;
        while(tmp) {
            printf("PID: %d | status: %c | priority: %lu | cpu_u: %.2f | mem_u: %.2f\n", tmp->pid, tmp->status, tmp->priority, tmp->cpu_u, tmp->mem_u);
            tmp = tmp->next;
        }
    }

    printf("\n");

}

proc* proc_finder(const proc_list* l, proc* p) {
    proc* tmp = l->head;
    while(tmp) {
        if (tmp == p) return tmp;
        tmp = tmp->next;
    }
    return NULL;
}

void insert(proc_list* l, proc* p) {

    if (l->size == 0) {
        l->head = (proc*)malloc(sizeof(proc));
        l->head = p;
        l->head->next = NULL;
        l->tail = l->head;
        l->size++;
        return;
    }

    //verifica che il processo non sia contenuto nella lista
    if (p == proc_finder(l, p)) {
        printf("Il processo %d è già all'interno della lista\n", p->pid);
        return;
    }

    proc* tmp = (proc*) malloc(sizeof(proc));
    tmp = l->head;

    while(tmp) {

        if (tmp->next == NULL) {
            p->prev = tmp;
            tmp->next = p;
            l->tail = p;
            l->size++;
            return;
        } else tmp = tmp->next;
    }

    free(tmp);
}

void remove_elem(proc_list* l, proc* p) {
    if (l->size == 0) {
        printf("La lista è vuota..\n\n");
        return;
    }

    proc* tmp = (proc*) malloc(sizeof(proc));
    tmp = l->head;

    while(tmp) {

        if (tmp == p) {
            proc* prev = tmp->prev;
            proc* next = tmp->next;
            if (prev != NULL) prev->next = next;
            if (next != NULL) next->prev = prev;
            if (tmp == l->tail) l->tail = prev;
            l->size--;
            free(tmp);
        }
        tmp = tmp->next;
    }

    free(tmp);
}

void list_destroyer(proc_list* l) {
    
    if (l->size == 0) {
        printf("La lista è vuota..\n\n");
        return;
    }

    proc* tmp = (proc*) malloc(sizeof(proc));
    tmp = l->head;
    while(tmp) {
                     
        if (tmp->next == NULL) {
            remove_elem(l, tmp);
            if (l->size == 0) l->head = NULL;
            tmp = tmp->prev;
        } else tmp = tmp->next;
               
    } 

}

proc_list* sort(proc_list* l) {
    //COMPLETARE
    return l;
}

char* read_row(int fd, int row, char* info) {

    if (row <= 0) {
        printf("Errore! La riga deve avere un valore maggiore di 0\n");
        return NULL;
    }

    char* buf = (char*) malloc(1024*sizeof(char));

    // Opzione 1 --> Non funziona
    char* c = (char*) malloc(sizeof(char)); 
    int ret = 0;
    int offset = 0;
    int n_bytes = 0;

    while(c[0] != ' ') {
        ret = read(fd, c, 1);
        //printf("ret: %d\n", ret);
        if (ret == -1) perror("Errore in lettura file..");
        offset++;
        row--;
    }

    while (row == 0) {
            fd = lseek(fd, offset, 0);
            ret = read(fd, c, 1);
            //printf("ret: %d\n", ret);
            if (ret == -1) {
                if (errno == EINTR) continue;
                else perror("Errore in lettura file..");
            }
            n_bytes++;
            if (c[0] == ' ') {
                ret = read(fd, buf, n_bytes);
                //printf("ret: %d\n", ret);
                if (ret == -1) perror("Errore in lettura file..");
                return buf;
            }
        }

    //Opzione 2 --> funziona
    
    // char* res = (char*) malloc(1024*sizeof(char));

    // while(row > 0) {
    //     res = fgets(buf, 1024, fd);
    //     if (res == NULL) {
    //         ferror(fd);
    //         return NULL;
    //     }
    //     row--;
    // }

    //DA MODIFICARE: Bisogna rimuovere l'eventuale intestazione del dato raccolto
    //              [Es: Se su file si legge "PID = 1" bisogna raccogliere solo "1"]
    //potenziale idea n.1 --> funziona
    // if (row == 0) {
    //     if (strstr(buf, info)) {
    //         int i = 0;
    //         while(i < strlen(info)) {
    //             buf[i] = buf[i + strlen(info)];
    //             i++;
    //         }
    //     }
    //     return buf;
    // }

    if (row > 0) printf("Errore! Il file ha numero di righe minore di %d\n", row);
    return buf;

}
