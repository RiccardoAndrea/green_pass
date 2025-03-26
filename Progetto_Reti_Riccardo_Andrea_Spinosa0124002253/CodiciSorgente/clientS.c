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
    // Descrittore del socket
    int sockFd;
    // Indirizzo del server
    struct sockaddr_in serverAddr;
    // Struct per la tessera sanitaria
    struct Pass greenPass;

    // Verifica inserimento
    if(argc != 3){
    fprintf(stderr, "Uso: %s <Indirizzo IP> <Tessera Sanitaria>\n",argv[0]);
    exit(1);
    }

    // se lunghezza della tessera sanitaria non è di 20 caratteri
    if(strlen(argv[2]) != 20){
    fprintf(stderr, "Tessera invalida\n");
    exit(1);
    }

    // Preparo la struttura greenPass
    strcpy(greenPass.tessera,argv[2]);
    greenPass.servizio = 0;

    // Creo il socket
    sockFd = Socket(AF_INET, SOCK_STREAM, 0);

    // Preparo l'indirizzo del server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1026);
    if(inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) < 0) {
    fprintf(stderr, "Errore per %s\n",argv[1]);
    exit(1);
    }

    // Effettuo la connessione al server
    Connect(sockFd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Scrivo la tessera sanitaria e la richiesta al serverG
    if(write(sockFd, &greenPass, sizeof(greenPass)) != sizeof(greenPass)) {
    perror("Errore di scrittura");
    exit(1);
    }
    printf("Richiesta in corso\n");

    // Leggo la risposta del serverG riguardante la tessera sanitaria richiesta
    if(read(sockFd, &greenPass, sizeof(greenPass)) != sizeof(greenPass)){
    perror("Errore di lettura");
      exit(1);
    }


    //valido
    if(greenPass.scadenza >= time(NULL)){
      printf("Greenpass : valido\n");
    }else {//invalido
      printf("Greenpass: non valido\n");
    }
    close(sockFd);
    exit(0);



}
