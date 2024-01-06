#ifndef SEMESTRALNA_PRACA_SERVER_H
#define SEMESTRALNA_PRACA_SERVER_H
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cstdbool>
#include <malloc.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>


int server(int argc, char *argv[]);

int CitanieZClienta(char buffer[256], int n, int newsockfd);

void PosielanieNaCLienta(char sprava[512], int n, int newsockfd);



#endif //SEMESTRALNA_PRACA_SERVER_H

