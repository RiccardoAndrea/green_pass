# Green Pass

**Green Pass** è un sistema distribuito per la gestione dei Green Pass, sviluppato per l'esame di Reti dei Calcolatori. Il sistema consente la registrazione, la verifica della validità e l'invalidazione/ripristino dei Green Pass in caso di contagio o guarigione.

## Funzionalità

- **Registrazione Green Pass**: Dopo la vaccinazione, un Green Pass viene registrato nel sistema.
- **Verifica della validità**: Il sistema consente di verificare la validità di un Green Pass.
- **Invalidazione/Ripristino**: In caso di contagio o guarigione, il Green Pass può essere invalidato o ripristinato.
  
## Componenti del sistema

Il sistema è composto da più componenti che comunicano tra loro tramite socket:

- **Client (utenti vaccinati)**: Comunica il codice tessera sanitaria al centro vaccinale per la registrazione del Green Pass.
- **ServerV**: Gestisce la validità dei Green Pass.
- **ServerG**: Verifica lo stato dei Green Pass interrogando il **ServerV**.
- **ClientS**: Verifica la validità di un Green Pass.
- **ClientT**: Invalida o ripristina il Green Pass in caso di contagio o guarigione.

## Tecnologie utilizzate

- **Socket**: La comunicazione tra i vari componenti del sistema avviene tramite socket.
- **Linguaggio di programmazione**: Il progetto è sviluppato in linguaggio **C**
  

