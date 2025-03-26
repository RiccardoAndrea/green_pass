#include <stdio.h>
#include<sys/types.h>
#include <string.h>
#include<unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include "Helper.h"

// scadenza del green pass 3 mesi
#define SCADENZA 7776000

// green pass
struct Pass {
  // id del servizio
  int servizio;
  // Numero di tessera
  char tessera[21];
  // scadenza
  time_t scadenza;
};

int main(int argc,char **argv){
  // Struct IP e porta del Centro Vaccinale e del server V
  struct sockaddr_in serverCV, serverV;
  struct Pass greenPass;
  pid_t pid;
  int nClient = 0;
  // socket
  int lisenFd;
  // connessione
  int conFd;
  // serverV
  int serverVFd;
  int n;


  // Controlla che l'IP address del server V sia stato fornito
  if(argc != 2){
    fprintf(stderr,"uso: %s <IPaddress>\n",argv[0]);
    exit(1);
  }

  // socket
  lisenFd = Socket(AF_INET,SOCK_STREAM,0);

  // Inizializzazione della struttura
  serverCV.sin_addr.s_addr = htonl(INADDR_ANY);
  serverCV.sin_port = htons(1024);

  // Associa  socket all'indirizzo e porta indicati
  Bind(lisenFd,(struct sockaddr *)&serverCV,sizeof(serverCV));

  Listen(lisenFd,1024);

  // Accetta connessioni  e crea un processo figlio
  for( ; ;) {
    conFd = Accept(lisenFd,(struct sockaddr *) NULL,NULL);
    nClient++;
    if((pid = fork()) < 0){
    // Errore
      perror("errore nel fork");
      exit( -1);
    }
    // sono figlio
    if(pid == 0)
    {
      close( lisenFd);
      // Chiude socket
      while((n = read(conFd,greenPass.tessera,20)) > 0 ) {
        // controllo che la lunghezza sia 20
        if (n < 20)
            greenPass.tessera[n] = 0;
        else
            greenPass.tessera[20] = 0;
        if (fputs(greenPass.tessera,stdout) == EOF) {
          fprintf(stderr,"error\n");
          exit(1);
        }
        greenPass.scadenza = time(NULL) + SCADENZA;
        greenPass.servizio = -1;

        if(n<0){
          fprintf(stderr,"errore in lettura\n");
          exit(1);
        }

        //invia dati a serverV
        serverVFd = Socket(AF_INET,SOCK_STREAM,0);

        serverV.sin_family = AF_INET;
        serverV.sin_port = htons(1025);

          // ip in formato binario
        if(inet_pton(AF_INET,argv[1], &serverV.sin_addr) < 0)
        {
            // errore
            fprintf(stderr, "Errore per %s\n",argv[1]);
            // Esci
            exit(1);
        }
        // Connessione al server
        Connect(serverVFd,(struct sockaddr *)&serverV,sizeof(serverV));

        // Scrive green pass
        if( write(serverVFd,&greenPass,sizeof(greenPass)) != sizeof(greenPass) )
        {
        // errore
          perror("Errore in scrittura");
        // Esci
          exit(1);
        }

        printf("Consegna avvenuta con successo\n");


        close(serverVFd);

        close(conFd);
        exit(0);


    }

exit(0);


}else {close(conFd);}
}
}

