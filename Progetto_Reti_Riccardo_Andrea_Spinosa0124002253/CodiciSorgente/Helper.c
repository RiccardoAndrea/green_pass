//
//  Helper.h
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>



/*
 * Questa funzione si connette ad un server tramite il socket con descrittore 'sockFd'
 * e l'indirizzo 'addr' di lunghezza 'addrlen'.
 * In caso di errore, stampa un messaggio di errore e termina il programma con exit(1).
 */
int Connect(int sockFd, const struct sockaddr *addr,socklen_t addrLen){
  if(connect(sockFd,addr,addrLen) < 0){
    perror("errore connessione");
    exit(1);
  }
}



/*
 * Questa funzione imposta la socket 'sockFd' in modalità di ascolto con una lunghezza massima della coda di attesa di 'queue_lenght'.
 * In caso di errore, stampa un messaggio di errore e termina il programma con exit(1).
 */
int Listen(int sockFd,int queLen){
  if(listen(sockFd,queLen) < 0){
    perror("errore listen");
    exit(1);
  }
}

/*
 * Questa funzione accetta una connessione in entrata sulla socket 'sockFd' e restituisce il descrittore associato alla nuova connessione.
 * La struttura 'clientaddr' memorizza l'indirizzo del client, mentre 'addr_dim' ne memorizza la lunghezza.
 * In caso di errore, stampa un messaggio di errore e termina il programma con exit(1).
 */
int Accept(int sockFd,struct sockaddr *restrict clientAddr,socklen_t *restrict addrDime){
int n;
if((n = accept(sockFd,clientAddr, addrDime))<0){
  perror("errore accept");
  exit(1);
}
return n;
}

/*
 * Questa funzione crea un socket e restituisce il descrittore associato al socket.
 * In caso di errore, stampa un messaggio di errore e termina il programma con exit(1).
 */
int Socket(int family,int type,int protocol){
  int n;
  if((n = socket(family,type,protocol))<0){
    perror("errore socket");
    exit(1);
  }
  return n;
}

/*
 * Questa funzione associa un indirizzo alla socket 'sockFd' tramite la struttura 'serverAddr' di lunghezza 'addrLen'.
 * In caso di errore, stampa un messaggio di errore e termina il programma con exit(1).
 */
int Bind(int sockFd, const struct sockaddr *serverAddr,socklen_t addrLen){
  if(bind(sockFd,serverAddr, addrLen) < 0){
    perror("errore bind");
    exit(1);
  }
}
