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

typedef struct data {
    int typMravcov;
    int pocetMravcov;

    int vyska;
    int sirka;
    bool **pole;
    int **poleMravcov;

    int pocetZM;
    int cykl;
    int n;
    int newsockfd;
    pthread_mutex_t* mutex;
} DATA;

int client(int argc, char *argv[]);
int CitanieZoServera(char buffer[512],int n, int sockfd);
int PosielanieNaServer(char buffer[256], int n, int sockfd);

void posunMravcov (DATA* d);
void vykresli(DATA* d);


#endif //SEMESTRALNA_PRACA_CLIENT_H
