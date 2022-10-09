#include "libs.h"

int main() {

    //utilizzato per testare le funzioni ausiliarie di proc_liste
    proc* p1 = set_proc(1, 's', 1, 0.4, 0.0);
    proc* p2 = set_proc(2, 's', 4, 0.3, 0.5);
    proc* p3 = set_proc(3, 's', 5, 0.2, 0.3);
    proc* p4 = set_proc(4, 's', 1, 0.1, 0.7);
    
    proc_list* l = (proc_list*)malloc(sizeof(proc_list));
    list_init(l);

    list_destroyer(l);

    list_init(l);

    remove_elem(l, p4);
    list_printer(l);

    insert(l, p1);
    list_printer(l);

    insert(l, p3);
    list_printer(l);

    insert(l, p4);
    list_printer(l);

    insert(l, p2);
    list_printer(l);

    remove_elem(l, p4);

    list_printer(l);

    remove_elem(l, p2);
    
    list_printer(l);

    list_destroyer(l);
    list_printer(l);

    //variabili utilizzate per testare le funzioni di raccolta dati dai file
    char* buf[1024];
    int fd;

    fd = open("/proc/1/stat", O_RDONLY);
    if (fd == -1) errors_handler("Errore in apertura file..");

    *buf = read_row(fd, 1, "PID");
    printf("%s\n", *buf);

    if (close(fd)) errors_handler("Errore in chiusura file...");

    return 0;
}