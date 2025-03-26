#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include<errno.h>
#include<netdb.h>
#include "Helper.h"
#include <time.h>



struct Pass {
  char tessera[21];
  time_t scadenza;
  int servizio;
};

int main(int argc,char **argv) {


  int sockFd,lisenFd,n,requestFd;  //sockFd: descrittore per connettersi a clientS
  struct Pass greenPass;  // lisenFd: descrittore per ascolare nuove connessioni da clientS o clientT
  struct sockaddr_in serverAddr;
  struct sockaddr_in requestAddr;
  pid_t pid;

    // Controllo il numero di argomenti

  if(argc != 2){
    fprintf(stderr,"uso: %s <IPaddress> \n",argv[0]);
    exit(1);
  }


    // Creazione del socket per ascoltare nuove connessioni
  lisenFd = Socket(AF_INET,SOCK_STREAM,0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddr.sin_port = htons(1026);
  Bind(lisenFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr));
  Listen(lisenFd,1026);

    // Ciclo per accettare nuove connessioni e gestirle in un processo figlio
for( ; ; )
{

sockFd = Accept(lisenFd,(struct sockaddr *)NULL,NULL);

if((pid = fork()) < 0 ){
  perror("fork error");
  exit(-1);
}

if(pid == 0){
  close(lisenFd);

    // Ricezione dei dati della Green Pass e richiesta di servizio da parte del client
   while((n = read(sockFd,&greenPass,sizeof(greenPass))) > 0){


  greenPass.tessera[21] = 0;

  printf("Tessera Sanitaria: %s\n",greenPass.tessera);

  if(n<0){
    fprintf(stderr,"errore in lettura\n");
    exit(1);
  }


           // Creazione del client per connettersi a serverV
    requestFd = Socket(AF_INET,SOCK_STREAM,0);

    requestAddr.sin_family = AF_INET;
           // Impostazione del numero di porta
    requestAddr.sin_port = htons(1025);

       // Converte un indirizzo di stringa in un indirizzo di rete, argv[1] contiene l'indirizzo IP
  if(inet_pton(AF_INET,argv[1], &requestAddr.sin_addr) < 0) {
    fprintf(stderr,"Errore per %s\n",argv[1]);
    exit(1);
  }

  //connessione  a serverV
  Connect(requestFd,(struct sockaddr *) &requestAddr,sizeof(requestAddr));
       // Scrittura su serverV
  if( write(requestFd,&greenPass,sizeof(greenPass)) != sizeof(greenPass)) {
    perror("write");
    exit(1);
  }
  printf("Richiesta Consegnata\n");
  printf("Servizio : %d\n",greenPass.servizio);

       //serivizio clientS
  if(greenPass.servizio == 0){
    int r = read(requestFd,&greenPass,sizeof(greenPass));

      //se green pass non esiste
    if( (r != sizeof(greenPass))  && (r!= sizeof(int))   ){
      perror("read");
      exit(1);
    }else if(r < sizeof(greenPass)){
      printf("Green Pass non trovato\n");
    }else{

  // greenPass a clientS
  if( write(sockFd,&greenPass,sizeof(greenPass)) < sizeof(greenPass)){
    perror("Errore in scrittura");
    exit(1);
  }
    }



    close(sockFd);
    close(requestFd);
    exit(0);

    //caso ClientT
    }else if(greenPass.servizio == 1 || greenPass.servizio ==2){

    close(sockFd);
    close(requestFd);
    exit(0);
    }

          }


        }

      }
      close(sockFd);
      exit(0);
    }




