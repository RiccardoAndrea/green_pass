Green Pass


Il progetto consiste in un sistema distribuito per la gestione dei Green Pass, realizzato per l'esame di reti dei calcolatori che permette:

Registrazione di un Green Pass dopo la vaccinazione

Verifica della validità

Invalidazione/Ripristino in caso di contagio o guarigione

Il sistema è composto da:

Client per utenti vaccinati (comunica il codice tessera sanitaria al centro vaccinale)

ServerV (gestisce la validità dei Green Pass)

ServerG (verifica lo stato dei Green Pass interrogando ServerV)

ClientS (verifica la validità di un Green Pass)

ClientT (invalida/ripristina Green Pass in caso di contagio/guarigione)

il tutto è fatto mediante socket.

compilare il progetto e da terminale eseguire i test.

