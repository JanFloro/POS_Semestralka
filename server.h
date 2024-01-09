#ifndef SEMESTRALNA_PRACA_SERVER_H
#define SEMESTRALNA_PRACA_SERVER_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdbool>
#include <malloc.h>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <ctime>
#include <cstdbool>
#include "limits"
#include <fstream>
#include <thread>


typedef struct serverData {
    char* buffer;
    int n;
    int newsockfd;
    pthread_mutex_t mutexServer;
} SERVERDATA;


int server(int argc, char *argv[]);

void CitanieZClienta(char buffer[512], int newsockfd);

void PosielanieNaCLienta(char sprava[512], int newsockfd);


void ulozenieServer(bool** pole, int vyska, int sirka);

bool** nacitanieServer(int &vyska, int &sirka);

void clientServer(int newsockfd);

void inicializaciaServerData(SERVERDATA* data);

void odstranenieServerData(SERVERDATA* data);

#endif //SEMESTRALNA_PRACA_SERVER_H

