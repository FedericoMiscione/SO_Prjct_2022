#include "libs.h"

//Variabili globali
proc_list running_list;
proc_list zombie_list;
proc_list waiting_list;


int main(int argc, char* argv[]) {

    int pageSize = getpagesize();
	printf("Memory page size : %d\n\n",pageSize);
	//getchar();
	char cmd[2]; //Dimensione della stringa da valutare in base ai comandi che possono essere eseguiti 
				 //[Per ora sarebbe sufficiente un char]

	printf("Top || ");
	local_time();
	printf(" || \n");
	// uptime ***da rivedere***
	//file_reader("/proc/uptime");
	//printf(" ||\n");
	
	while(1) {
		printf("Comando: ");
		int N = 0;
		char c = getchar();

		//utilizzato per testare le funzioni ausiliarie per le liste
		/*
		proc* p1 = set_proc(1, 's', 1, 0.0, 0.0);
		proc* p2 = set_proc(2, 's', 1, 0.0, 0.0);
		proc* p3 = set_proc(3, 's', 1, 0.0, 0.0);
		proc* p4 = set_proc(4, 's', 1, 0.0, 0.0);
		
		proc_list* l = (proc_list*)malloc(sizeof(proc_list));
		list_init(l);
		*/
		
		if (c != '\n' && N <= 2) {
			cmd[N++] = c;
			c = getchar();
		}

		//variabili utilizzata per testare le funzioni di raccolta dati dai file
		/*
		char* buf[1024];
		FILE* fd;
		*/

		switch(*cmd) {

			case 'q':
				printf("Chiusura del programma TOP...\n");
				return 0;

			//utilizzato per testare le funzioni ausiliarie per le liste
			/*
			case 'c':

				list_destroyer(l);

				remove_elem(l, p4);
				list_printer(l);

				insert(l, p1);
				insert(l, p3);
				insert(l, p4);
				insert(l, p2);
				list_printer(l);

				remove_elem(l, p4);
				list_printer(l);

				remove_elem(l, p2);
				list_printer(l);

				list_destroyer(l);
				list_printer(l);

				continue;
			*/

			//utilizzato per testare le funzioni di raccolta dati dai file
			/*
			case 'f':
				fd = fopen("/home/epicmusk/Scrivania/mio_file.txt", "r");
				if (fd == NULL) errors_handler("Errore in apertura file..");

				*buf = read_row(fd, 2);
				if (*buf != NULL) printf("%s", *buf);

				if (fclose(fd) == EOF) errors_handler("Errore in chiusura file...");
				continue;
			*/

			case 'k':
				//KILLING PROCESS
				//.
				//Richiedere PID del processo da terminare
				//.
				continue;

			case 'r':
				//RESTARTING PROCESS
				//.
				//Richiedere PID del processo da riavviare
				//.
				continue;

			case 's':
				//SUSPENDING PROCESS
				//.
				//Richiedere PID del processo da sospendere
				//.
				continue;

			case 't':
				//TERMINATING PROCESS
				//.
				//Richiedere PID del processo da terminare
				//.
				continue;

			default:
				continue;
		}



	}

	return 0;

	//.
	//Stampa sulla shell delle specifiche generali in intestazione di Top generico
	//.
	//.
	//.
	//Stampa dei processi in /proc su shell
	//.
	//.
	//.
	//Implementazione dei comandi richiesti da specifiche
	//.
	//.
	//.
	//Chiusura della directory /proc
	//.

}




