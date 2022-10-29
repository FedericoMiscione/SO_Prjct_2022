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

void get_sys_info(struct sysinfo* info) {
    
    //uptime
    unsigned long uptime = info->uptime;
    printf("|| up: %li ||\n", uptime);
    
    //mem stats
    unsigned long total_mem = info->totalram;
    unsigned long free_mem = info->freeram;
    unsigned long buff_mem = info->bufferram;
    printf("Mem Total: %.2lf MiB | Mem Free: %.2lf MiB | Buf: %.2lf MiB\n", (double) total_mem/ONE_MiB, (double) free_mem/ONE_MiB, (double) buff_mem/ONE_MiB);

    //swap stats
    unsigned long total_swap = info->totalswap;
    unsigned long free_swap = info->freeswap;
    printf("Swap Total: %.2lf MiB | Swap Free: %.2lf MiB\n", (double) total_swap/ONE_MiB, (double) free_swap/ONE_MiB);

}

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

void proc_printer(const proc* p) {
    printf("PID: %d | command: %s | status: %c | priority: %ld | group_ID: %d | virt: %lu KiB | rss: %ld KiB | cpu_u: %.2f %% | mem_u: %.2f %%\n", 
                    p->pid, p->command, p->status, p->priority, p->group_id, p->vsize, p->rss, p->cpu_u, p->mem_u);
}

//DA RIVEDERE: cpu_usage dovrebbe essere corretto ma va controllato
proc* setProc(char** fields) {

    proc* p = (proc*)malloc(sizeof(proc));

    p->pid = atoi(fields[0]);
    p->command = fields[1];
    p->status = fields[2][0];
    p->group_id = atoi(fields[3]);
    p->priority = atol(fields[6]);
    p->vsize = (strtoul(fields[8], NULL, 0))/ONE_KiB;

    struct sysinfo info;
    sysinfo(&info);
    int clk_tck = sysconf(_SC_CLK_TCK);                             // Numero di clock ticks per secondo

    unsigned long utime = strtoul(fields[4], NULL, 0);              // (*) base = 0 implica conversione in decimale
    unsigned long stime = strtoul(fields[5], NULL, 0);              // (*)
    unsigned long ttime = utime + stime;                            // Moltiplicato x100 per riportare il valore in percentuale
    double ttime_s = ttime/clk_tck;                                 // Tempo totale del processo in secondi

    unsigned long uptime = info.uptime;                             // uptime in secondi
    unsigned long long start_time = strtoull(fields[7], NULL, 0);   // (*)
    double start_time_s = start_time/clk_tck;                       // start time del processo in
    unsigned long elapsed_time = uptime - start_time_s;

    double cpu_usage = (double) ttime_s / (double) elapsed_time;
    cpu_usage*=100;
    p->cpu_u = cpu_usage;

    long int total_mem = (long int) info.totalram;

    int page_size = getpagesize();
    long int proc_pages = atol(fields[9]);
    long int proc_mem = page_size*proc_pages;

    double mem_usage = 0;
    mem_usage = ((double) proc_mem) / ((double) total_mem);
    mem_usage*=100;
    p->rss = proc_mem/ONE_KiB;
    p->mem_u = mem_usage;
    
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
            printf("PID: %d | command: %s | status: %c | priority: %ld | group_ID: %d | virt: %lu KiB | rss: %ld KiB | cpu_u: %.2f %% | mem_u: %.2f %%\n", 
                    tmp->pid, tmp->command, tmp->status, tmp->priority, tmp->group_id, tmp->vsize, tmp->rss, tmp->cpu_u,   tmp->mem_u);
            tmp = tmp->next;
        }
    }

    printf("\n");

}

proc* proc_finder(const proc_list* l, proc* p, int option) {
    proc* tmp = l->head;
    while(tmp) {
        
        if (option == NO_OPT) {
            if (tmp == p) return tmp;
            tmp = tmp->next;
        } else if (option == PID_FNDR) {
            if (tmp->pid == p->pid) return tmp;
            tmp = tmp->next;
        }
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
    if (p == proc_finder(l, p, NO_OPT)) {
        printf("Il processo %d è già all'interno della lista\n", p->pid);
        return;
    }

    proc* tmp = (proc*) malloc(sizeof(proc));
    tmp = l->tail;

    if (tmp->next == NULL) {
        p->prev = tmp;
        tmp->next = p;
        p->next = NULL;
        l->tail = p;
        l->size++;
        return;
    }

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

//DA COMPLETARE
proc_list* sort(proc_list* l) {
    return l;
}

int is_PIDFolder(char* path, int* pid) {
    int i = 0;
    const int lenght = strlen(path);

    for (i = 0; i < lenght; i++) {
        if (path[i] < '0' || path[i] > '9')
            return 0;
    }    
    
    if (lenght == i) *pid = atoi(path);
    else *pid = 0;
    
    return 1;
}

char** read_fields_from_file(char* path, int* field_pos, int lenght) {
    
    FILE* fp = fopen(path, "r");
    if (fp != NULL) {
        char** fields = (char**) malloc(lenght*sizeof(char*));
        char c[2];
        int j = 0;
        int delim_pos = -1;
        int file_position = 0;
        int current_field_pos = 1;                

        while (fgets(c, sizeof(c), fp) != NULL) {
                       
            file_position++;
            if (strcmp(c, " ") == 0) {

                for (int i = 0; i < lenght; i++) {

                    if (field_pos[i] == current_field_pos) {
                        fseek(fp, ++delim_pos, SEEK_SET);
                        int field_len = file_position - delim_pos - 1;
                        char* field = (char*) malloc((field_len + 1)*sizeof(char));

                        if (fgets(field, field_len + 1, fp) == NULL) {
                            free(field);
                            break;
                        }

                        fields[j] = field;
                        fseek(fp, 1, SEEK_CUR);
                        j++;
                        break;
                    }

                }

                current_field_pos++;
                delim_pos = file_position-1;
            }

        }

        fclose(fp);
        return fields;
    }

    return NULL;
}

proc_list* listing_proc() {
    DIR* dir;
    struct dirent* entry;

    if (!(dir = opendir("/proc")))
        return NULL;

    proc_list* l = (proc_list*) malloc(sizeof(proc_list));
    list_init(l);

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            int pid = 0;

            if (is_PIDFolder(entry->d_name, &pid) == 0)
                continue;            

            char path[MAX_BUFFER_DIM];
            snprintf(path, MAX_BUFFER_DIM, "/proc/%d/stat", pid);
            int field_pos_stat[] = {1, 2, 3, 5, 14, 15, 18, 22, 23, 24};
            char** proc_fields = read_fields_from_file(path, field_pos_stat, sizeof(field_pos_stat)/sizeof(int));

            snprintf(path, MAX_BUFFER_DIM, "/proc/%d/statm", pid);

            if (proc_fields != NULL ) {
                proc* p = setProc(proc_fields);
                insert(l, p);
            }
        }
    }

    closedir(dir);
    return l;
}

void tasks_info(const proc_list* src) {
    int running_cnt = 0;
    int sleeping_cnt = 0;
    int stopped_cnt = 0;
    int zombie_cnt = 0;

    proc* tmp = (proc*) malloc(sizeof(proc));
    tmp = src->head;

    while(tmp) {
        if (tmp->status == 'R') running_cnt++;
        else if (tmp->status == 'Z') zombie_cnt++;
        else if (tmp->status == 'T') stopped_cnt++;
        else sleeping_cnt++;

        tmp = tmp->next;
    }

    printf("Running: %d, Sleeping: %d, Stopped: %d, Zombie: %d\n", running_cnt, sleeping_cnt, stopped_cnt, zombie_cnt);

}