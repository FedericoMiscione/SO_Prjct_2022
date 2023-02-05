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
    if (uptime/3600 < 24) printf(" || up %.2ld:%.2ld:%.2ld ||", uptime/3600, (uptime/60)%60, uptime%60);
    else printf(" || up %d days ||", (int) (uptime/3600)/24);

    //load averages
    double loads[3];
    getloadavg(loads, 3);
    printf(" load averages: %.2f, %.2f, %.2f ||\n", loads[0], loads[1], loads[2]);
    
    //cpu stats
    printf("%%CPU: ");
    get_cpu_stats();

    int fields_pos[] = {1, 2, 3, 4, 5, 15, 16, 24};
    char** mem_stats = read_fields_from_file("/proc/meminfo", fields_pos, sizeof(fields_pos)/sizeof(int), "\n");

    //mem stats
    double total_mem = (double) atol(strtok(strtok(strtok(mem_stats[0], "MemTotal:"), "kB"), " "))/ONE_KiB;
    double free_mem = (double) atol(strtok(strtok(strtok(mem_stats[1], "MemFree:"), "kB"), " "))/ONE_KiB;
    double buff_mem = (double) atol(strtok(strtok(strtok(mem_stats[3], "Buffers:"), "kB"), " "))/ONE_KiB;
    double cached_mem = (double) atol(strtok(strtok(strtok(mem_stats[4], "Cached:"), "kB"), " "))/ONE_KiB;
    double srecl_mem = (double) atol(strtok(strtok(strtok(mem_stats[7], "SReclaimable:"), "kB"), " "))/ONE_KiB;
    double cach_buff = cached_mem + srecl_mem + buff_mem;
    double used_mem = total_mem - free_mem - cach_buff;
    printf("Mem:  total %8.2lf MiB, free %8.2lf MiB, used %8.2lf MiB, buf/cache %8.2lf MiB\n", total_mem, free_mem, used_mem, cach_buff);

    //swap stats
    double total_swap = (double) atol(strtok(strtok(strtok(mem_stats[5], "SwapTotal:"), "kB"), " "))/ONE_KiB;
    double free_swap = (double) atol(strtok(strtok(strtok(mem_stats[6], "SwapFree:"), "kB"), " "))/ONE_KiB;
    double used_swap = total_swap - free_swap;
    unsigned long avail_mem = atol(strtok(strtok(strtok(mem_stats[2], "MemAvailable:"), "kB"), " "))/ONE_KiB;
    printf("Swap: total %8.2lf MiB, free %8.2lf MiB, used %7.2lf MiB. ", total_swap, free_swap, used_swap);
    printf("Avail Mem: %8.2lf MiB\n", (double) avail_mem);
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
    printf("\nPID: %5d | status: %2c | priority: %5ld | virt: %10lu KiB | rss: %10ld KiB | cpu_u: %5.2f %% | mem_u: %5.2f %% | command: %20s\n\n", 
                    p->pid, p->status, p->priority, p->vsize, p->rss, p->cpu_u, p->mem_u, p->command);
}

proc* setProc(char** fields) {

    proc* p = (proc*)malloc(sizeof(proc));

    p->pid = atoi(fields[0]);
    p->command = fields[1];
    p->status = fields[2][0];
    p->priority = atol(fields[5]);
    p->vsize = (strtoul(fields[7], NULL, 0))/ONE_KiB;

    struct sysinfo info;
    sysinfo(&info);
    int clk_tck = sysconf(_SC_CLK_TCK);                             // Numero di clock ticks per secondo

    unsigned long utime = strtoul(fields[3], NULL, 0);              // (*) base = 0 implica conversione in decimale
    unsigned long stime = strtoul(fields[4], NULL, 0);              // (*)
    unsigned long ttime = utime + stime;                            // Moltiplicato x100 per riportare il valore in percentuale
    double ttime_s = ttime/clk_tck;                                 // Tempo totale del processo in secondi

    unsigned long uptime = info.uptime;                             // uptime in secondi
    unsigned long long start_time = strtoull(fields[6], NULL, 0);   // (*)
    double start_time_s = start_time/clk_tck;                       // start time del processo in
    unsigned long elapsed_time = uptime - start_time_s;

    double cpu_usage = (double) ttime_s / (double) elapsed_time;
    cpu_usage*=100;
    p->cpu_u = cpu_usage;

    long int total_mem = (long int) info.totalram;

    int page_size = getpagesize();
    long long int proc_pages = atol(fields[8]);
    long long int proc_mem = page_size*proc_pages;

    double mem_usage = 0;
    mem_usage = ((double) proc_mem) / ((double) total_mem)*100;
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
//Aggiungere/rimuovere elementi rilevanti/non rilevanti per il programma e eventualmente cambiare precisione dei float
void list_printer(proc_list* l) {
    if (l->size == 0) {
        printf("La lista è vuota...\n\n");
    } else {
        proc* tmp = l->head;
        while(tmp) {
            printf("PID: %5d | status: %2c | priority: %5ld | virt: %10lu KiB | rss: %10ld KiB | cpu_u: %5.2f %% | mem_u: %5.2f %% | command: %20s\n", 
                    tmp->pid, tmp->status, tmp->priority, tmp->vsize, tmp->rss, tmp->cpu_u, tmp->mem_u, tmp->command);
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

char** read_fields_from_file(char* path, int* field_pos, int lenght, char* delim) {
    
    FILE* fp = fopen(path, "r");
    if (fp != NULL) {
        char** fields = (char**) malloc(lenght*sizeof(char*));
        char c[2];
        int j = 0;
        int sem = 0;
        int delim_pos = -1;
        int file_position = 0;
        int current_field_pos = 1;               

        while (fgets(c, sizeof(c), fp) != NULL) {
                       
            file_position++;

            if (strcmp(c, "(") == 0) sem = 1;
            
            if (strcmp(c, ")") == 0) sem = 0;
            
            if (strcmp(c, delim) == 0) {

                if (sem == 0) {

                    for (int i = 0; i < lenght; i++) {

                        if (field_pos[i] == current_field_pos) {
                                                    
                            fseek(fp, ++delim_pos, SEEK_SET);
                            int field_len = file_position - delim_pos - 1;
                            char* field = (char*) malloc((field_len + 1)*sizeof(char));

                            if (fgets(field, field_len + 1, fp) == NULL) {
                                free(field);
                                break;
                            }

                            fields[j] = strtok(strtok(field, "("), ")");
                            j++;
                            fseek(fp, 1, SEEK_CUR);
                            break;
                        }
                    }
                } else continue;

                current_field_pos++;
                delim_pos = file_position-1;
            }
            
        }

        fclose(fp);
        return fields;
    }

    return NULL;
}

void get_cpu_stats() {
    char path[MAX_BUFFER_DIM];
    snprintf(path, MAX_BUFFER_DIM, "/proc/stat");
    int field_pos_stat[8] = {3, 4, 5, 6, 7, 8, 9, 10};
    char** fields = read_fields_from_file(path, field_pos_stat, sizeof(field_pos_stat)/sizeof(int), " ");

    unsigned long total_cpu_time = 0;
    for (int i = 0; i < 8; i++) {
        total_cpu_time += 100*atol(fields[i]);
    }

    int clk_tck = sysconf(_SC_CLK_TCK);

    double user = (double) 100*clk_tck*atol(fields[0])/total_cpu_time;
    double nice = (double) 100*clk_tck*atol(fields[1])/total_cpu_time;
    double sys = (double) 100*clk_tck*atol(fields[2])/total_cpu_time;
    double idle = (double) 100*clk_tck*atol(fields[3])/total_cpu_time;
    double wait = (double) 100*clk_tck*atol(fields[4])/total_cpu_time;
    double hi = (double) 100*clk_tck*atol(fields[5])/total_cpu_time;
    double si = (double) 100*clk_tck*atol(fields[6])/total_cpu_time;
    double steal = (double) 100*clk_tck*atol(fields[7])/total_cpu_time;

    printf(" %3.2f us, %3.2f sy, %3.2f ni, %3.2f id, %3.2f wa, %3.2f hi, %3.2f si, %3.2f st\n", user, nice, sys, idle, wait, hi, si, steal);

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
            int field_pos_stat[] = {1, 2, 3, 14, 15, 18, 22, 23, 24};
            char** proc_fields = read_fields_from_file(path, field_pos_stat, sizeof(field_pos_stat)/sizeof(int), " ");

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
