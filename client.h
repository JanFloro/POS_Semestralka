#ifndef SEMESTRALNA_PRACA_CLIENT_H
#define SEMESTRALNA_PRACA_CLIENT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "iostream"
#include <ctime>
#include <cstdbool>
#include "limits"
#include <fstream>

typedef struct data {
    int typMravcov;
    int pocetMravcov;

    int vyska;
    int sirka;
    bool **pole;
    int **poleMravcov;

    int pocetZivich;
    bool ukoncenie;
    bool pauznutie;
    pthread_mutex_t mutexData;
} DATA;

int client(int argc, char *argv[]);
void CitanieZoServera(char buffer[512], int sockfd);
void PosielanieNaServer(char buffer[512], int sockfd);

void inicializaciaDat(DATA* d, int typMravcov, int pocetMravcov, int vyska, int sirka, bool **pole, int **poleMravcov, int pocetZivich);
void odstranenieDat (DATA* d);

void* hra (void *d);

void ulozenieMapyDoSuboru(bool** pole, int vyska, int sirka);

bool** nacitanieMapyZoSuboru1(int& vyska, int& sirka);

void posunMravcov (void * d);
void zabiMravca (void* data);
void vykresli(void* d);



#endif //SEMESTRALNA_PRACA_CLIENT_H
