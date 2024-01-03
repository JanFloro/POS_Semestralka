#include "server.h"

using namespace std;


int citaniePodmienkaServer;

int server(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[512];

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }

    //-------------------------------------------Main Kód---------------------------------
    int sirka, vyska, pocetMravcov, typMravcov, pocetZM, typGenerovaniaPlohy, pocetChyb = 0, typCitania;

    //---------Server sa opyta ci chce client vytvorit mapu alebo nacitat zo suboru
    do {
        PosielanieNaCLienta(" Vyber si moznost - \n 1.Vytvorit mapu: \n 2.Nacitat mapu zo suboru: \n ",n,newsockfd);
        citaniePodmienkaServer = CitanieZClienta(buffer,n,newsockfd);
        if (citaniePodmienkaServer == 1 || citaniePodmienkaServer == 2) {
            break;
        }
    } while (true);

    if (citaniePodmienkaServer == 1) {
        // Vyska
        do {
            PosielanieNaCLienta("Zadaj vysku mapy min 5: \n",n,newsockfd);
            vyska = CitanieZClienta(buffer,n,newsockfd);
            if (vyska >= 5) {
                break;
            }
        } while (true);
        //Sirka
        do {
            PosielanieNaCLienta("Zadaj sirku mapy min 5: \n",n,newsockfd);
            sirka = CitanieZClienta(buffer,n,newsockfd);
            if (vyska >= 5) {
                break;
            }
        } while (true);

        //--------------------------------------------Generovanie mapy
        PosielanieNaCLienta(" Vyber si moznost - \n 1.Nahodne cierne policka: \n 2.Rucne napisat cierne policka: \n ",n,newsockfd);
        citaniePodmienkaServer = CitanieZClienta(buffer,n,newsockfd);
        if (citaniePodmienkaServer == 1) {
            srand(time(0));
            //----------Hracie pole
            int **pole = new int* [vyska];
            for (int i = 0; i < vyska; i++) {
                pole[i] = new int [sirka];
            }
            for (int i = 0; i < vyska; ++i) {
                for (int j = 0; j < sirka; ++j) {
                    if ((rand() % RAND_MAX) < 0.5 ) {
                        pole[i][j] = 0;
                    } else {
                        pole[i][j] = 1;
                    }
                }
            }
        } else {
            //Urceni pocet ciernich policok

        }

    } else {
        //TODO     Nacitanie mapy zo suboru
    }


    //----------Server sa opyta na pocet mravcou a pyta sa to stale dokym client neda rozumne cislo
    do {
        PosielanieNaCLienta("Zadaj kolko mravcou chces mat na mape: \n",n,newsockfd);
        pocetMravcov = CitanieZClienta(buffer,n,newsockfd);
    } while (pocetMravcov == 0); // Ak client nazada rozumne cislo tak sa to opakuje


    //---------------------------------------------Typ mravcov
    do {
        PosielanieNaCLienta("Zadaj aky typ mravcou chces : \n 1.Priamich: \n 2.Inverznich: \n ",n,newsockfd);
        typMravcov = CitanieZClienta(buffer,n,newsockfd);
        if (typMravcov == 1 || typMravcov == 2) {
            break;
        }
    } while (true);

    DAT data = {.typMravcov = typMravcov, .pocetMravcov = pocetMravcov, .vyska = vyska, .sirka = sirka} ;
    printf("Pocet mravcov : %d \n", data.pocetMravcov);
    printf("Typ mravcoc : %d \n", data.typMravcov);
    printf("Vyska : %d \n", data.vyska);
    printf("Pocet sirka : %d \n", data.sirka);
        //----------------------------------------Koniec MAIN-----------------------------------

    close(newsockfd);
    close(sockfd);

    return 0;
}

int CitanieZClienta(char buffer[256], int n, int newsockfd)
{
    bzero(buffer,256);
    n = read(newsockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
    }
    int cislo = atoi(buffer);
    return cislo;
}

void PosielanieNaCLienta(char sprava[512], int n, int newsockfd) {
    n = write(newsockfd, sprava, strlen(sprava));
    if (n < 0) {
        perror("Error writing to socket");
    }
}