#include "libs.h"

int main(int argc, char* argv[]) {

	pid_t pid;
	proc_list* l = listing_proc();

	struct sysinfo info;
    sysinfo(&info);

	printf("Top || ");
	local_time();
	get_sys_info(&info);
	printf("Tasks: %d -> ", l->size);
	tasks_info(l);

	printf("Comando (1 carattere) ['h' per vedere l'elenco dei comandi]: ");

	while(1) {
		char cmd = getchar();

		switch (cmd) {

			case '\n':
				l = listing_proc();
				printf("Top || ");
				local_time();
				sysinfo(&info);
				get_sys_info(&info);
				printf("Tasks: %d -> ", l->size);
				tasks_info(l);
				printf("Comando (1 carattere) ['h' per vedere l'elenco dei comandi]: ");
				continue;

			case 'f':
				l = listing_proc();
				printf("Inserire il PID del processo: ");
				proc* p = (proc*) malloc(sizeof(proc));
				scanf("%d", &(p->pid));
				p = proc_finder(l, p, PID_FNDR);
				if (p != NULL) proc_printer(p);
				else printf("Processo non trovato!\n");
				free(p);
				break;

			case 'p':
				l = listing_proc();
				list_printer(l);
				break;

			case'q':
				printf("Chiusura del programma TOP...\n");
				list_destroyer(l);
				sleep(1);
				return 0;

			case 'h':
				printf("\nElenco dei comandi:\n");
				printf("> f: Ricerca il processo con PID scelto\n");
				printf("> k: Invia il segnale kill al processo con PID scelto\n");
				printf("> p: Stampa la lista dei processi\n"); // Temporaneo
				printf("> q: Termina il programma TOP\n");
				printf("> r: Riavvia il processo con PID scelto\n");
				printf("> s: Sospende il processo con PID scelto\n");
				printf("> t: Termina il processo con PID scelto\n\n");
				continue;

			case 'k':
				//KILLING PROCESS

				printf("Inserire il PID del processo: ");
				scanf("%d", &pid);
				
				if (pid == getpid()) {

					char opt;
					printf("Sei sicuro di voler uccidere questo processo? ('y' or 'n') ");
					scanf("%s", &opt);

					if (opt == 'y') {
						if (kill(pid, SIGKILL) == -1) {
							if (errno == EINVAL || errno == EPERM || errno == ESRCH)
								print_error("Errore");
						} else 
							printf("Processo %d ucciso con successo!\n\n", pid); 
					} else printf("\n");

				} else {
					if (kill(pid, SIGKILL) == -1) {
						if (errno == EINVAL || errno == EPERM || errno == ESRCH)
							print_error("Errore");
					} else 
						printf("Processo %d ucciso con successo!\n\n", pid);
				}

				continue;

			case 'r':
				//RESTARTING PROCESS

				printf("Inserire il PID del processo: ");
				scanf("%d", &pid);

				if (kill(pid, SIGCONT) == -1) {
					if (errno == EINVAL || errno == EPERM || errno == ESRCH) {
						print_error("Errore");
						printf("\n");
					}
				} else 
					printf("Processo %d ripreso con successo!\n\n", pid);
				
				continue;

			case 's':
				//SUSPENDING PROCESS

				printf("Inserire il PID del processo: ");
				scanf("%d", &pid);

				if (kill(pid, SIGSTOP) == -1) {
					if (errno == EINVAL || errno == EPERM || errno == ESRCH) {
						print_error("Errore");
						printf("\n");
					}
				} else 
					printf("Processo %d messo in pausa con successo!\n\n", pid);

				continue;

			case 't':
				//TERMINATING PROCESS

				printf("Inserire il PID del processo: ");
				scanf("%d", &pid);
				
				if (pid == getpid()) {

					char opt;
					printf("Sei sicuro di voler terminare questo processo? ('y' or 'n') ");
					scanf("%s", &opt);

					if (opt == 'y') {
						if (kill(pid, SIGTERM) == -1) {
							if (errno == EINVAL || errno == EPERM || errno == ESRCH)
								print_error("Errore");
						} else 
							printf("Processo %d terminato con successo!\n\n", pid); 
					} else printf("\n");

				} else {
					if (kill(pid, SIGTERM) == -1) {
						if (errno == EINVAL || errno == EPERM || errno == ESRCH)
							print_error("Errore");
					} else 
						printf("Processo %d terminato con successo!\n\n", pid);
				}

				continue;

			default:
				printf("\nComando selezionato non esistente..\n\n");
				continue;

		}

	}

	return 0;

}




