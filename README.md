# SO_Prjct_2022
Implementazione di un monitor di processi che permetta di:
-terminare,
-sospendere,
-riavviare,
-uccidere (invio di segnale kill)
i processi in esecuzione.
L'accesso ai processi è permesso attraverso il filesystem '/proc'.
Il programma monitora le risosrse utilizzate da ogni programma (utillizzo memoria e CPU).

La directory è composta di:
> Libreria nel file libs.h;
> Definizione dei metodi nel file libs.c;
> Compilazione attraverso Makefile;
> Esecuzione attraverso il file eseguibile 'top' (comando './top').
