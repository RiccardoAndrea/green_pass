
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "Helper.h"
#include <time.h>

int main(int argc,char **argv) {
    // descrittore socket
    int sockFd;
    // server address
    struct sockaddr_in serverAddr;

    // Verifica argomenti
    if(argc != 3){
    fprintf(stderr,"Uso: %s <Indirizzo IP> <Tessera Sanitaria>\n",argv[0]);
    exit(1);
    }

    // lunghezza tessera sanitaria
    if(strlen(argv[2]) != 20){
    //errore
        fprintf(stderr,"Tessera non valida \n");
        exit(1);
    }

    // Creazione socket
    sockFd = Socket(AF_INET,SOCK_STREAM,0);

    // Impostazione parametri della connessione
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1024);

    // Conversione da stringa a binario
    if(inet_pton(AF_INET,argv[1], &serverAddr.sin_addr) < 0) {
    fprintf(stderr,"Errore per %s\n",argv[1]);
    exit(1);
    }
    // Connessione al server
    Connect(sockFd,(struct sockaddr *) &serverAddr,sizeof(serverAddr));

    // Invio della tessera sanitaria
    if( write(sockFd,argv[2],strlen(argv[2])) != strlen(argv[2])) {
      perror("errore in scrittura");
      exit(1);
    }
    printf("Tessera Consegnata\n");

    // Chiude il socket
    close(sockFd);
}

