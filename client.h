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

typedef struct dataC {
    char *ckoniec;
    char *cbuffer;
    int cn;
    int csockfd;
} DATC;

int client(int argc, char *argv[]);
int CitanieZoServera(char buffer[512],int n, int sockfd);
int PosielanieNaServer(char buffer[256], int n, int sockfd);

#endif //SEMESTRALNA_PRACA_CLIENT_H
