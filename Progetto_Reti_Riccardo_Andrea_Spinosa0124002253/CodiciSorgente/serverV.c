
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "Helper.h"

// la scadenza del green pass dopo 3 mesi espressa in secondi
#define SCADENZA  7776000

#define SCADENZA2GIORNI 172800


struct Pass {
int servizio;
char tessera[21];
time_t scadenza;
};

int main(int argc, char **argv) {

    struct sockaddr_in serverAddr;
    struct Pass ricevuto;
    struct Pass temp;
    pid_t pid;
    int zero = 0;
    FILE *file = fopen("file", "rb+");
    sem_t *access = sem_open("semaphore", O_CREAT, O_RDWR, 1);
    int listFd;
    int conFd;


    listFd = Socket(AF_INET, SOCK_STREAM, 0);


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(1025);

    /* Assegna l'indirizzo  */
    Bind(listFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    /* accetta connessioni */
    Listen(listFd, 1025);

    /* accettare connessioni all'interno del ciclo finchè vero  */
    while (1) {

        // istanzia connessione
        conFd = Accept(listFd,(struct sockaddr *) NULL,NULL);

        if((pid = fork()) < 0){
          perror("errore di fork");
          exit(-1);
        }

        //sono figlio
        if(pid==0){
            //chiusura del socket listener
          close(listFd);


    if( read(conFd,&ricevuto,sizeof(ricevuto)) != sizeof(ricevuto)){
      perror("errore in lettura");
      exit(1);
    }

            //switch che gestisce le varie richieste che possono avvenire
    switch (ricevuto.servizio) {

      case -1:



    fseek(file,0,SEEK_SET);
    while(fread(&temp,sizeof(struct Pass),1,file) == 1){
      if(strcmp(temp.tessera,ricevuto.tessera) == 0){
        fseek(file,-(sizeof(struct Pass)),SEEK_CUR);
        sem_wait(access);
        fwrite(&ricevuto,sizeof(struct Pass),1,file);
        sem_post(access);
        printf("Tessera aggiornata\n");

        close(conFd);
        exit(0);
        break;
      }
    }



    sem_post(access);


      fseek(file,0,SEEK_END);
      sem_wait(access);
      fwrite(&ricevuto,sizeof(struct Pass),1,file);
      sem_post(access);


      printf("Tessera Sanitaria: %s\n",ricevuto.tessera);
      printf("Scadenza: %.24s\r\n",ctime(&ricevuto.scadenza));

          close(conFd);
          exit(0);
          break;


      case 0:


            /*Si utilizza fseek per posizionare il puntatore del file all'inizio, quindi si utilizza un ciclo while per leggere ogni green pass nel file fino a quando non viene trovato quello corrispondente alla tessera sanitaria richiesta.*/
       fseek(file,0,SEEK_SET);
      while(fread(&temp,sizeof(struct Pass),1,file) == 1){

          /*utilizziamo poi semwait per acquisire l'accesso al file per inviare il Green Pass*/

    if(strcmp(temp.tessera,ricevuto.tessera) == 0){
      sem_wait(access);
      if( write(conFd,&temp,sizeof(struct Pass)) != sizeof(struct Pass)){
        sem_post(access);
        perror("errore in scrittura");
        exit(1);
      }else{
        sem_post(access);
        close(conFd);
        exit(0);
      }
    }
    }

    //caso in cui il green pass non sia stato trovato, ritorniamo 0 per indicate la non presenza dle green pass

        if( write(conFd,&zero,sizeof(int)) != sizeof(int))  {
          perror("errore in scrittura");
          exit(1);
        }


      close(conFd);
      exit(0);

          break;
          int i = 0;


      case 1:
      //la richiesta arriva dal serverG ma viene inviata dal ClientT, gestiamo quindi la convalida del greenPass
    i = 0;
      printf("Convalida greenPass in corso\n");

            /*Si utilizza fseek per posizionare il puntatore del file all'inizio, quindi si utilizza un ciclo while per leggere ogni green pass nel file fino a quando non viene trovato quello corrispondente alla tessera sanitaria specifica */

      fseek(file,0,SEEK_SET);
      while(fread(&temp,sizeof(struct Pass),1,file) == 1){
        if(strcmp(temp.tessera,ricevuto.tessera) == 0){
        //caso in cui il greenPass viene registarto da un tampone
          if(temp.servizio == 3){
            temp.scadenza = time(NULL) + SCADENZA2GIORNI;
          }else{
         //caso in cui il GreenPass viene validato
          temp.scadenza = time(NULL) + SCADENZA;
        }
          printf("Green Pass validato\n");
          fseek(file,i*sizeof(struct Pass),SEEK_SET);
          sem_wait(access);
          fwrite(&temp,sizeof(struct Pass),1,file);
          sem_post(access);
          close(conFd);
          exit(0);
        }else{i++;}
      }
      sem_post(access);


    //nel caso non sia nel file dei vaccinati la tessera sanitaria ottiene il greenPass valido 2 giorni(ha effettuato un tampone)
      strcpy(temp.tessera,ricevuto.tessera);
      temp.scadenza = time(NULL) + SCADENZA2GIORNI;
      temp.servizio = 3;
      printf("Tessera Sanitaria: %s\n",temp.tessera);
      printf("Scadenza: %.24s\r\n",ctime(&temp.scadenza));
      sem_wait(access);
      fseek(file,0,SEEK_END);
      fwrite(&temp,sizeof(struct Pass),1,file);
      sem_post(access);


      //chiudiamo la connessione
      close(conFd);
      exit(0);
        break;



    /*caso  di invalidazione del green pass*/
      case 2:
       i = 0;
      /*Si utilizza fseek per posizionare il puntatore del file all'inizio,
      quindi si utilizza un ciclo while per leggere ogni green pass nel file fino a quando non viene trovato
      quello corrispondente alla tessera sanitaria specifica.*/

       fseek(file,0,SEEK_SET);
       while(fread(&temp,sizeof(struct Pass),1,file) == 1){

         if(strcmp(temp.tessera,ricevuto.tessera) == 0){
           temp.scadenza = time(NULL) - SCADENZA2GIORNI;
           printf("Green Pass invalidato\n");
           fseek(file,i*sizeof(struct Pass),SEEK_SET);
           sem_wait(access);
           fwrite(&temp,sizeof(struct Pass),1,file);
           sem_post(access);
           close(conFd);
           exit(0);
         }else{i++;}
       }
       sem_post(access);

       /*quest'ultimo avrà comunque un greenPass valido grazie all guarigione*/

       strcpy(temp.tessera,ricevuto.tessera);
       temp.scadenza = time(NULL) - SCADENZA2GIORNI;
       printf("Tessera Sanitaria: %s\n",temp.tessera);
       printf("Scadenza: %.24s\r\n",ctime(&temp.scadenza));
       sem_wait(access);
       fseek(file,0,SEEK_END);
       fwrite(&temp,sizeof(struct Pass),1,file);
       sem_post(access);
          break;
    }


    /*Infine, in ogni caso, si chiude la connessione con il client e si esce dal programma con exit(0).
    e chiudiamo l'accesso al file.*/
    fclose(file);
    sem_unlink("semaphore");
    close(conFd);
    exit(0);
        }else{close(conFd);}
      }
      exit(0);


    }

