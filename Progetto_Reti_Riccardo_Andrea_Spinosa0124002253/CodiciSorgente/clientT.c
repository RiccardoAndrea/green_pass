#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "Helper.h"
#include <time.h>



struct Pass {
  int servizio;
  char tessera[21];
  time_t scadenza;
};

int main(int argc, char** argv){
int sockFd;
int n;
struct sockaddr_in serverAddr;
struct Pass greenPass;
// Stringa valdiità idionea
char valido[] = "V";
// Stringa per invalido
char invalido[] = "I";

//se la il numero di argomento non è giusto
if(argc != 4){
fprintf(stderr,"Uso: %s <indirizzo IP> <Tessera Sanitaria> <V o I>\n",argv[0]);
exit(1);
}

// Controllo lunghezza tessera sanitaria
if(strlen(argv[2]) != 20){
fprintf(stderr,"Tessera non valida\n");
exit(1);
}

// Copia della tessera
strcpy(greenPass.tessera,argv[3]);

// Impostazione del servizio in base all'ingresso dato
if(strcmp(argv[3],valido) == 0){
greenPass.servizio = 1;
} else if(strcmp(argv[3],invalido) == 0){
greenPass.servizio = 2;
} else {
printf("%s il servizio non è valido",argv[3]);
exit(1);
}

// socket creata
sockFd = Socket(AF_INET,SOCK_STREAM,0);

// struttura serverAddr
serverAddr.sin_family = AF_INET;
serverAddr.sin_port = htons(1026);
if(inet_pton(AF_INET,argv[1], &serverAddr.sin_addr) < 0) {
fprintf(stderr,"Errore per %s\n",argv[1]);
exit(1);
}

// Connessione al server
Connect(sockFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));

// Scrittura del greenPass e invio richiesta al server
if( write(sockFd,&greenPass,sizeof(greenPass)) != sizeof(greenPass)) {
perror("Errore di scrittura");
exit(1);
}
// la richiesta è stata inviata con successo
printf("Richiesta inviata\n");
close(sockFd);
exit(0);

}
